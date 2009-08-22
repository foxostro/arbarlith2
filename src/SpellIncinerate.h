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

#ifndef _SPELL_INCINERATE_H_
#define _SPELL_INCINERATE_H_

#include "Spell.h"

namespace Arbarlith2 {

/** Creates an explosion on top of every creature near the caster */
class SpellIncinerate : public Spell
{
public:
	/**
	Constructor
	@param zone The zone in which the spell applies
	@param ownerID Spell caster owner
	@param fileName Loads spell data from the file
	*/
	SpellIncinerate(World *zone, OBJECT_ID ownerID, const string &fileName)
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
	/**
	Get applicable target group:
	-> If the spell was cast by a player, it may deal damage to other players accidentally
	-> If the spell was casst by a creature, only players will be damaged.  This prevents
	   creatures from mass killing eachother
	@param owner the spell caster
	*/
	ActorSet getApplicableGroup(Creature &owner) const;

	/** Damage to deal to creatures in the immediate vicinity */
	int damageValue;

	/** radius of the effect */
	float spellRadius;
};

} // namespace Arbarlith2

#endif
