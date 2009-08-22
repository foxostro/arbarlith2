/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2007 Game Creation Society
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

#ifndef _SHADOW_MANAGER_H_
#define _SHADOW_MANAGER_H_

#include "Shadow.h"

namespace Engine {

class World;

const size_t MAX_SHADOWS = 6;

class ShadowManager
{
public:
	ShadowManager(void)
	{
		clear();
	}

	~ShadowManager(void)
	{
		destroy();
	}

	/**	Clears the manager to a just-constructed state */
	void clear(void);

	/**	Cleanly destroys and clears the manager */
	void destroy(void);

	/**	Reconstructs the Shadow Manager */
	void create(void);

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Sets the scene
	@param zone Scene
	*/
	void setZone(World *zone);

	/**
	Update the shadows in the world
	@param zoneActors Actors may be drawn out of this source
	@param deltaTime Time elapsed since the last update
	*/
	void update(const ActorSet &zoneActors, float deltaTime);

	/**
	Gets the maximum number of shadows possible
	@return the maximum number of shadows
	*/
	size_t getMaxShadows(void) const;

	/**
	Gets a particular shadow
	@param idx Index of the shadow
	@return Reference to the shadow
	*/
	const Shadow& getShadow(size_t idx) const;

private:
	/** All shadows */
	vector<Shadow*> shadows;

	/** The scene */
	World *zone;

	/** times milliseconds until our periodic calculations */
	float periodicTimer;

	/** Reassign the shadow's objects */
	void reassignShadows(void);
};

} // namespace Engine

#endif
