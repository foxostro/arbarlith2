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

#ifndef _MONSTER_FSM_H_
#define _MONSTER_FSM_H_

#include "StateMachine.h"


namespace Engine {



/** FSM for simple, mindless monsters */
class MonsterFSM : public StateMachine
{
public:
	GEN_RTTI(MonsterFSM)

public:
	/** Constructs the FSM for a particular Creature */
	MonsterFSM(OBJECT_ID handle);

	/** Processes the FSM states */
	virtual bool States( StateMachineEvent event, Message_s * msg, int state );

protected:
	/**
	Gets a handle to the closest applicable target, or else returns INVALID_ID
	@return Handle to the closest target creature
	*/
	OBJECT_ID getClosestTarget(void) const;

	/**
	Gets the set of all actors that may be considered targets by the AI
	@return Set of potential targets
	*/
	virtual ActorSet getApplicableTargets(void) const;

	/** Orders the creature to attack the target */
	virtual void orderTheAttack(void);

	/**
	Gets the next destination while wandering around randomly
	@return vector
	*/
	vec3 getRandomWalk(void);

	/**
	Gets the distance to the target, if possible
	@return distance to target or 0
	*/
	float distanceToTarget(void) const;

	/**
	Gets the distance to the given creature
	@param creature creature
	@return distance to creature or 0
	*/
	float distanceToCreature(const Creature &creature) const;

	/**
	Gets the distance to the given point
	@param b The point given
	@return distance to target or 0
	*/
	float distanceToPoint(const vec3 &b) const;

	/** Returns true or false depending on whether the target creature exists */
	bool haveTarget(void) const;

	/**
	Translates the targetCrature handle into an actual object
	@return target creature
	*/
	Creature& getTarget(void) const;

	/** Handle to the target creature */
	OBJECT_ID targetCreatureHANDLE;

	/** Distance threshold at which to notice and take interest in nearby players */
	float thresholdGainInterest;

	/** Distance threshold at which to lose interest in nearby players */
	float thresholdLoseInterest;

	/** Distance threshold at which to stop wandering and return to the spawn point */
	float thresholdWanderTooFar;

	/** Health % below which the creature will begin to flee from attackers */
	float fleeThresholdForHealth;
};

} // namespace Engine

#endif
