/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009 Game Creation Society
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

#ifndef _BULLET_H_
#define _BULLET_H_

#include "engine/Trigger.h"

using namespace Engine;

namespace Arbarlith2 {

/** Shoots a projectile that can kill creatures */
class Bullet : public Trigger
{
public:
	GEN_RTTI(Bullet, "class Arbarlith2::Bullet")

public:
	/**
	Constructs the Trigger
	@param ID The desired unique ID of the object
	*/
	Bullet(OBJECT_ID ID);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Shoots the bullet in a new direction
	@param owner the bullet's originator cannot be harmed by it
	@param velocity constant velocity of the bullet
	@param damageValue The amount of damage done by the bullet
	@param particleDef file name of the particle system definition
	@param explosionParticleFile file name for the particle explosion definition
	@param explosionSoundEffectFile file name for the explsoion sound effect
	@param causesFreeze indicates that the bullet will cause the target to freeze for a time
	@param knockbackMagnitude Knockback acceleration to apply to the target
	*/
	virtual void shoot(OBJECT_ID owner, const vec3 &velocity, int damageValue, const string &particleDef, const string &explosionParticleFile, const string &explosionSoundEffectFile, bool causesFreeze, float knockbackMagnitude);

	/** kills the bullet */
	virtual void kill(void);

	/**
	Determines whether a given actor is in close proximity to us.
	If the actor is a Creature, then ignore dead actors.
	@param actor the actor in question
	@param triggerRadius Radius to define "proximity" as
	@return true if the specified actor is in proximity to us
	*/
	virtual bool isInProximity(OBJECT_ID actor, float triggerRadius) const;

	/**
	The originator of the bullet
	@return the bullet originator's ID
	*/
	OBJECT_ID getOwner(void) const
	{
		return owner;
	}

protected:
	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	virtual bool pollConditions(void) const;

	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/** Called in the event that the actor slid against a wall */
	virtual void onSlidOnWall(void);

public:
	/** damage that the bullet will do to the target */
	int damageValue;

private:
	/** bullet causes the target to freeze */
	bool causesFreeze;

	/** The originator of the bullet */
	OBJECT_ID owner;

	/** handle to the particle system */
	size_t particleHandle;

	/** Particle effects to use for the explosion */
	string explosionParticleFile;

	/** sound effects to use for the explosion */
	string explosionSoundEffectFile;

	/** Knockback acceleration to apply to the target */
	float knockbackMagnitude;
};

} // namespace Arbarlith2

#endif
