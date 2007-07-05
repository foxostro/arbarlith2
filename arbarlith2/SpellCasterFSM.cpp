/*
Original Author: Andrew Fox
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
#include "engine/World.h"
#include "engine/Zone.h"
#include "engine/Creature.h"
#include "engine/Player.h"
#include "engine/random.h"
#include "Spell.h"
#include "Monster.h"
#include "SpellCasterFSM.h"



#ifdef _MSC_VER
#pragma warning( disable: 4127 ) // conditional expression is constant
#pragma warning( disable: 4189 ) // unreferenced local variable
#endif



namespace Arbarlith2 { 

GEN_FSM_RTTI_CPP(SpellCasterFSM)

enum States
{
	STATE_Rnd,
	STATE_ReturnToSpawn,
	STATE_Spell_Attack,
	STATE_Flee
};

SpellCasterFSM::SpellCasterFSM(OBJECT_ID handle)
: thresholdSpellRange(5),
  MonsterFSM(handle)
{
	thresholdGainInterest = 7;
	thresholdLoseInterest = 9;
	thresholdWanderTooFar = 9;
}

void SpellCasterFSM::orderTheAttack(void)
{
	ASSERT(m_Owner!=0, _T("Owner was null"));
	ASSERT(haveTarget(), _T("Method requires that owner has a target first"));

	Monster* monsterOwner = dynamic_cast<Monster*>(m_Owner);
	ASSERT(monsterOwner!=0, _T("Owner is not a Monster type object.  This is required."));

	if(distanceToTarget() < thresholdSpellRange)
	{
		m_Owner->CancelOrders();
		m_Owner->lookAt(getTarget().getPos());

		// Cast an offensive spell at the target	
		Spell *spell = monsterOwner->getSpellPtr();
		ASSERT(spell!=0, _T("Owner's preferred spell was null"));
		spell->beginCast();
	}
	else
	{
		// Move towards the target to get in range
		if(!m_Owner->HasOrders())
		{
			m_Owner->QueueCommand(  CommandMoveToTarget(getTarget())  );
		}
	}
}

}; // namespace
