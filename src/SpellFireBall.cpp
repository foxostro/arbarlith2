/*
Author: Andrew Fox
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

#include "stdafx.h"
#include "engine/gl.h"
#include "Bullet.h"
#include "SpellFireBall.h"

namespace Arbarlith2 { 

void SpellFireBall::clear(void)
{
	Spell::clear();

	damageValue = 20;

	bulletSpeed = 4.0f;

	particleFile = "data/particle/fireball.xml";

	explosionParticleFile = "data/particle/fireball-explosion.xml";

	explosionSoundEffectFile = "data/sound/s_explosion.wav";

	causesFreeze = false;

	knockbackMagnitude = 0;

	height = 0.5f;
}

void SpellFireBall::load(PropertyBag &xml, Engine::World *zone, Engine::OBJECT_ID ownerID)
{
	Spell::load(xml, zone, ownerID);

	xml.getSym(damageValue);
	xml.getSym(bulletSpeed);
	xml.getSym(particleFile);
	xml.getSym(explosionParticleFile);
	xml.getSym(explosionSoundEffectFile);
	xml.getSym(causesFreeze);
	xml.getSym(knockbackMagnitude);
	xml.getSym(height);
}

void SpellFireBall::castSpell(void)
{
	ASSERT(zone!=0, "zone is null");

	ActorSet &s = zone->getObjects();
	
	ASSERT(s.isMember(ownerID), "owner is not present in the active zone");

	Actor &owner = s.get(ownerID);

	OBJECT_ID id = s.create(Bullet::getTypeString(), zone);

	Bullet &bullet = dynamic_cast<Bullet&>(s.get(id));

	bullet.Place(owner.getPos() + vec3(0, owner.getHeight()*height, 0));
	bullet.shoot(owner.m_ID, -owner.getOrientation().getAxisZ() * bulletSpeed, damageValue, particleFile, explosionParticleFile, explosionSoundEffectFile, causesFreeze, knockbackMagnitude);
}

}; // namespace
