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

#ifndef _WORLD_H_
#define _WORLD_H_

#include "vec4.h"
#include "ActorFactory.h"
#include "PropBag.h"
#include "Singleton.h"

#define MAX_PLAYERS 4

namespace Engine {

// class prototype
class Player;
class Zone;

/** handles the game world by tracking the game Zones */
class World : public Singleton<World>
{
public:
	/**
	Constructs the World and loads the game from the given filename
	@param fileName
	*/
	World(const _tstring &fileName);

	/** Destructor */
	virtual ~World(void);

	/** Cleanly destroys the world */
	void destroy(void);

	/** Resets all data fields to a just-constructed state */
	void clear(void);

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Save to XML
	@param bag XML source
	@return true if the calling child class should save fully, false
	if the calling child class should save just enough for editor mode
	*/
	virtual bool SaveXml(CPropBag &bag);

	/**
	Load from XML
	@param bag XML source
	@return true if successful
	*/
	virtual bool LoadXml(CPropBag &bag);

	/**
	Save to XML
	@param fileName XML source
	@return true if success, false if failure
	*/
	virtual bool SaveXml(const _tstring &fileName);

	/**
	Load from XML
	@param fileName XML source
	@return true if successful
	*/
	virtual bool LoadXml(const _tstring &fileName);

	/**
	Updates the tak every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime);

	/** Return the number of realms in the game world */
	size_t getNumRealms(void) const
	{
		return m_Realms.size();
	}

	/**
	Searches for a realm of the given name
	@param realmName The name of the realm
	@return The index for the realm
	*/
	size_t getRealmIdx(const _tstring &realmName);

	/**
	Searches for a realm of the given name
	@param realmName The name of the realm
	@return The realm that has been found
	*/
	Zone& getZone(const _tstring &realmName);

	/**
	Searches for a realm of the given name
	@param realmName The name of the realm
	@return The realm that has been found
	*/
	Zone* getZonePtr(const _tstring &realmName);

	/**
	Retrieve a realm by its index
	@param idx The realm index
	@return The realm
	*/
	Zone& getZone(size_t idx);

	/**
	Retrieve a realm by its index
	@param idx The realm index
	@return The realm
	*/
	Zone* getZonePtr(size_t idx);

	/**
	Gets the current time in the game, in milliseconds
	@return milliseconds
	*/
	inline double getClockTicks(void) const
	{
		return m_ClockTicks;
	}

	/**
	Gets the name of the starting Zone
	@return milliseconds
	*/
	inline const _tstring& getStartingZone(void) const
	{
		return startingRealm;
	}

	/**
	Gets the number of players less than the maximum that are actually in use
	@return number of players
	*/
	inline size_t getNumOfPlayers(void) const
	{
		return NumOfPlayers;
	}

	/**
	Gets the player
	@param playerNum Names the player to get by number (starts counting at zero)
	@return A reference to the player
	*/
	const Player& getPlayer(size_t playerNum ) const;

	/**
	Gets the player
	@param playerNum Names the player to get by number (starts counting at zero)
	@return A pointer to the player
	*/
	const Player* getPlayerPtr(size_t playerNum) const;

	/**
	Gets the player
	@param playerNum Names the player to get by number (starts counting at zero)
	@return A reference to the player
	*/
	Player& getPlayer(size_t playerNum = 0);

	/**
	Gets the player
	@param playerNum Names the player to get by number (starts counting at zero)
	@return A pointer to the player
	*/
	Player* getPlayerPtr(size_t playerNum);

	/**
	Reloads the player
	@param newGame The "new game" file
	*/
	void reloadPlayer(CPropBag &newGame);

	/** Harmonizes the camera with the current player positions */
	void updateCamera(void);

	/**
	Gets the most recently calculated mean player position
	@return mean player position
	*/
	inline const vec3& getAveragePlayerPosition(void) const
	{
		return averagePlayerPosition;
	}

private:
	/** The name of the starting Zone */
	_tstring startingRealm;

	/** Milliseconds since this game began */
	double m_ClockTicks;

	/** This is a list of the realms that are living at the current moment */
	vector<Zone*> m_Realms;

	/** Storage for all possible players */
	Player *player[MAX_PLAYERS];

	/** The number of players less than the maximum that are actually in use */
	size_t NumOfPlayers;

	/** Periodically calculates and caches the average player position */
	vec3 averagePlayerPosition;

	/**
	Attempts to cache a Zone stored on disc
	@param The file name of the Zone to attempt to cache
	@return a pointer to the cached Zone, or 0
	*/
	Zone* cacheZone(const _tstring &fileName);

	/** Periodically calculates and caches the average player position */
	void recalculateAveragePlayerPosition(void)
	{
		averagePlayerPosition = findAveragePlayerPosition();
	}

	/**
	Finds the mean position of all the players in the game
	@return aveerage position of all the players
	*/
	vec3 findAveragePlayerPosition(void) const;
};

} // namespace Engine

#endif
