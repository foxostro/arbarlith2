/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Game Creation Society nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE Game Creation Society ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE Game Creation Society BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _FACTORY_H_
#define _FACTORY_H_

namespace Engine {

typedef int OBJECT_ID;
typedef size_t OBJECT_TYPE;
const OBJECT_ID INVALID_ID = -1;

#include "tstring.h"

// Generates static RTTI accessor methods for a class
#define GEN_RTTI(TYPE, NAME)                                                              \
static string getTypeString(void)                                                       \
{                                                                                         \
	return string( NAME );                                                      \
}                                                                                         \
static OBJECT_TYPE getType(void)                                                          \
{                                                                                         \
	const type_info *typePtr = &typeid(TYPE);                                         \
	return (OBJECT_TYPE)(typePtr);                                                    \
}

// Evaluates true when the object is of the given type or extends that type
#define instanceof(o, TYPE)     (dynamic_cast<const TYPE *>(&o)!=0)

/** Templated allocator function is acceptable in nearly all instances */
template<class TYPE, class SUB_TYPE> TYPE* allocatorFn(OBJECT_ID handle)
{
	return new SUB_TYPE(handle);
}

/**
Factory pattern generator.
Allocates specific objects when passed the type name of the desired object.
Parameters for constructors are assumed to be very uniform, only accepting
a copy of its own handle within the factory.
*/
template<class TYPE> class Factory
{
public:
	/** Prototype for the allocator function */
	typedef TYPE* (*AllocatorFn)(OBJECT_ID);
	typedef map < OBJECT_ID, TYPE* > mapHandleToObject;

	/** Stores objects that have been allocated */
	mapHandleToObject objects;

private:

	typedef map < string, OBJECT_TYPE > mapStringToType;
	typedef map < OBJECT_TYPE, AllocatorFn > mapTypeToAlloc;

	/** Tracks the incrementing UID for the next object to be created */
	static OBJECT_ID uniqueID;

	/** Maps from the type name to the type ID */
	mapStringToType toTypeID;

	/** Maps from the type to the allocator function */
	mapTypeToAlloc toAllocator;

public:
	/** Default constructor */
	Factory(void)
	{}

	/** Destructor */
	~Factory(void)
	{
		destroy();
	}

	/** Cleanly destroys the objects before clearing the records of them */
	void destroy(void)
	{
		while(!objects.empty())
		{
			delete(objects.begin()->second);
			erase(objects.begin());
		}
	}

	/**
	Removes a single object from the database
	@param handle ID of the object to delete
	*/
	void remove(OBJECT_ID handle)
	{
		delete(objects.find(handle)->second);
		objects.erase(objects.find(handle));
	}

	/**
	Registers a type with the factory database
	@param typeName Name of the type (type_info::name is implementation dependent)
	*/
	template<class T>
	void registerType(const string &typeName)
	{
		const type_info& typeInfo = typeid(T);
		const OBJECT_TYPE typeID = (OBJECT_TYPE)(&typeInfo); // address of this is constant

		if(toTypeID.find(typeName) == toTypeID.end())
		{
			toTypeID.insert(  make_pair(typeName,typeID)  );

			AllocatorFn fn = &::Engine::allocatorFn<TYPE, T>;
			toAllocator.insert(  make_pair(typeID, fn)  );

			TRACE("Registered type \"" + typeName + "\"");
		}
	}

	/**
	Creates an object
	@param type The type of the object
	@return a pointer to the object
	*/
	TYPE* createPtr(const string &type)
	{
		mapStringToType::iterator iter = toTypeID.find(type);

		if(iter == toTypeID.end())
		{
			return 0;
		}

		OBJECT_ID handle = create(iter->second);
		return objects.find(handle)->second;
	}

	/**
	Creates an object
	@param type The type of the object
	@return The unique identifier to the new object
	*/
	OBJECT_ID create(const string &type)
	{
		mapStringToType::iterator iter = toTypeID.find(type);

		return (iter == toTypeID.end()) ? INVALID_ID : create(iter->second);
	}

	/**
	Creates an object
	@param type The type of the object
	@return The unique identifier to the new object
	*/
	OBJECT_ID create(OBJECT_TYPE type)
	{
		OBJECT_ID handle = uniqueID;
		uniqueID++;

		TYPE *o = (toAllocator.find(type)->second)(handle);
		ASSERT(o!=0, "Allocator failed");

		objects.insert(make_pair(handle, o));

		return handle;

		/*
		mapTypeToAlloc::iterator i;

		iter = toAllocator.find(type);
		ASSERT(iter!=toAllocator.end(), "The specified type was not found");

		OBJECT_ID handle = uniqueID;
		uniqueID++;

		TYPE *o = (iter->second)(handle);
		ASSERT(o!=0, "Allocator failed");

		objects.insert(make_pair(handle, o));

		return handle;*/
	}
};

} // namespace Engine

#endif
