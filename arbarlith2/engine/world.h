/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright � 2003-2007 Game Creation Society
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
#include "Singleton.h"
#include "PropBag.h"

#include "ActorSet.h"
#include "ActorFactory.h"
#include "messagerouter.h"
#include "particle.h"
#include "LightManager.h"
#include "ShadowManager.h"
#include "MusicEngine.h"
#include "Map.h"
#include "fog.h"


#define MAX_PLAYERS (4)


namespace Engine {

// class prototype
class EditorToolBar;
class Player;

/** handles the game world */
class World : public Singleton<World>
{
public:
	/** Destructor */
	~World(void);

	/** Default constructor */
	World(void);

	/** Clears the zone to a just-constructed state */
	void clear(void);

	/** Cleanly destroys and clears the zone */
	void destroy(void);

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Gets the default file name to save or load the world from
	@param name The name of the world
	*/
	static _tstring getDefaultSaveFileName(const _tstring &name)
	{
		return _T("data/zones/") + name + _T(".xml");
	}

	/**
	Saves the world state to the PropertyBag data structure
	@return world state
	*/
	PropertyBag save(void) const;

	/**
	Loads the world state
	@param xml data source
	*/
	void load(const PropertyBag &xml);

	/**
	Saves the world to file
	@param fileName Name of the file to write
	*/
	inline void saveToFile(const _tstring &fileName) const
	{
		save().saveToFile(fileName);
	}

	/** Saves the world to file */
	inline void saveToFile(void) const
	{
		saveToFile(getDefaultSaveFileName(getName()));
	}

	/**
	Loads the world from file
	@param fileName XML source
	@param xml data source
	*/
	void loadFromFile(const _tstring &fileName);

	/** Loads the world from file */
	inline void loadFromFile(void)
	{
		loadFromFile(getDefaultSaveFileName(getName()));
	}

	/**
	Retrieves the name of the realm
	@return Name of the realm
	*/
	inline const _tstring& getName(void) const
	{
		return name;
	}

	/**
	Sets the name of the realm
	@param name New name of the realm
	*/
	inline void setName(const _tstring &name)
	{
		this->name = name;
	}

	/**
	Gets the object database
	@return object database
	*/
	inline ActorSet& getObjects(void)
	{
		return objects;
	}

	/**
	Gets the object database
	@return object database
	*/
	inline const ActorSet& getObjects(void) const
	{
		return objects;
	}

	/**
	Gets the tile-based representation of the game world
	@return a reference to the map object
	*/
	inline Map& getMap(void)
	{
		return map;
	}

	/**
	Gets the tile-based representation of the game world
	@return a reference to the map object
	*/
	inline const Map& getMap(void) const
	{
		return map;
	}

	/** Render the current zone */
	void draw(void) const;

	/**
	Update the World
	@param deltaTime Time elapsed since the last update
	*/
	void update(float deltaTime);

	/**
	Update particles and delete stale particle systems
	@param deltaTime Time elapsed since the last update
	*/
	void updateParticles(float deltaTime);

	/**
	Spawns a particle system
	@param strFile Source file of the particle system
	@param vPos The starting position of the particle system
	@return Handle to the particle system
	*/
	size_t SpawnPfx(_tstring strFile, const vec3 &vPos);

	/**
	Gets the light manager
	@return the light manager
	*/
	inline LightManager& getLightManager(void)
	{
		return lightManager;
	}

	/**
	Gets the light manager
	@return the light manager
	*/
	inline const LightManager& getLightManager(void) const
	{
		return lightManager;
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
	Reloads all the players
	@param newGame The "new game" file
	*/
	void reloadPlayers(const PropertyBag &newGame);

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

	/**
	Gets the current time in the game, in milliseconds
	@return milliseconds
	*/
	inline double getClockTicks(void) const
	{
		return clockTicks;
	}

	/**
	Gets a particle system, given its handle
	@param handle The handle to the particle system
	@return particle system
	*/
	inline ParticleSystem& getParticleSystem(size_t handle)
	{
		ASSERT(handle<particles.size(), _T("handle is invalid!"));
		ASSERT(particles[handle]!=0, _T("particles[handle] is null!"));
		return *particles[handle];
	}

	/**
	Gets a particle system, given its handle
	@param handle The handle to the particle system
	@return particle system
	*/
	inline const ParticleSystem& getParticleSystem(size_t handle) const
	{
		ASSERT(handle<particles.size(), _T("handle is invalid!"));
		ASSERT(particles[handle]!=0, _T("particles[handle] is null!"));
		return *particles[handle];
	}

private:
	/**
	Loads and processes XML data
	@param Bag XML-Source
	@return true if succesful, false otherwise
	*/
	bool LoadData(const PropertyBag &Bag);

	/**
	Draws the scene without shadows
	@param useLights When true, the shadowless scene is rendered with lighting.
	Otherwise, only full ambient light is used.
	*/
	void drawShadowlessScene(bool useLights) const;

	/** Draws the scene for the editor */
	void drawEditorScene(void) const;

	/** Draws the particles in the scene */
	void drawParticles(void) const;

	/** Draws the scene with shadows */
	void drawScene(void) const;

	/**
	Draws the shadow receiving geometry of the scene.
	Assumes that appropriate states are already setup for the operation.
	*/
	void drawShadowReceivers(void) const;

	/**
	Update the shadows
	@param deltaTime Time elapsed since the last update
	*/
	void updateShadows(float deltaTime);

	/** Periodically calculates and caches the average player position */
	inline void recalculateAveragePlayerPosition(void)
	{
		averagePlayerPosition = findAveragePlayerPosition();
	}

	/**
	Finds the mean position of all the players in the game
	@return aveerage position of all the players
	*/
	vec3 findAveragePlayerPosition(void) const;

public:
	/** Music to play in the realm */
	MusicEngine music;

	/** Game Object Message Router */
	MessageRouter router;

	/**
	Name of the World
	@todo Engine::World::name should be made private
	*/
	_tstring name;

private:
	/** All lights in the World */
	LightManager lightManager;

	/** All shadows in the World */
	ShadowManager shadowManager;

	/** Manages fog settings */
	Fog fog;

	/** Set of objects that reside within this World */
	ActorSet objects;

	/** Brick and Mortar walls of the World */
	Map map;

	/** Storage for all possible players */
	Player *player[MAX_PLAYERS];

	/** The number of players less than the maximum that are actually in use */
	size_t NumOfPlayers;

	/** Periodically calculates and caches the average player position */
	vec3 averagePlayerPosition;

	/** Milliseconds since this game began */
	double clockTicks;

	/** Collection of particle systems in the world */
	vector<ParticleSystem*> particles;
};

} // namespace Engine

#endif
