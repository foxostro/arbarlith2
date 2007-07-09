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

#include "stdafx.h"
#include "gl.h"
#include "OpenGL.h"

#include "WaitScreen.h"
#include "frustum.h"
#include "ActorSet.h"
#include "ActorFactory.h"
#include "priority_queue.h"
#include "profile.h"

#include "Player.h"


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

Actor* ActorSet::createPtr(const _tstring &type, Zone *zone)
{
	OBJECT_ID handle = create(type, zone);

	return getPtr(handle);
}

OBJECT_ID ActorSet::create(const _tstring &type, Zone *zone)
{
	ActorFactory &factory = Engine::getActorFactory();

	ActorFactory::mapHandleToObject::iterator iter = factory.objects.find(  factory.create(type)  );

	if(iter==factory.objects.end())
	{
		FAIL(_T("cannot create an object of the type: ") + type);
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
	ASSERT(isMember(id), _T("Not a member"));
	return(*find(id)->second);
}

Actor& ActorSet::get(OBJECT_ID id)
{
	ASSERT(isMember(id), _T("Not a member"));
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

void ActorSet::update(float deltaTime, Zone *zone)
{
	PROFILE

	for_each(begin(), end(), bind(ActorUpdate, _1, deltaTime));
	for_each(begin(), end(), bind(ActorCollisionDetection, _1, this));
	for_each(begin(), end(), bind(ActorCollisionResponse, _1));



	// Spawn creatures as requested
	for(size_t i=0; i<requestedSpawns.size(); ++i)
	{
		RequestedSpawn &request = requestedSpawns[i];
		spawnNow(request.monsterDataFile, request.position, zone);
	}
	requestedSpawns.clear();



	garbageCollection(); //   TODO: MAKE THIS A PERIODIC TASK
}

vector<Actor*> ActorSet::getByName(const _tstring &name)
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
	ASSERT(frustum!=0, _T("Null parameter! frustum was null"));
	ASSERT(p!=0, _T("Null parameter! p was null"));

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawObject();
}

void ActorSet::drawActorTransparent(const Frustum *frustum, Actor *p)
{
	ASSERT(frustum!=0, _T("Null parameter! frustum was null"));
	ASSERT(p!=0, _T("Null parameter! p was null"));

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawTransparentObject();
}

void ActorSet::drawActorDebugText(const Frustum *frustum, Actor *p)
{
	ASSERT(frustum!=0, _T("Null parameter! frustum was null"));
	ASSERT(p!=0, _T("Null parameter! p was null"));

	if(!p->zombie && frustum->SphereInFrustum2(p->getPos(), p->getSphereRadius()*2))
		p->drawObjectDebugData();
}

void ActorSet::drawActorToDepthBuffer(Actor *p)
{
	ASSERT(p!=0, _T("Null parameter! p was null"));

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

void ActorSet::spawn(const _tstring &dataFile, const vec3 &position)
{
	RequestedSpawn request;

	request.monsterDataFile = dataFile;
	request.position = position;

	requestedSpawns.push_back(request);
}

void ActorSet::spawnNow(const _tstring &dataFile, const vec3 &position, Zone *zone)
{
	CPropBag xml;
	xml.Load(dataFile);
	spawnNow(xml, position, zone);
}

Actor& ActorSet::spawnNow(CPropBag &xml, Zone *zone)
{
	_tstring fileName, rtti;

	if(xml.Get(_T("file"), fileName))
	{
		// The type is defined in an external file
		CPropBag external;
		external.Load(fileName);
		external.Get(_T("type"), rtti);
	}
	else
	{
		// No exernal data file
		xml.Get(_T("type"), rtti);
	}

	// Create the object inside the game world
	OBJECT_ID id = create(rtti, zone);

	// Load the object from XML
	Actor &object = get(id);

	// Load the object
	object.LoadXml(xml);

	return object;
}

void ActorSet::spawnNow(CPropBag &xml, const vec3 &position, Zone *zone)
{
	spawnNow(xml, zone).Place(position);
}

void ActorSet::load(CPropBag &xml, Zone *zone)
{
	ASSERT(zone!=0, _T("zone was null"));

	_tstring rtti;

	TRACE(_T("Loading ActorSet"));
	g_WaitScreen.Render();

	// Get the number ofobjects coming up
	int numObjects = xml.GetNumInstances(_T("object"));

	// Load the objects
	for(int i=0; i<numObjects; ++i)
	{
		CPropBag ThisObjBag;

		xml.Get(_T("object"), ThisObjBag, i);

		spawnNow(ThisObjBag, zone);
	}

	// Player data is saved separately
	deleteActors<Player>();

	TRACE(_T("All Objects Loaded"));
	g_WaitScreen.Render();
}

CPropBag ActorSet::save(void) const
{
	CPropBag xml;

	for(const_iterator i = begin(); i != end(); ++i)
	{
		Actor &a = *(i->second);

		// Do not save Creatures.  We are saving spawns instead and the player data is separate
		if(!instanceof(a, Creature))
		{
			xml.Add(_T("object"), a.save());
		}
	}

	return xml;
}

pair<float,OBJECT_ID> ActorSet::rayIntersectActor(OBJECT_ID caster, const vec3 &start, const vec3 &dir, Actor *p)
{
	ASSERT(p!=0, _T("Null parameter! p was null"));
	return make_pair(
						(p->m_ID!=caster && p->rayIntersect(start, dir)) ? vec3(p->getPos() - start).getMagnitude() : 9999.f,
						p->m_ID
					);
}

OBJECT_ID ActorSet::rayCast(OBJECT_ID caster, const vec3 &start, const vec3 &dir)
{
	vector< pair<float,OBJECT_ID> > intersections;

	transform(begin(), end(), back_inserter(intersections), bind(&ActorSet::rayIntersectActor, caster, start, dir, bind(&toActor, _1)));

	if(intersections.empty())
	{
		return INVALID_ID;
	}
	else
	{
		sort(intersections.begin(), intersections.end(), tuple_less());

		return(intersections[0].second);
	}
}

ActorSet::Tuple ActorSet::getDistance(pair<OBJECT_ID, Actor*> a, vec3 p)
{
	return make_pair(vec3((a.second)->getPos().x - p.x, 0, (a.second)->getPos().z - p.z).getMagnitude(), (a.second)->m_ID);
}

OBJECT_ID ActorSet::mousePick(void)
{
	return pick(g_Input.MouseX, g_Input.MouseY);
}

OBJECT_ID ActorSet::pick(int mx, int my)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	g_Camera.setCamera();



	OBJECT_ID id = INVALID_ID;

	// Create the mouse ray
	vec3 mouse1 = UnProject(mx, my, 0.0f);
	vec3 mouse2 = UnProject(mx, my, 10.0f);
	vec3 delta = mouse2 - mouse1;




	id = rayCast(INVALID_ID, mouse1, delta);




	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	return id;
}

void ActorSet::removeObjectNow(OBJECT_ID id)
{
	ASSERT(isMember(id), _T("The object is not a member of this set"));
	erase(find(id));
}

void ActorSet::moveObject(ActorSet &dest, OBJECT_ID id)
{
	ASSERT(isMember(id), _T("The object is not a member of this set"));
	ASSERT(!dest.isMember(id), _T("The object is already a member of the other set"));

	// Add it to the other set
	dest.insert(  make_pair(id, toActor(*find(id)))  );

	// Remove it from this set
	erase(find(id));
}

bool ActorSet::isMember(OBJECT_ID id) const
{
	return (id!=INVALID_ID) && (find(id)!=end());
}

bool ActorSet::query(const _tstring &name, OBJECT_ID &out) const
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

}; // namespace
