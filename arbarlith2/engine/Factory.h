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
#define GEN_RTTI(TYPE)                                                                    \
static _tstring getTypeString()                                                           \
{                                                                                         \
	return toTString(typeid(TYPE).name());                                            \
}                                                                                         \
static OBJECT_TYPE getType()                                                              \
{                                                                                         \
	const type_info *typePtr = &typeid(TYPE);                                         \
	return (OBJECT_TYPE)(typePtr);                                                    \
}

// Evaluates true when the given object is of the given type or extends that type
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
template<class TYPE> class Factory : public map<OBJECT_ID, TYPE*>
{
public:
	/** Default constructor */
	Factory(void)
	{
		clear();
	}

	/** Destructor */
	~Factory(void)
	{
		destroy();
	}

	/** Cleanly destroys the objects before clearing the records of them */
	void destroy(void)
	{
		while(!empty())
		{
			iterator i = begin();
			TYPE *a = i->second;
			delete(a);
			erase(i);
		}
	}

	/**
	Removes a single object from the database
	@param handle ID of the object to delete
	*/
	void remove(OBJECT_ID handle)
	{
		iterator i = find(handle);
		ASSERT(i != end(), _T("Failed to find object by handle"));
		TYPE *a = i->second;
		delete(a);
		erase(i);
	}

	/** Registers a type with the factory database */
	template<class T>
	void registerType(void)
	{
		const type_info& typeInfo = typeid(T);
		const char *pszName = typeInfo.name();
		const _tstring typeName = toTString(pszName);
		const OBJECT_TYPE typeID = (OBJECT_TYPE)(&typeInfo); // address of this is constant

		if(toTypeID.find(typeName) == toTypeID.end())
		{
			toTypeID.insert(  make_pair(typeName,typeID)  );

			AllocatorFn fn = &::Engine::allocatorFn<TYPE, T>;
			toAllocator.insert(  make_pair(typeID, fn)  );
		}
	}

	/**
	Creates an object
	@param type The type of the object
	@return a pointer to the object
	*/
	TYPE* createPtr(const _tstring &type)
	{
		map<_tstring, OBJECT_TYPE>::iterator iter = toTypeID.find(type);

		if(iter == toTypeID.end())
		{
			return 0;
		}

		OBJECT_ID handle = create(iter->second);
		return find(handle)->second;
	}

	/**
	Creates an object
	@param type The type of the object
	@return The unique identifier to the new object
	*/
	OBJECT_ID create(const _tstring &type)
	{
		map<_tstring, OBJECT_TYPE>::iterator iter = toTypeID.find(type);

		return (iter == toTypeID.end()) ? INVALID_ID : create(iter->second);
	}

	/**
	Creates an object
	@param type The type of the object
	@return The unique identifier to the new object
	*/
	OBJECT_ID create(OBJECT_TYPE type)
	{
		OBJECT_ID handle = uniqueID++;

		map<OBJECT_TYPE, AllocatorFn>::iterator iter = toAllocator.find(type);
		ASSERT(iter!=toAllocator.end(), _T("The specified type was not found"));

		AllocatorFn allocatorFn = iter->second;
		TYPE *o = allocatorFn(handle);
		ASSERT(o!=0, _T("Allocator failed"));

		insert(make_pair(handle, o));

		return handle;
	}

private:
	/** Prototype for the allocator function */
	typedef TYPE* (*AllocatorFn)(OBJECT_ID);

	/** Tracks the incrementing unique ID for the next object to be created */
	static OBJECT_ID uniqueID;

	/** Maps from the type name to the type ID */
	map<_tstring, OBJECT_TYPE> toTypeID;

	/** Maps from the type to the allocator function */
	map<OBJECT_TYPE, AllocatorFn> toAllocator;
};

}; // namespace

#endif
