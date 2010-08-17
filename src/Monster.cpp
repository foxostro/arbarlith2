/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009,2010 Game Creation Society
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
#include "SpellFireBall.h"
#include "SpellIncinerate.h"
#include "SpellHeal.h"
#include "SpellCasterFSM.h"
#include "Monster.h"

#include "engine/MonsterFSM.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(Monster, "class Arbarlith2::Monster")

Monster::Monster(OBJECT_ID ID)
: Creature(ID)
{
	clear();
}

void Monster::clear(void)
{
	Creature::clear();

	spell=0;
	preferredSpell=0;
}

void Monster::destroy(void)
{
	delete spell;

	Creature::destroy();
}

const string Monster::getAttackAnim(void) const
{
	return "highhit";
}

void Monster::update(float deltaTime)
{
	if(spell!=0)
	{
		spell->update(deltaTime);
	}

	Creature::update(deltaTime);
}

bool Monster::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, "preferredSpell", preferredSpell);

	StateMachine *fsm = GetStateMachine();

	string type = "none";

	if(fsm != 0)
	{
		type = fsm->getTypeString();
	}

	saveTag(xml, dataFile, "fsm", type);

	return Creature::saveTidy(xml, dataFile);
}

void Monster::load(const PropertyBag &xml)
{
	Creature::load(xml);

	if(xml.exists("preferredSpell")) {
		xml.get_optional("preferredSpell", preferredSpell);
	} else {
		preferredSpell = 0;
	}

	switch(preferredSpell)
	{
	case 0: spell = new SpellFireBall	(&getZone(), m_ID, "data/spells/fireball.xml");		break; // Fireball
	case 1: spell = new SpellFireBall	(&getZone(), m_ID, "data/spells/arctic-wind.xml");	break; // Arctic Wind
	case 2: spell = new SpellIncinerate	(&getZone(), m_ID, "data/spells/incinerate.xml");		break; // Incinerate
	case 3: spell = new SpellFireBall	(&getZone(), m_ID, "data/spells/chill.xml");		break; // Chill
	case 4: spell = new SpellHeal		(&getZone(), m_ID, "data/spells/heal.xml");			break; // Heal
	case 5: spell = new SpellFireBall	(&getZone(), m_ID, "data/spells/ice-blast.xml");		break; // Ice Blast
	case 6: spell = 0; break; // Reserved for "Resurrect" Spell
	default: spell = 0; break;
	};

	if(spell!=0)
	{
		spell->available = true;
	}
}

const string Monster::getSpellCastAnim(void) const
{
	return string("cast");
}

void Monster::onSlidOnWall(void)
{
	Creature::onSlidOnWall();

	// cancel movement orders to force a new waypoint to be generated
	CancelOrders();
}

} //namespace Arbarlith2
