/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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

#ifndef _ACTOR_SET_H_
#define _ACTOR_SET_H_

#include <queue>
using std::queue;

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include "object.h"
#include "ActorFactory.h"

namespace Engine {

class Frustum;
class World;

/** Collection of associated actors, by reference */
class ActorSet : public map<const Engine::OBJECT_ID, Engine::Actor*>
{
public:
	/** Creates an empty set */
	ActorSet(void)
	{
		clear();
	}

	/**
	Creates a set of objects from an XML data source
	@param xml The XML data source
	@param zone The home zone of the object
	*/
	ActorSet(const PropertyBag &xml, World *zone)
	{
		ASSERT(zone!=0, "zone was NULL");
		clear();
		load(xml, zone);
	}

	/** Safely destroys and resets the ActorSet to an empty set */
	void destroy(void);

	/**
	Creates a set of objects from an XML data source
	@param xml The XML data source
	@param world The home world of the object
	*/
	void load(const PropertyBag &xml, World *world);

	/** Saves the set of objects to an XML data source */
	PropertyBag save(void) const;

	/**
	Determines whether an object is a member of the ActorSet
	@param id The GUID of the object
	@return true if the object is contained within the set, false otherwise
	*/
	bool isMember(OBJECT_ID id) const;

	/**
	Creates an Actor, caches it in the global ActorFactory
	Actor is then added to the set
	@param type The type of the object
	@param zone The starting realm of the object
	@return The unique identifier to the new object
	*/
	OBJECT_ID create(const string &type, World *zone);

	/**
	Creates an object
	@param type The type of the object
	@param zone The starting realm of the object
	@return a pointer to the object
	*/
	Actor* createPtr(const string &type, World *zone);

	/**
	Gets an object from the set
	@param id The ID of the object to retrieve
	@return reference of the object
	*/
	Actor& get(OBJECT_ID id);

	/**
	Gets an object from the set
	@param name Name of the object
	@return reference of the object
	*/
	vector<Actor*> getByName(const string &name);

	/**
	Gets an object from the set
	@param id The ID of the object to retrieve
	@return reference of the object
	*/
	Actor* getPtr(OBJECT_ID id);

	/**
	Gets an object from the set
	@param id The ID of the object to retrieve
	@return reference of the object
	*/
	const Actor* getPtr(OBJECT_ID id) const;

	/**
	Gets an object from the set
	@param id The ID of the object to retrieve
	@return reference of the object
	*/
	const Actor& get(OBJECT_ID id) const;

	/**
	Draws all objects in the set
	@param frustum The frustum of the camera
	*/
	void draw(const Frustum &frustum) const;

	/** Draws all objects in the set to the depth buffer */
	void drawToDepthBuffer(void) const;

	/**
	Update the objects
	@param deltaTime The milliseconds between now and the last tick
	@param zone zone
	*/
	void update(float deltaTime, World *zone);

	/**
	Add the objects from one actor set to another.
	The original actor set can still destroy the objects it contains; be careful!
	*/
	void addAll(ActorSet &s);

	/**
	Removes the object from the set and deletes it
	@param id The object to remove
	*/
	void removeObjectNow(OBJECT_ID id);

	/**
	Moves the object from one set to another
	@param dest The destination actor set
	@param id The object to move
	*/
	void moveObject(ActorSet &dest, OBJECT_ID id);

	/**
	Determines whether the ActorSet contains an Actor of the specified name
	@param name The name of the actor
	@param out Returns an ID to the actor, or INVALID_ID
	@return true if the actor was found
	*/
	bool query(const string &name, OBJECT_ID &out) const;

	/**
	Gets all objects within the frustum
	@param frustum The frustum
	@return set of objects
	*/
	ActorSet isWithin(const Frustum &frustum) const;

	/**
	Gets all objects not within the frustum
	@param frustum The frustum
	@return set of objects
	*/
	ActorSet isNotWithin(const Frustum &frustum) const;

	/** Gets all objects within the frustum that have moved */
	ActorSet haveMoved(void) const;

	/** Delete all actors of the templated type */
	template<class T>
	void deleteActors(void)
	{
		for_each(begin(), end(), &ActorSet::deleteActor<T>);
	}

	/**
	Excludes a particular object from the result set
	@param id ID of the exluded object
	*/
	ActorSet exclude(OBJECT_ID id) const;

	/**
	gets the object closest to the given position, or INVALID_ID if the object would be greater than the threshold diatance
	@param pos position
	@param threshold threshold distance
	@return ID of an object
	*/
	template<class TYPE>
	ActorSet excludeType(void) const
	{
		ActorSet s;

		// collect the distances from the position to each actor
		for(const_iterator iter = begin(); iter!=end(); ++iter)
		{
			if(!instanceof(*(iter->second), TYPE))
				s.insert(*iter);
		}

		return s;
	}

	/**
	gets the object closest to the given position, or INVALID_ID if the object would be greater than the threshold diatance
	@param pos position
	@param threshold threshold distance
	@return ID of an object
	*/
	template<class TYPE>
	ActorSet typeFilter(void) const
	{
		ActorSet s;

		// collect the distances from the position to each actor
		for(const_iterator iter = begin(); iter!=end(); ++iter)
		{
			if(instanceof(*(iter->second), TYPE))
				s.insert(*iter);
		}

		return s;
	}

	/**
	gets the object closest to the given position, or INVALID_ID if the object would be greater than the threshold diatance
	@param pos position
	@param threshold threshold distance
	@return ID of an object
	*/
	template<class TYPE>
	OBJECT_ID getClosest(vec3 pos, float threshold) const
	{
		if(empty())
		{
			return INVALID_ID;
		}

		vector<Tuple> objects;

		// collect the distances from the position to each actor
		for(const_iterator iter = begin(); iter!=end(); ++iter)
		{
			if(instanceof(*(iter->second), TYPE))
				objects.push_back(ActorSet::getDistance(*iter, pos));
		}

		// Sort by distance
		sort(objects.begin(), objects.end(), tuple_less());

		// Return the closest one, or INVALID_ID if it is outside of the threshold distance
		return (objects.empty())
					? INVALID_ID
					: (objects[0].first > threshold)
						? INVALID_ID
						: objects[0].second;
	}

	/**
	gets the objects closest to the given position
	@param pos position
	@param N maximum number of objects to retrieve
	@param distanceThreshold objects farther than this distance are ignored
	@return ID of an object
	*/
	template<class TYPE>
	ActorSet getClosestSeveral(vec3 pos, size_t N, float distanceThreshold) const
	{
		vector<Tuple> objects;

		for(const_iterator i = begin(); i != end(); ++i)
		{
			if(instanceof(*(i->second), TYPE))
			{
				Tuple tuple = getDistance(*i, pos);

				if(tuple.first < distanceThreshold)
				{
					objects.push_back(tuple);
				}
			}
		}

		// Sort by distance
		sort(objects.begin(), objects.end(), tuple_less());

		// Return the closest few
		ActorSet s;
		for(size_t i=0; i<N && i<objects.size(); ++i)
			s.insert(*find(objects[i].second));

		return s;
	}

	/** Deletes zombie actors */
	void garbageCollection(void);

	/**
	request that an actor be spawned on the next update
	@param dataFile The datafile of the actor (specifies type)
	@param position position to place the actor
	*/
	void spawnNow(const string &dataFile, const vec3 &position, World *zone);

	/**
	request that an actor be spawned on the next update
	@param dataFile The datafile of the actor (specifies type)
	@param position position to place the actor
	*/
	void spawnNow(const PropertyBag &xml, const vec3 &position, World *zone);

	/**
	request that an actor be spawned on the next update
	@param dataFile The datafile of the actor (specifies type)
	@param position position to place the actor
	*/
	void spawn(const string &dataFile, const vec3 &position);

	/**
	request that an actor be spawned on the next update
	@param dataFile The datafile of the actor (specifies type)
	*/
	Actor& spawnNow(const PropertyBag &xml, World *zone);

private:
	typedef pair<float,OBJECT_ID> Tuple;

	struct tuple_greater
	{
		bool operator()(const Tuple& _Left, const Tuple& _Right) const
		{
			return(_Left.first > _Right.first);
		}
	};

	struct tuple_less
	{
		bool operator()(const Tuple& _Left, const Tuple& _Right) const
		{
			return(_Left.first < _Right.first);
		}
	};

	struct RequestedSpawn
	{
		string monsterDataFile;
		vec3 position;
	};

	vector<RequestedSpawn> requestedSpawns;

	/**
	Handle a requested spawn
	@param request The requested spawn
	*/
	void doSpawnRequest(const RequestedSpawn &request, World *zone);

	/**
	Gets the distance from a given position to a given actor
	@param a actor
	@param p position
	@return The distance
	*/
	static Tuple getDistance(pair<OBJECT_ID, Actor*> a, vec3 p);

	/**
	Deletes an actor if its of the templated type
	@param p pair
	*/
	template<class TYPE>
	static void deleteActor(pair<OBJECT_ID,Actor*> p)
	{
		if(instanceof(*(p.second), TYPE))
		{
			(p.second)->zombie = true;
		}
	}

	/**
	If the actor is within the viewing frustum, draw the actor
	@param frustum he viewing frustum
	@param p The actor in question
	*/
	static void drawActor(const Frustum *frustum, Actor *p);

	/**
	If the actor is within the viewing frustum, draw the actor
	@param frustum he viewing frustum
	@param p The actor in question
	*/
	static void drawActorTransparent(const Frustum *frustum, Actor *p);

	/**
	If the actor is within the viewing frustum, draw the actor
	@param frustum he viewing frustum
	@param p The actor in question
	*/
	static void drawActorDebugText(const Frustum *frustum, Actor *p);

	/**
	If the actor is within the viewing frustum, draw the actor to the depth buffer
	@param frustum he viewing frustum
	@param p The actor in question
	*/
	static void drawActorToDepthBuffer(Actor *p);
};

} // namespace Engine

#endif
