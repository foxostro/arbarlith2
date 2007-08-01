/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#ifndef _SPELL_FIREBALL_H_
#define _SPELL_FIREBALL_H_

#include "Spell.h"

namespace Arbarlith2 {

/** Shoots a bullet projectile */
class SpellFireBall : public Spell
{
public:
	/**
	Constructor
	@param zone The zone in which the spell applies
	@param ownerID Spell caster owner
	@param fileName Loads spell data from the file
	*/
	SpellFireBall(World *zone, OBJECT_ID ownerID, const string &fileName)
	{
		loadFromFile(fileName, zone, ownerID);
	}

	/** Resets data */
	virtual void clear(void);

protected:
	/**
	Loads the spell data from file
	@param xml XML source
	@param zone The zone in which the spell applies
	@param ownerID Spell caster owner
	*/
	virtual void load(PropertyBag &xml, Engine::World *zone, Engine::OBJECT_ID ownerID);

	/** Actually casts the spell */
	virtual void castSpell(void);

private:
	/** damage to deal with the bullet */
	int damageValue;

	/** Speed of the bullet */
	float bulletSpeed;

	/** Particle effects to use for the bullet */
	string particleFile;

	/** Particle effects to use for the explosion */
	string explosionParticleFile;

	/** Explosion sound effect file */
	string explosionSoundEffectFile;

	/** Indicates that the bullet freezes the enemy */
	bool causesFreeze;

	/** Knockback acceleration to apply to the target */
	float knockbackMagnitude;

	/** height to spawn the bullet at (% of caster height) */
	float height;
};

} // namespace Arbarlith2

#endif
