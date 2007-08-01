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
#include "engine/gl.h"
#include "Explosion.h"
#include "SpellIncinerate.h"

namespace Arbarlith2 {

void SpellIncinerate::clear(void)
{
	Spell::clear();

	damageValue = 15;
	spellRadius = 3.0f;
}

void SpellIncinerate::load(PropertyBag &xml, Engine::World *zone, Engine::OBJECT_ID ownerID)
{
	Spell::load(xml, zone, ownerID);

	xml.getSym(damageValue);
	xml.getSym(spellRadius);
}

void SpellIncinerate::castSpell(void)
{
	ASSERT(zone!=0, "zone is null");

	ActorSet &zoneObjects = zone->getObjects();

	ASSERT(zoneObjects.isMember(ownerID), "owner is not present in the active zone");

	Actor &owner = zoneObjects.get(ownerID);

	// Get all enemies in the spell vicinity.
	ActorSet s = getApplicableGroup(dynamic_cast<Creature&>(owner));

	for(ActorSet::const_iterator i=s.begin(); i!=s.end(); ++i)
	{
		OBJECT_ID id = i->first;

		if(s.isMember(id))
		{
			Creature &creature = dynamic_cast<Creature&>(s.get(id));

			createExplosion(owner.getZone(),
			                creature.getPos(),
			                damageValue,
			                1.0f,
			                ownerID,
			                "data/particle/fireball-explosion.xml",
			                "data/sound/s_explosion.wav");
		}
	}
}

ActorSet SpellIncinerate::getApplicableGroup(Creature &owner) const
{
	if(instanceof(owner, Player))
		return owner.getZone().getObjects().exclude(owner.m_ID).getClosestSeveral<Creature>(owner.getPos(), 4, spellRadius);
	else
		return owner.getZone().getObjects().exclude(owner.m_ID).getClosestSeveral<Player>(owner.getPos(), 4, spellRadius);
}

}; // namespace
