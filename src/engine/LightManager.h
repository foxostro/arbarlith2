/*
Original Author: Andrew Fox
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

#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

#include "Light.h"
#include "camera.h"

namespace Engine {

/** Handle to a Light in the manager */
typedef int HLIGHT;

/** A type for a list of lights */
typedef map<HLIGHT, Light*> LIGHTS;

/** A handle set to INVALID_LIGHT is a null, or invalid, handle */
const HLIGHT INVALID_LIGHT = -1;

/** Manages all Lights and how they lights bound for OpenGL */
class LightManager
{
private:
	/** Collection of all lights in the zone */
	LIGHTS lights;

	/** The next handle to assign */
	HLIGHT newHandle;
	/** times milliseconds until our periodic calculations */
	float periodicTimer;

	/** the most recently computed active set of lights */
	vector<const Light*> activeSet;

	/** Determine the active set of lights */
	void computeActiveSet(void);

public:
	/** Ambient light intensity of the scene */
	float ambientLight;

public:
	/** Constructor */
	LightManager(void)
	{
		clear();
	}

	/** Destructor */
	~LightManager(void)
	{
		destroy();
	}

	/**	Clears the manager to a just-constructed state */
	void clear(void);

	/**	Cleanly destroys and clears the manager */
	void destroy(void);

	/**	Reconstructs the Light Manager */
	void create(void);

	/**
	Gets the ambient light intensity for this scene
	@return ambient light intensity
	*/
	inline float getAmbientLight (void) const
	{
		return ambientLight;
	}

	/**
	Adds a light to the zone.
	@param light The light state
	@return Returns a handle to the light.
	*/
	HLIGHT addLight(Light *light);

	/**
	Adds a light to the zone.
	@param light The light state
	@return Returns a handle to the light.
	*/
	HLIGHT addLight(Light &light)
	{
		return addLight(&light);
	}

	/**
	Removes a light from the zone.
	@param handle The handle to the Light
	*/
	void removeLight(HLIGHT handle);

	/**
	gets a light in the zone.
	@param handle The handle to the Light
	@return Returns the light or null, when the handle is invalid
	*/
	Light& getLight(HLIGHT handle);

	/**
	gets a light in the zone.
	@param handle The handle to the Light
	@return Returns the light or null, when the handle is invalid
	*/
	const Light& getLight(HLIGHT handle) const;

	/** Disables the active set of OpenGL lights */
	void disableAll(void) const;

	/** Binds the appropriate lights to OpenGL */
	void bindAll(void) const;

	/**
	Gets the list of active lights
	return list of active lights
	*/
	vector<const Light*> getActiveSet(void) const;

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Update the shadows in the world
	@param deltaTime Time elapsed since the last update
	*/
	void update(float deltaTime);
};

} // namespace Engine

#endif
