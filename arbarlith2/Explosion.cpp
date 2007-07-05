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

#include "stdafx.h"
#include "engine/ActorSet.h"
#include "engine/Creature.h"
#include "engine/Zone.h"
#include "Explosion.h"

namespace Arbarlith2 {

/**
Determines, based on distance from the center of the explosion, what splash damage should be dealt
@param radius falloff radius of the explosion
@param distance Distance to the center of the explosion
@param baseDamage the base damage of the explosion before radial fall-off
@return splash damage
*/
int splashDamage(float radius, float distance, int baseDamage)
{
	return (int)floorf(baseDamage * powf((float)M_E, -SQR(distance/radius)));
}

void createExplosion(Zone &zone, const vec3 &position, int damageValue, OBJECT_ID attackerID, const _tstring &explosionParticleFile, const _tstring &soundEffectFileName)
{
	// spawn some explosion FX
	size_t particleHandle = zone.SpawnPfx(explosionParticleFile, position);
	g_SoundSystem.play(soundEffectFileName);

	// Get the radius of the particle system
	float radius = zone.particles[particleHandle]->getRadius();

	// Get the set of objects in the zone and within the radius
	ActorSet s = zone.getObjects().typeFilter<Creature>();

	for(ActorSet::iterator i = s.begin(); i != s.end(); ++i)
	{
		Creature *a = dynamic_cast<Creature*>(i->second);

		int finalDamage = splashDamage(radius, vec3(position - a->getPos()).getMagnitude(), damageValue);

		if(finalDamage>0)
		{
			a->damage(finalDamage, attackerID);
		}
	}
}

}; // namespace
