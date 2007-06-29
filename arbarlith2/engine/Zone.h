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

#ifndef _ZONE_H_
#define _ZONE_H_

#include "ActorSet.h"
#include "ActorFactory.h"

#include "messagerouter.h"
#include "particle.h"
#include "LightManager.h"
#include "ShadowManager.h"
#include "MusicEngine.h"
#include "Map.h"
#include "fog.h"
#include "World.h"

namespace Engine { 

// class prototype
class EditorToolBar;

/** A contained area of the game world */
class Zone
{
	friend World;
	friend EditorToolBar; // FIXME: remove this statement! needed so that the toolbar can edit the Zone name... correct this

public:
	/** Default constructor */
	Zone(void);

	/**
	Constructs the Zone from data loaded from file
	@param fileName Zone file name
	*/
	Zone(const _tstring &fileName);

	/** Destructor */
	virtual ~Zone(void);

	/** Clears the zone to a just-constructed state */
	virtual void clear(void);

	/** Cleanly destroys and clears the zone */
	virtual void destroy(void);

	/** Release assets */
	virtual void release(void);

	/** Reaquire assets */
	virtual void reaquire(void);

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
	virtual void SaveXml(const _tstring &fileName)
	{
		CPropBag bag;
		SaveXml(bag);
		bag.saveToFile(fileName);
	}

	/**
	Load from XML
	@param fileName XML source
	@return true if successful
	*/
	virtual bool LoadXml(const _tstring &fileName)
	{
		CPropBag bag;
		bag.Load(fileName);
		return LoadXml(bag);
	}

	/**
	Retrieves the name of the realm
	@return Name of the realm
	*/
	const _tstring& getName(void) const
	{
		return name;
	}

	/**
	Determines whether the realm has loaded data from XML yet or not
	@return true if the realm has loaded data from XML, false otherwise
	*/
	bool isLoaded(void) const
	{
		return loaded;
	}

	/**
	Gets the object database
	@return object database
	*/
	ActorSet& getObjects(void)
	{
		return objects;
	}

	/**
	Gets the object database
	@return object database
	*/
	const ActorSet& getObjects(void) const
	{
		return objects;
	}

	/**
	Gets the tile-based representation of the game world
	@return a reference to the map object
	*/
	Map& getMap(void)
	{
		return map;
	}

	/**
	Gets the tile-based representation of the game world
	@return a reference to the map object
	*/
	const Map& getMap(void) const
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
	LightManager& getLightManager(void)
	{
		return lightManager;
	}

private:
	/**
	Loads and processes XML data
	@param Bag XML-Source
	@return true if succesful, false otherwise
	*/
	bool LoadData(CPropBag &Bag);

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

public:
	/** Music to play in the realm */
	MusicEngine music;

	/** Game Object Message Router */
	MessageRouter router;

	/** List of particle systems in the world */
	PARTICLE_SYSTEMS particles;
	
private:
	/** All lights in the Zone */
	LightManager lightManager;

	/** All shadows in the Zone */
	ShadowManager shadowManager;

	/** Manages fog settings */
	Fog fog;

	/**
	Flags whether or not the realm has been loaded from XML yet.
	If the cache exists, this determines whether the realm has
	been created from it or not.
	*/
	bool loaded;

	/** Caches the data for the Zone until the Zone is loaded, as indicated by the 'Engine::Zone::loaded' property */
	CPropBag xmlCache;

	/** Name of the Zone */
	_tstring name;

	/** Set of objects that reside within this Zone */
	ActorSet objects;

	/** Brick and Mortar walls of the Zone */
	Map map;
};


}; // namespace



#endif
