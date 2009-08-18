/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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
#ifndef _CREATURE_H_
#define _CREATURE_H_

#include <deque>
using std::deque;

#include "object.h"
#include "Command.h"

namespace Engine {

// class prototypes
class StateMachine;
class World;


/** Active AI entity */
class Creature : public Actor
{
public:
	GEN_RTTI(Creature, "class Engine::Creature")

public:
	/** Senses sort objects by one of several qualities */
	enum SORT_TYPE
	{
		/** Returned entries are sorted by increasing distance from the calling Creature */
		SORT_DIST_ASCENDING,

		/** Returned entries are sorted by decreasing distance from the calling Creature */
		SORT_DIST_DESCENDING
	};

	/**
	Constructs a blank Creature
	@param ID The unique identifying number for the Creature
	*/
	Creature(OBJECT_ID ID);

	/** Destructor */
	virtual ~Creature(void);

	/** Initializes the Creature to a just constructed state */
	virtual void clear(void);

	/** Destroys the Creature instance */
	virtual void destroy(void);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Updates the creature
	@param deltaTime The milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Processes a message received from the message routing system
	@param message The message received
	*/
	virtual void OnMessage(Message_s message);

	/** Response when the creature dies */
	virtual void OnDeath(void);

	/** Kills the creature at the next tick */
	virtual void kill(void);

	/** Transforms and renders the creature */
	virtual void drawObject(void) const;

	/** Transforms and renders the transparent parts of the creature */
	virtual void drawTransparentObject(void) const;

	/** Used by drawObject to render Debug data */
	virtual void drawObjectDebugData(void) const;

	/**
	Administer healing to the creature, restoring some amount of HP
	@param healValue Total amount of HP to restore
	*/
	virtual void heal(int healValue);

	/** Freezes the creature for a time */
	virtual void freeze(void);

	/**
	Damages the creature
	@param amount The amount of damage
	@param attackerID The ID of the attacking creature or INVALID_ID if this is not applicable
	*/
	virtual void damage(int amount, OBJECT_ID attackerID);

	/**
	Respond to the event of being dealt damage by another creature
	@param attacker
	@param finalDamage The final damage dealt after all modifiers
	*/
	virtual void OnAttacked(Creature &attacker, int finalDamage);

	/** Resurrect the creature, bringing it back to life on the spot */
	virtual void resurrect(void);

	/**
	Set the AI state machine. The old one is deleted.
	@param pFSM The new state machine
	*/
	virtual void SetStateMachine(StateMachine *pFSM)
	{
		// Set to the new one
		m_pFSM = pFSM;
	}

	/**
	Sets the Finite State Machine of the creature
	@param strFSM The name of the FSM
	*/
	void setFSM(const string &strFSM);

	/**
	Retrieves a pointer to the AI state machine
	@return The AI state machine
	*/
	inline StateMachine* GetStateMachine(void) const
	{
		return m_pFSM;
	}

	/**
	If possible, begin an attack action directed towards the given target
	@param id the id of the creature to attack
	*/
	virtual void startAttackAction(OBJECT_ID id);

	/**
	Queues a command to be handled when all commands previous to it are completed
	@param wp The command waypoint
	@return true if the command was successfully queued
	*/
	bool QueueCommand(const Command &wp);

	/**
	Injects a command to the front if the order queue.
	This command will be completed and then existing orders wil be resumed.
	@param wp The command waypoint
	@return true if the command was successfully queued
	*/
	bool InjectCommand(const Command &wp);

	/** Cancels all AI orders */
	void CancelOrders(void);

	/**
	Determines whether the AI has orders pending completion
	@return true if the AI has orders pending completion
	*/
	inline bool HasOrders(void) const
	{
		return ordersRemaining.empty()==false;
	}

	/**
	Gets the current order
	@return Command
	*/
	inline Command& getCurrentOrder(void)
	{
		return ordersRemaining.front();
	}

	/**
	Gets the current order
	@return Command
	*/
	inline const Command& getCurrentOrder(void) const
	{
		return ordersRemaining.front();
	}

	/**
	Determines whether the creature is alive or dead
	@return true if the creature is alive, false if dead
	*/
	inline bool isAlive(void) const
	{
		return(state != DYING && state != DEAD && state != GHOST);
	}

	/**
	Determines the health percentage of the creature
	@return HP / maxHP
	*/
	inline float getHealthPercentage(void) const
	{
		return((float)healthPoints / maxHealthPoints);
	}

	/**
	Gets the name of the Creature.
	@return The Creature's name
	*/
	inline const string& getName(void) const
	{
		return m_strName;
	}

	/**
	Creates the editor toolbar for the Creature
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

	/** Gets the name of the proper pain animation */
	virtual const string getPainAnim(void) const;

	/** Gets the name of the proper dying animation */
	virtual const string getDyingAnim(void) const;

	/** Gets the name of the proper attack animation */
	virtual const string getAttackAnim(void) const;

	/** Gets the name of the proper idle/stand animation */
	virtual const string getIdleAnim(void) const;

	/** Gets the name of the proper run-fwd animation */
	virtual const string getRunFwdAnim(void) const;

	/** Gets the name of the proper run-fwd animation */
	virtual const string getRunRevAnim(void) const;

	/** Gets the name of the proper run-left animation */
	virtual const string getRunLeftAnim(void) const;

	/** Gets the name of the proper run-right animation */
	virtual const string getRunRightAnim(void) const;

	/** Gets the name of the proper run-fwd-left animation */
	virtual const string getRunLeftFwdAnim(void) const;

	/** Gets the name of the proper run-fwd-right animation */
	const string getRunRightFwdAnim(void) const;

	/** Gets the name of the proper run-rev-left animation */
	virtual const string getRunLeftRevAnim(void) const;

	/** Gets the name of the proper run-rev-right animation */
	virtual const string getRunRightRevAnim(void) const;

	/**
	Gets the name of the proper walking animation
	@param speed meters per second speed of the creature
	@return animation name
	*/
	virtual const string getWalkAnim(float speed) const;

	/**
	Get the name of the sound effect file to play when the creature is dying
	@return sound effect file name
	*/
	virtual const string getDyingSfx(void) const;

	/**
	Get the name of the sound effect file to play when the creature is hurt
	@return sound effect file name
	*/
	virtual const string getHurtSfx(void) const;

	/**
	Get the name of the sound effect file to play when the creature is does an attack action
	@return sound effect file name
	*/
	virtual const string getAttackSfx(void) const;

	/**
	Get the name of the sound effect file to play when the creature is does an attack action
	@return sound effect file name
	*/
	virtual const string getAttnSfx(void) const;

	/** Called in the event that the we are placed */
	virtual void OnPlace(void);

	/**
	Apply a new knock back action, or replace an existing one
	@param direction Vector pointing towards the direction to knock back along
	*/
	void applyKnockBack(const vec3 &direction);

	/** Called when knock back occurs */
	virtual void OnKnockBack(void);

private:
	/**
	Processes the current command.
	The FSM queues commands for the creature to follow.
	ProcessCommand will execute the current command each tick.
	*/
	void ProcessCommand(void);

	/**
	Moves the Creature to the target object's position
	@param wayPoint The waypoint and the order itself
	*/
	void Process_MoveToTarget(const CommandMoveToTarget &wayPoint);

	/**
	Moves the Creature to the target waypoint's position
	@param wayPoint The waypoint and the order itself
	*/
	void Process_MoveToPos(const CommandMoveToLocation &wayPoint);

	/**
	Orders the Creature to attack the target
	@param wayPoint The waypoint and the order itself
	*/
	void Process_AttackTarget(const CommandAttack &wp);

	/**
	Orders the Creature to flee the target
	@param wayPoint The waypoint and the order itself
	*/
	void Process_FleeTarget(const CommandFlee &wp);

	/**
	Orders the Creature to use the target
	@param wayPoint The waypoint and the order itself
	*/
	void Process_UseTarget(const CommandUse &wp);



	/** When knocked back, this is how much more time until we exit the knockback state */
	float knockBackTimer;

	/** When knocked back, scoot backwards for this amount of time */
	float maxKnockBackTime;

	/** When knocked back, scoot backwards with this speed */
	float knockBackSpeed;

	/** When knocked back, scoot backwards with this velocity */
	vec3 currentKnockBackVelocity;

	/** Longest stun time that can be inflicted */
	float maxStunTime;

	/** When dealt damage greater thann this percent of remaining health, the creature is stunned */
	float damagePercentToStun;

	/** angle of the stars floating around our head while stunned */
	float starAngle;

	/** speed that the stars around our head spin (radians/millisecond) */
	float starSpeed;

	/** material to use for stars */
	Material starMaterial;

	/** Controls High-Level AI */
	StateMachine *m_pFSM;

	/** the creature's HP */
	int healthPoints;

	/** the creature's max HP */
	int maxHealthPoints;

	/** the base amount of damage the creature can deal */
	float attackDamage;

	/** damage dealt by the creature is multiplied by this value */
	float weaponMultiplier;

	/** damage dealt to the creature is multiplied by this value */
	float armorMultiplier;

	/** base time between attacks */
	float attackCoolDown;

	/** attack cool down is multiplied by this value */
	float attackCoolDownMultiplier;

	/** attack cool down timer counts down milliseconds until the next attack is possible */
	float attackCoolDownTimer;

	/** base attack charge time */
	float attackChargeTime;

	/** Counts the milliseconds since the last attack */
	float timeSinceLastAttack;

	/** spell cool down (specified by the spell itself) is multiplied by this value */
	float spellCoolDownMultiplier;

	/** spell cool down timer counts down milliseconds until the next attack is possible */
	float spellTimer;

	/** Timer to stagger the FSM updates between several frames */
	float fsmUpdateTimer;

	/** Pending low-level AI commands to complete */
	deque<Command> ordersRemaining;

	/** Counts down to zero the amount of time until the current order is cancelled */
	float timeUntilOrderCancelled;

	/**
	Every tick that an AI command must execute, the
	target is retrieved at the beginning of the tick
	*/
	Actor *target;

	/** Inicates that a message has recently been received */
	bool haveMessage;

	/** Last received message */
	Message_s lastMessage;

protected:
	/**
	Draws stars floating above the creature's head
	@param y Height above the ground beneath the player
	*/
	void drawStarsAboveHead(float y) const;

	/** Draws the health bar for the creature */
	void drawHealthBar(void) const;

	/** Draws the attack-charge bar for the creature */
	void drawChargeBar(void) const;

	/**
	Draws a floating bar
	@param y Height above the ground beneath the player
	@param p Percent full
	@param a Color 1
	@param b Color 2
	*/
	void drawFloatingBar(float y, float p, COLOR a, COLOR b) const;

	/**
	The attack animation has completed, actually deal damage to the fuckers
	@param amount The amount of damage
	@param attackedCreature The creature that we are attacking
	*/
	virtual void dealAttackDamage(int amount, OBJECT_ID attackedCreature);

	/**
	Determines whether a given actor is in close proximity to us.
	If the actor is a Creature, then ignore dead actors.
	@param actor the actor in question
	@param triggerRadius Radius to define "proximity" as
	@return true if the specified actor is in proximity to us
	*/
	virtual bool isInProximity(OBJECT_ID actor) const;

	/**
	Determines whether a given actor is in close proximity to us.
	If the actor is a Creature, then ignore dead actors.
	@param actor the actor in question
	@param triggerRadius Radius to define "proximity" as
	@return true if the specified actor is in proximity to us
	*/
	virtual bool isInProximity(OBJECT_ID actor, float triggerRadius) const;

	/**
	Determines whether the creature can move
	@return the player can move
	*/
	virtual bool canMove(void) const;

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** Cancels the current order and goes to the next one */
	void gotoNextOrder(void);

	/**
	Has the Creature walk towards the specified target position
	@param target Target position
	@param speed value between -1 and +1 to describe the speed to use when walking
	*/
	virtual void walkTowards(const vec3 &target, float speed);

	/** Dying sounds */
	vector<string> dyingSounds;

	/** Pain sounds */
	vector<string> painSounds;

	/** Melee attack sounds */
	vector<string> attackSounds;

	/** Attention sounds */
	vector<string> attnSounds;

	/** essential state of the Creature */
	enum
	{
		NORMAL,
		STUNNED,
		FROZEN,
		DYING,
		DEAD,
		GHOST
	} state;

	/** timer specific to the current Essential State of the creature (milliseconds) */
	float stateTimer;
};

} //namespace Engine

#endif
