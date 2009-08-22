/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#include "stdafx.h"
#include "gl.h"
#include "opengl.h"
#include "SDLwindow.h"
#include "WaitScreen.h"
#include "frustum.h"
#include "ActorFactory.h"
#include "profile.h"
#include "player.h"
#include "ActorSet.h"


namespace Engine {





vec3 UnProject(int winx, int winy, float winz); // stdafx.cpp



/**
Simple function to quickly go from a pair to an actor.
Useful in lambda functions!
@param p pair
@return actor
*/
static Actor* toActor(pair<OBJECT_ID,Actor*> p)
{
	return p.second;
}



void ActorSet::destroy(void)
{
	clear();
}

Actor* ActorSet::createPtr(const string &type, World *zone)
{
	OBJECT_ID handle = create(type, zone);

	return getPtr(handle);
}

OBJECT_ID ActorSet::create(const string &type, World *zone)
{
	ActorFactory &factory = Engine::getActorFactory();

	ActorFactory::mapHandleToObject::iterator iter = factory.objects.find(  factory.create(type)  );

	if(iter==factory.objects.end())
	{
		FAIL("cannot create an object of the type: " + type);
		return INVALID_ID;
	}
	else
	{
		OBJECT_ID handle = iter->first;
		Actor *actor = iter->second;

		// add it to the set
		insert(make_pair(handle, actor));

		// set the initial zone
		actor->setZone(zone);

		// return the handle
		return handle;
	}
}

const Actor& ActorSet::get(OBJECT_ID id) const
{
	ASSERT(isMember(id), "Not a member");
	return(*find(id)->second);
}

Actor& ActorSet::get(OBJECT_ID id)
{
	ASSERT(isMember(id), "Not a member");
	Actor *actor = find(id)->second;
	return(*actor);
}

const Actor* ActorSet::getPtr(OBJECT_ID id) const
{
	if(isMember(id))
		return(find(id)->second);
	else
		return 0;
}

Actor* ActorSet::getPtr(OBJECT_ID id)
{
	if(isMember(id))
		return(find(id)->second);
	else
		return 0;
}

void ActorUpdate(pair<OBJECT_ID,Actor*> p, float deltaTime)
{
	if(!p.second->zombie)
	{
		p.second->update(deltaTime);
	}
}

void ActorCollisionDetection(pair<OBJECT_ID,Actor*> p, ActorSet * s)
{
	if(!p.second->zombie)
	{
		p.second->DoCollisionDetection(*s);
	}
}

void ActorCollisionResponse(pair<OBJECT_ID,Actor*> p)
{
	if(!p.second->zombie)
	{
		p.second->DoCollisionResponse();
	}
}

void ActorSet::update(float deltaTime, World *world)
{
	PROFILE

	for_each(begin(), end(), bind(ActorUpdate, _1, deltaTime));
	for_each(begin(), end(), bind(ActorCollisionDetection, _1, this));
	for_each(begin(), end(), bind(ActorCollisionResponse, _1));

	// Spawn creatures as requested
	for_each(requestedSpawns.begin(), requestedSpawns.end(), bind(&ActorSet::doSpawnRequest, this, _1, world));
	requestedSpawns.clear();

	garbageCollection(); // TODO: MAKE THIS A PERIODIC TASK
}

vector<Actor*> ActorSet::getByName(const string &name)
{
	vector<Actor*> actors;

	for(iterator iter = begin(); iter != end(); iter++)
	{
		Actor *a = iter->second;

		if(toLowerCase(a->getName()) == toLowerCase(name))
		{
			actors.push_back(a);
		}
	}

	return actors;
}

void ActorSet::garbageCollection(void)
{
	// delete zombie actors
	ActorSet::const_iterator iter = begin();
	ActorSet::const_iterator nextIter;

	while(iter != end())
	{
		// Get an iterator to the next element (or the end of the map)
		nextIter = iter;
		nextIter++;

		if(static_cast<Actor*>(iter->second)->zombie)
		{
			Engine::getActorFactory().remove(iter->first);
			erase(iter->first);
		}

		iter = nextIter;
	}
}

void ActorSet::drawActor(const Frustum *frustum, Actor *p)
{
	ASSERT(frustum!=0, "Null parameter! frustum was null");
	ASSERT(p!=0, "Null parameter! p was null");

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawObject();
}

void ActorSet::drawActorTransparent(const Frustum *frustum, Actor *p)
{
	ASSERT(frustum!=0, "Null parameter! frustum was null");
	ASSERT(p!=0, "Null parameter! p was null");

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawTransparentObject();
}

void ActorSet::drawActorDebugText(const Frustum *frustum, Actor *p)
{
	ASSERT(frustum!=0, "Null parameter! frustum was null");
	ASSERT(p!=0, "Null parameter! p was null");

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawObjectDebugData();
}

void ActorSet::drawActorToDepthBuffer(Actor *p)
{
	ASSERT(p!=0, "Null parameter! p was null");

	if(!p->zombie && p->doesCastShadows())
		p->drawObjectToDepthBuffer();
}

ActorSet ActorSet::isWithin(const Frustum &frustum) const
{
	ActorSet s;

	for(const_iterator i=begin(); i!=end(); ++i)
	{
		const Actor *p = i->second;

		if(!p->zombie && frustum.SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		{
			s.insert(*i);
		}
	}

	return s;
}

ActorSet ActorSet::isNotWithin(const Frustum &frustum) const
{
	ActorSet s;

	for(const_iterator i=begin(); i!=end(); ++i)
	{
		const Actor *p = i->second;

		if(!p->zombie && !frustum.SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		{
			s.insert(*i);
		}
	}

	return s;
}

ActorSet ActorSet::haveMoved(void) const
{
	ActorSet s;

	for(const_iterator i=begin(); i!=end(); ++i)
	{
		const Actor *p = i->second;

		if(!p->zombie && (p->hasAnimated || p->hasMoved))
		{
			s.insert(*i);
		}
	}

	return s;
}

void ActorSet::draw(const Frustum &frustum) const
{
	PROFILE
	for_each(begin(), end(), bind(&ActorSet::drawActor, &frustum, bind(&toActor, _1)));

	for_each(begin(), end(), bind(&ActorSet::drawActorTransparent, &frustum, bind(&toActor, _1)));

	if(g_Application.displayDebugData)
	{
		for_each(begin(), end(), bind(&ActorSet::drawActorDebugText, &frustum, bind(&toActor, _1)));
	}
}

void ActorSet::drawToDepthBuffer(void) const
{
	for_each(begin(), end(), bind(&ActorSet::drawActorToDepthBuffer, bind(&toActor, _1)));
}

void ActorSet::addAll(ActorSet &s)
{
	for(ActorSet::iterator iter = s.begin(); iter != s.end(); ++iter)
	{
		insert(*iter);
	}
}

void ActorSet::spawn(const string &dataFile, const vec3 &position)
{
	RequestedSpawn request;

	request.monsterDataFile = dataFile;
	request.position = position;

	requestedSpawns.push_back(request);
}

void ActorSet::doSpawnRequest(const RequestedSpawn &request, World *zone)
{
	spawnNow(request.monsterDataFile, request.position, zone);
}

void ActorSet::spawnNow(const string &dataFile, const vec3 &position, World *zone)
{
	PropertyBag xml;
	xml.loadFromFile(dataFile);
	spawnNow(xml, position, zone);
}

Actor& ActorSet::spawnNow(const PropertyBag &data, World *zone)
{
	OBJECT_ID id = create(data.getString("type"), zone);
	Actor &object = get(id);
	object.load(data);

	return object;
}

void ActorSet::spawnNow(const PropertyBag &xml, const vec3 &position, World *zone)
{
	spawnNow(xml, zone).Place(position);
}

void ActorSet::load(const PropertyBag &xml, World *world)
{
	ASSERT(world!=0, "world was null");

	TRACE("Loading ActorSet...");

	for(size_t i=0, numObjects=xml.getNumInstances("object"); i<numObjects; ++i)
	{
		PropertyBag ThisObjBag;

		xml.get("object", ThisObjBag, i);

		spawnNow(ThisObjBag, world);
	}

	// Player data is saved separately
	deleteActors<Player>();

	TRACE("...finished (Loading ActorSet)");
}

PropertyBag ActorSet::save(void) const
{
	PropertyBag xml;

	for(const_iterator i = begin(); i != end(); ++i)
	{
		Actor &a = *(i->second);

		// Do not save Creatures.  We are saving spawns instead and the player data is separate
		if(!instanceof(a, Creature))
		{
			xml.add("object", a.save());
		}
	}

	return xml;
}

ActorSet::Tuple ActorSet::getDistance(pair<OBJECT_ID, Actor*> a, vec3 p)
{
	return make_pair(vec3((a.second)->getPos().x - p.x, 0, (a.second)->getPos().z - p.z).getMagnitude(), (a.second)->m_ID);
}

void ActorSet::removeObjectNow(OBJECT_ID id)
{
	ASSERT(isMember(id), "The object is not a member of this set");
	erase(find(id));
}

void ActorSet::moveObject(ActorSet &dest, OBJECT_ID id)
{
	ASSERT(isMember(id), "The object is not a member of this set");
	ASSERT(!dest.isMember(id), "The object is already a member of the other set");

	// Add it to the other set
	dest.insert(  make_pair(id, toActor(*find(id)))  );

	// Remove it from this set
	erase(find(id));
}

bool ActorSet::isMember(OBJECT_ID id) const
{
	return (id!=INVALID_ID) && (find(id)!=end());
}

bool ActorSet::query(const string &name, OBJECT_ID &out) const
{
	for(const_iterator iter=begin(); iter != end(); ++iter)
	{
		if(name == static_cast<Actor*>(iter->second)->getName())
		{
			out = iter->first;
			return true;
		}
	}

	out = INVALID_ID;
	return false;
}

ActorSet ActorSet::exclude(OBJECT_ID id) const
{
	ActorSet s = *this;

	ActorSet::iterator iter = s.find(id);

	if(iter != s.end()) // if present
	{
		s.erase(iter);
	}

	return s;
}

} // namespace Engine
