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
#include "engine/ActorSet.h"
#include "engine/World.h"
#include "engine/Zone.h"
#include "Explosion.h"
#include "Bullet.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(Bullet, "class Arbarlith2::Bullet")

Bullet::Bullet(OBJECT_ID ID)
:Trigger(ID)
{}

void Bullet::clear(void)
{
	Trigger::clear();

	owner = INVALID_ID;
	topSpeed = 3;
	frictionAcceleration=0.0f;
	velocity = vec3(0,0,0);
	particleHandle = 0;
	damageValue = 50;
}

void Bullet::onSlidOnWall(void)
{
	Trigger::onSlidOnWall();
	kill();
}

void Bullet::update(float deltaTime)
{
	cylinderRadius = 0.2f;
	sphereRadius = 0.2f;
	solid = true;
	floating = true;

	Trigger::update(deltaTime);

	// Kill the bullet once it has collided with anything
	if(anyCollisions())
	{
		kill();
	}
	else
	{
		ParticleSystem *s = getZone().particles[particleHandle];
		s->setPosition(position);
	}
}

void Bullet::shoot(OBJECT_ID owner, const vec3 &velocity, int damageValue, const _tstring &particleDef, const _tstring &explosionParticleFile, const _tstring &explosionSoundEffectFile, bool causesFreeze, float knockbackMagnitude)
{
	this->owner = owner;
	this->damageValue = damageValue;
	this->velocity = velocity;
	this->explosionParticleFile = explosionParticleFile;
	this->explosionSoundEffectFile = explosionSoundEffectFile;
	this->causesFreeze = causesFreeze;
	this->knockbackMagnitude = knockbackMagnitude;

	topSpeed = velocity.getMagnitude();
	frictionAcceleration = 0.0f;

	// Fire off the particle system, have it follow the bullet's position
	particleHandle = getZone().SpawnPfx(particleDef, getPos());
}

bool Bullet::pollConditions(void) const
{
	OBJECT_ID unused;

	// Only consider Creature from our Zone that are not the Bullet owner
	ActorSet s = getZone().getObjects().typeFilter<Creature>().exclude(owner);

	return isAnythingInProximity(s, unused);
}

void Bullet::onTrigger(void)
{
	OBJECT_ID id = INVALID_ID;

	// Only consider Creature from our Zone that are not the Bullet owner
	ActorSet s = getZone().getObjects().typeFilter<Creature>().exclude(owner);

	if(isAnythingInProximity(s, id))
	{
		Creature& creature = dynamic_cast<Creature&>(s.get(id));

		creature.damage(damageValue, owner);
		creature.applyKnockBack(creature.getPos()-getPos());

		if(causesFreeze)
			creature.freeze();

		kill();
	}
}

void Bullet::kill(void)
{
	zombie=true;

	// release the particle system now that we are done with it
	ParticleSystem *s = getZone().particles[particleHandle];
	s->Kill();

	// splash damage explosion
	createExplosion(getZone(), getPos(), damageValue/2, owner, explosionParticleFile, explosionSoundEffectFile);
}

bool Bullet::isInProximity(OBJECT_ID actor, float triggerRadius) const
{
	const ActorSet &s = getZone().getObjects();

	if(zombie || !s.isMember(actor))
		return false;

	const Actor &a = s.get(actor);

	const Creature *creature = dynamic_cast<const Creature*>(s.getPtr(actor));

	/*
	Only accept actors that are not slated for deletion.
	If the actor is also a creature, then only accept it if its alive
	*/
	if(  !a.zombie && (creature==0 || (creature!=0 && creature->isAlive()))  )
	{
		float minDist = (triggerRadius + a.getCylinderRadius());
		float realDist = getDistance(this, a) - minDist;
		if(realDist < 0.0f)
		{
			return true;
		}
	}

	return false;
}

} // namespace Arbarlith2
