/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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
#include "World.h"
#include "Creature.h"
#include "Player.h"
#include "MonsterFSM.h"



#ifdef _MSC_VER
#pragma warning( disable: 4127 ) // conditional expression is constant
#pragma warning( disable: 4189 ) // unreferenced local variable
#endif



namespace Engine {

GEN_FSM_RTTI_CPP(MonsterFSM, "class Engine::MonsterFSM")

enum States
{
	STATE_Rnd,			/** re-evaluate options and randomly wander if no state transition is desireable */
	STATE_Pause_Before_Attack,	/** pause before continuing to attack */
	STATE_Attack,			/** chase and attack the target creature */
	STATE_Flee			/** flee from the target creature */
};

MonsterFSM::MonsterFSM(OBJECT_ID handle)
: StateMachine(handle),
  targetCreatureHANDLE(INVALID_ID),
  thresholdGainInterest(4),
  thresholdLoseInterest(5), // the screen shows us everything in a 6 meter radius, minimum
  thresholdWanderTooFar(9)
{
	fleeThresholdForHealth = FRAND_RANGE(0.05f, 0.20f);
	SetState(STATE_Rnd);
}

vec3 MonsterFSM::getRandomWalk(void)
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	const float radius = FRAND_RANGE(0.0f, thresholdWanderTooFar);

	const float angle = FRAND_RANGE(0, 2.0f * (float)M_PI);

	return m_Owner->getSpawnPoint() + vec3(radius*cosf(angle), 0, radius*sinf(angle));
}

float MonsterFSM::distanceToTarget(void) const
{
	if(haveTarget())
	{
		return distanceToCreature(getTarget());
	}
	else
	{
		return 0.0f;
	}
}

float MonsterFSM::distanceToCreature(const Creature &creature) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	const float radiusA = m_Owner->getCylinderRadius();
	const vec3 a = m_Owner->getPos();

	const float radiusB = creature.getCylinderRadius();
	const vec3 b = creature.getPos();

	float mag=0;
	float minDist = radiusA + radiusB;

	vec3 delta = a - b;
	delta.y=0;
	mag = delta.getMagnitude();

	// meters of physical separation
	return(mag - minDist);
}

float MonsterFSM::distanceToPoint(const vec3 &b) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	const vec3 a = m_Owner->getPos();
	vec3 delta = a - b;
	delta.y=0;
	float mag = delta.getMagnitude();
	return mag;
}

bool MonsterFSM::haveTarget(void) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	const ActorSet &s = m_Owner->getZone().getObjects();

	if(!s.isMember(targetCreatureHANDLE))
	{
		// not a valid handle
		return false;
	}

	const Creature *creature = dynamic_cast<const Creature*>(s.getPtr(targetCreatureHANDLE));

	if(creature == 0)
	{
		// not a creature
		return false;
	}

	if(!creature->isAlive())
	{
		// is a corpse
		return false;
	}

	return true;
}

Creature& MonsterFSM::getTarget(void) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));
	ASSERT(haveTarget(), _T("This FSM does not have a target creature!"));

	ActorSet &s = m_Owner->getZone().getObjects();

	return dynamic_cast<Creature&>(s.get(targetCreatureHANDLE));
}

void MonsterFSM::orderTheAttack(void)
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));
	m_Owner->QueueCommand(CommandAttack(targetCreatureHANDLE, FRAND_RANGE(0.7f, 1.0f)));
}

OBJECT_ID MonsterFSM::getClosestTarget(void) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	const ActorSet s = getApplicableTargets();

	return s.getClosest<Actor>(m_Owner->getPos(), thresholdGainInterest);
}

ActorSet MonsterFSM::getApplicableTargets(void) const
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

	ActorSet s = m_Owner->getZone().getObjects().exclude(m_Owner->m_ID).typeFilter<Player>();

	// remove actors that are dead or ghosts
	ActorSet::iterator i = s.begin();
	while(i != s.end())
	{
		ActorSet::iterator nextIterator = i;
		++nextIterator;

		const Player &player = dynamic_cast<const Player&>(*i->second);

		if(!player.isAlive())
		{
			s.erase(i);
		}

		i = nextIterator;
	}

	return s;
}

bool MonsterFSM::States( StateMachineEvent event, Message_s * msg, int state )
{
	ASSERT(m_Owner!=0, _T("Owner was NULL"));

// Do the state machine:
BeginStateMachine

	///////////////////////////////////////////////////////////////
	State( STATE_Rnd )
		OnEnter
			m_Owner->CancelOrders();

		OnMsg( MSG_ATTACKED )
			// Attacker becomes our target, retaliate
			if(msg->m_Sender != targetCreatureHANDLE)
			{
				targetCreatureHANDLE = msg->m_Sender;
				SetState(STATE_Attack);
			}

		OnUpdate
			targetCreatureHANDLE = getClosestTarget();

			/*
			If a player is close enough to get our attention, then attack.
			Otherwise, wander randomly
			*/
			if(targetCreatureHANDLE != INVALID_ID)
			{
				SetState(STATE_Pause_Before_Attack);
			}
			else if(m_Owner->anyCollisions() || !m_Owner->HasOrders())
			{
				// Go someplace random
				m_Owner->CancelOrders();
				vec3 waypoint = getRandomWalk();
				float speed = FRAND_RANGE(0.5f, 0.7f);
				m_Owner->QueueCommand(CommandMoveToLocation(waypoint, speed, FLT_EPSILON, 2000.f));
			}

	///////////////////////////////////////////////////////////////
	State( STATE_Pause_Before_Attack )
		OnEnter
			// pause for a moment before reacting
			m_Owner->CancelOrders();
			m_Owner->QueueCommand(CommandFreeze(FRAND_RANGE(200.0f, 400.0f), targetCreatureHANDLE));

		OnUpdate
			if(!m_Owner->HasOrders())
			{
				SetState(STATE_Attack);
			}

	///////////////////////////////////////////////////////////////
	State( STATE_Attack )
		OnEnter
			m_Owner->CancelOrders();
			g_SoundSystem.play(m_Owner->getAttnSfx());

		OnMsg( MSG_ATTACKED )
			// Attacker becomes our new target, retaliate
			if(msg->m_Sender != targetCreatureHANDLE)
			{
				targetCreatureHANDLE = msg->m_Sender;
				SetState(STATE_Attack);
			}

		OnUpdate
			/*
			If we don't have a target, resume wandering.
			Else If target is out of range, resume wandering
			Else If we are low on health, flee.
			Else If we don't have anything to do, keep attacking.
			*/
			if(!haveTarget())
			{
				SetState(STATE_Rnd);
			}
			else if(distanceToTarget() > thresholdLoseInterest)
			{
				SetState(STATE_Rnd);
			}
			else if(m_Owner->getHealthPercentage() < fleeThresholdForHealth)
			{
				SetState(STATE_Flee);
			}
			else if(!m_Owner->HasOrders())
			{
				orderTheAttack();
			}

	///////////////////////////////////////////////////////////////
	State( STATE_Flee )
		OnEnter
			m_Owner->CancelOrders();
			g_SoundSystem.play(m_Owner->getAttnSfx());

		OnMsg( MSG_ATTACKED )
			// Attacker becomes our new target, flee
			if(msg->m_Sender != targetCreatureHANDLE)
			{
				targetCreatureHANDLE = msg->m_Sender;
				SetState(STATE_Flee);
			}

		OnUpdate
			/*
			If we don't have a target, resume wandering.
			Else If target is out of range, resume wandering
			Else If we are not low on health anymore, attack again.
			Else If we don't have anything to do, keep fleeing.
			*/
			if(!haveTarget())
			{
				SetState(STATE_Rnd);
			}
			else if(distanceToTarget() > thresholdLoseInterest)
			{
				SetState(STATE_Rnd);
			}
			else if(m_Owner->getHealthPercentage() >= fleeThresholdForHealth)
			{
				SetState(STATE_Attack);
			}
			else if(!m_Owner->HasOrders())
			{
				m_Owner->QueueCommand(CommandFlee(targetCreatureHANDLE, FRAND_RANGE(0.9f, 1.0f), thresholdLoseInterest, 5000.0f));
			}

	///////////////////////////////////////////////////////////////
EndStateMachine
}

} // namespace Engine
