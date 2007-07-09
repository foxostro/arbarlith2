/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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
#include "profile.h"

#include "Zone.h"
#include "World.h"
#include "WaitScreen.h"
#include "SearchFile.h"
#include "Player.h"

namespace Engine {

World::World(const _tstring &fileName)
{
	clear();

	g_WaitScreen.Render();
	LoadXml(fileName);
	TRACE(_T("Finished loading the game from file: ") + fileName);
	g_WaitScreen.Render();
}

World::~World()
{
	destroy();
}

bool World::SaveXml(const _tstring &fileName)
{
	CPropBag bag;
	SaveXml(bag);

	TRACE(_T("Writing the game to file"));
	g_WaitScreen.Render();

	bag.saveToFile(fileName);

	TRACE(_tstring(_T("World has been saved to")) + fileName);
	g_WaitScreen.Render();
	return true;
}

bool World::SaveXml(CPropBag &Bag)
{
	CPropBag AllRealmsBag, clientBag;

	// Do the waitscreen every so often
	TRACE(_T("Saving the game"));
	g_WaitScreen.Render();

	// Save the game clock
	Bag.Add(_T("ticks"), m_ClockTicks);

	// Save the realms
	TRACE(_T("Beginning to save game world realm data"));
	for(vector<Zone*>::iterator iter=m_Realms.begin(); iter!=m_Realms.end(); ++iter)
	{
		Zone *realm = (*iter);

		if(realm)
		{
			TRACE(_tstring(_T("Saving realm: ")) + realm->getName());

			// Save the realm
			CPropBag RealmBag;
			realm->SaveXml(RealmBag);
			AllRealmsBag.Add(_T("realm"), RealmBag);
		}
	}

	Bag.Add(_T("startingRealm"), getPlayer().getZone().getName());

	TRACE(_T("Writing all realms to disc. This may take a moment..."));
	Bag.Add(_T("allrealms"), AllRealmsBag);

	TRACE(_T("Saving the player"));
	Bag.Add(_T("player"), getPlayer().save());

	// Return with success
	TRACE(_T("Save completed successfully"));
	return true;
}

size_t World::getRealmIdx(const _tstring &realmName)
{
	const _tstring name = toLowerCase(realmName);

	for(size_t i=0; i<m_Realms.size(); ++i)
	{
		Zone *realm = m_Realms[i];
		ASSERT(realm!=0, _T("zone was NULL"));

		if(toLowerCase(realm->getName()) == name)
		{
			return i;
		}
	}

	FAIL(_T("Realm does not exist: ") + realmName);
	return 0;
}

Zone& World::getZone(const _tstring &zoneName)
{
	Zone *zone = getZonePtr(zoneName);

	if(zone == 0)
		FAIL(_tstring(_T("Zone does not exist: ")) + zoneName);

	return(*zone);
}

Zone* World::getZonePtr(const _tstring &zoneName)
{
	const _tstring name = toLowerCase(zoneName);

	for(size_t i=0; i<m_Realms.size(); ++i)
	{
		Zone *zone = m_Realms[i];
		ASSERT(zone!=0, _T("zone was NULL"));

		if(toLowerCase(zone->getName()) == toLowerCase(name))
		{
			// Have the realm load itself now, if necessary
			return getZonePtr(i);
		}
	}

	return 0;
}

Zone* World::getZonePtr(size_t idx)
{
	ASSERT(idx < getNumRealms(), _T("Invalid realm index"));

	Zone *realm = m_Realms[idx];

	ASSERT(realm!=0, _T("realm was NULL"));

	// Determine whether the realm has loaded itself from XML yet
	if(realm->isLoaded() == false)
	{
		// No?

		TRACE(_tstring(_T("Loading realm: ")) + realm->getName());
		g_WaitScreen.Render();
		realm->LoadXml(realm->xmlCache);
	}

	return realm;
}

Zone& World::getZone(size_t idx)
{
	return *getZonePtr(idx);
}

Zone* World::cacheZone(const _tstring &fileName)
{
	TRACE(_T("Started caching a Zone from disc"));
	g_WaitScreen.Render();

	Zone* zone = new Zone;

	ASSERT(zone != 0, _T("Failed to allocate zone"));

	if(!zone->xmlCache.Load(fileName))
	{
		ERR(_T("Failed to cache zone: failed to load file \"") + fileName + _T("\""));
		return 0;
	}

	if(!zone->xmlCache.Get(_T("name"), zone->name))
	{
		ERR(_T("Failed to cache zone: failed to retrieve name from XML"));
		return 0;
	}

	m_Realms.push_back(zone);


	g_WaitScreen.Render();
	TRACE(_tstring(_T("Finished caching zone: ")) + zone->getName());
	return zone;
}

bool World::LoadXml(const _tstring &fileName)
{
	CPropBag bag;
	bool result=false;

#ifdef ENABLE_PROFILER
	Profiler::ProfileBegin("Load World");
#endif

	g_WaitScreen.Render();

	bag.Load(fileName);
	result = LoadXml(bag);

#ifdef ENABLE_PROFILER
	Profiler::ProfileEnd("Load World");
	Profiler::ProfileDumpOutputToBuffer();
	TRACE(_T("Outputing Timing Data:\n") + Profiler::ProfileGetBuffer());
#endif

	return result;
}

bool World::LoadXml(CPropBag &xml)
{
	TRACE(_T("Loading game world..."));
	g_WaitScreen.Render();

	destroy();

	// Load the game time
	xml.Get(_T("ticks"), m_ClockTicks);

	// Load the starting realm
	xml.Get(_T("startingRealm"), startingRealm);
	Zone *defaultZone = cacheZone(  _T("data/zones/") + startingRealm + _T(".xml")  );
	ASSERT(defaultZone!=0, _T("defaultZone is null: ") + startingRealm);
	defaultZone->LoadXml(defaultZone->xmlCache);

	// Load the Player
	reloadPlayer(xml);

	TRACE(_T("...finished (Loading game world)"));
	return true;
}

void World::destroy(void)
{
	TRACE(_T("Destroying the current world..."));

	g_WaitScreen.Render();

	for_each(m_Realms.begin(), m_Realms.end(), bind(delete_arg(), _1));
	clear();

	TRACE(_T("...finished"));
}

void World::clear(void)
{
	m_Realms.clear();
	m_ClockTicks=0.0f;

	for(int i=0; i<MAX_PLAYERS; ++i)
	{
		player[i]=0;
	}

	NumOfPlayers = 0;
}

void World::release(void)
{
	for_each(m_Realms.begin(), m_Realms.end(), bind(&Engine::Zone::release, _1));
}

void World::reaquire(void)
{
	for_each(m_Realms.begin(), m_Realms.end(), bind(&Engine::Zone::reaquire, _1));
}

void World::update(float deltaTime)
{
	m_ClockTicks += double(deltaTime); // Update the game clock

	recalculateAveragePlayerPosition();

	Player &player = getPlayer(0);
	Zone &zone = player.getZone();
	zone.update(deltaTime);
}

const Player& World::getPlayer(size_t playerNum) const
{
	const Player *player = getPlayerPtr(playerNum);

	ASSERT(player!=0, _T("player[") + itoa((int)playerNum) + _T("] was null"));

	return(*player);
}

Player& World::getPlayer(size_t playerNum)
{
	ASSERT(playerNum < MAX_PLAYERS, _T("invalid player number"));

	Player *player = getPlayerPtr(playerNum);

	ASSERT(player!=0, _T("player[") + itoa((int)playerNum) + _T("] was null"));

	return(*player);
}

const Player* World::getPlayerPtr(size_t playerNum) const
{
	ASSERT(playerNum < MAX_PLAYERS, _T("invalid player number"));
	return player[playerNum];
}

Player* World::getPlayerPtr(size_t playerNum)
{
	return player[playerNum];
}

void World::reloadPlayer(CPropBag &newGame)
{
	TRACE(_T("Reloading the player..."));

	CPropBag playerBag;

	if(newGame.Get(_T("player"), playerBag))
	{
		_tstring file, type;

		if(playerBag.getSym(file))
		{
			// The type is defined in an external file
			CPropBag external;
			external.Load(file);
			external.getSym(type);
		}
		else
		{
			// No exernal data file
			playerBag.getSym(type);
		}

		int numOfJoysticks = SDL_NumJoysticks();

#if _PLAYER_ONE_HAS_NO_JOYSTICK_
		TRACE(_T("_PLAYER_ONE_HAS_NO_JOYSTICK_ is defined, so player 1 will always be on the keyboard"));
		NumOfPlayers = (numOfJoysticks==0) ? 1 : min(numOfJoysticks + 1, MAX_PLAYERS);
#else
		TRACE(_T("_PLAYER_ONE_HAS_NO_JOYSTICK_ is not defined, so player 1 will always be on the keyboard and joystick #1"));
		NumOfPlayers = (numOfJoysticks==0) ? 1 : min(numOfJoysticks, MAX_PLAYERS);
#endif

		// Create the players
		Zone *defaultZone = &getZone(startingRealm);
		for(int i = 0; (size_t)i < NumOfPlayers; ++i)
		{
			Actor *p = defaultZone->getObjects().createPtr(type, defaultZone);

			player[i] = dynamic_cast<Player*>(p);

			ASSERT(player[i]!=0, _T("Failed to construct player #") + itoa(i));

			player[i]->LoadXml(playerBag);
			TRACE(_T("Loaded player #") + itoa(i));

			player[i]->setPlayerNumber(i);
			TRACE(_T("Actually set player number to ") + itoa(i));

			if(NumOfPlayers>1)
			{
				float angle = 2.0f * ((float)i/NumOfPlayers) * (float)M_PI;
				vec3 offset = vec3(cosf(angle), 0, sinf(angle)) * 1.2f;
				player[i]->Place(player[i]->getPos() + offset);

				TRACE(_T("Placed player #") + itoa(i) + _T("in the game world."));
			}
		}
	}

	TRACE(_T("...finished (Reloading the player)"));
}

vec3 World::findAveragePlayerPosition(void) const
{
	vec3 averagePlayerPosition;
	const size_t numOfPlayers = getNumOfPlayers();

	ASSERT(numOfPlayers!=0, _T("Number of players is zero, and it shouldn't be."));
	ASSERT(numOfPlayers < MAX_PLAYERS, _T("Number of players (") + itoa((int)numOfPlayers) + _T(") is too large!  The maximum is ") + itoa((int)MAX_PLAYERS));

	for(size_t i=0; i<numOfPlayers; ++i)
	{
		const Player &player = getPlayer(i);
		averagePlayerPosition = averagePlayerPosition + player.getPos();
	}
	averagePlayerPosition = averagePlayerPosition * (1.0f/numOfPlayers);

	return averagePlayerPosition;
}

void World::updateCamera(void)
{
	const float minCameraDistance = 6.0f;
	float cameraDistance = minCameraDistance;
	const vec3 averagePlayerPosition = getAveragePlayerPosition();
	const size_t numOfPlayers = g_World.getNumOfPlayers();

	if(numOfPlayers>1)
	{
		float maxPlayerDistance=0;

		// Find the position of the player that is farthest away from the average position
		for(size_t i=0; i<numOfPlayers; ++i)
		{
			const Player &player = getPlayer(i);
			maxPlayerDistance = max(maxPlayerDistance, player.getDistanceFromAveragePosition());
		}

		cameraDistance = max(minCameraDistance, maxPlayerDistance + minCameraDistance);
	}

	g_Camera.lookAt(averagePlayerPosition + vec3(cameraDistance,cameraDistance,cameraDistance), averagePlayerPosition, vec3(0,1,0));
}

}; // namespace
