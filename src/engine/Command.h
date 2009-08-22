/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009 Game Creation Society
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

#ifndef _COMMAND_H_
#define _COMMAND_H_

namespace Engine {

const float defaultTimeout = 2000.0f;

/** Represents a generic low-level AI command for a waypoint */
class Command
{
public:
	/** The action the Creature will attempt */
	enum enumCommand
	{
		/** Stand in place */
		FREEZE,

		/** Move to an object */
		MOVE_TARGET,

		/** Move to a point in space */
		MOVE_POS,

		/** Swing at the target if in range */
		ATTACK,

		/** Move to an object and use it */
		USE,

		/** Flee from the target until we have exceeded the proximity distance or the time out has expired */
		FLEE
	};

	/**
	Constructs the Command
	@param cmd The action to be performed on this waypoint
	@param pos The absolute position of the waypoint
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	Command(enumCommand cmd,
	        vec3 pos,
	        float speed=1.0f,
	        float proximity=1.0f,
	        float timeout=defaultTimeout)
	: commandType(cmd),
	  position(pos),
	  target(INVALID_ID),
	  desiredSpeed(speed),
	  desiredProximity(proximity),
	  timeOut(timeout)
	{}

	/**
	Constructs the Command
	@param cmd The action to be performed on this waypoint
	@param id The ID of the target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	Command(enumCommand cmd,
	        OBJECT_ID id,
	        float speed=1.0f,
	        float proximity=1.0f,
	        float timeout=defaultTimeout)
	: commandType(cmd),
	  target(id),
	  desiredSpeed(speed),
	  desiredProximity(proximity),
	  timeOut(timeout)
	{}

	/**
	Constructs the Command
	@param cmd The action to be performed on this waypoint
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	Command(enumCommand cmd,
	        const Actor &actor,
	        float speed=1.0f,
	        float proximity=1.0f,
	        float timeout=defaultTimeout)
	: commandType(cmd),
	  target(actor.m_ID),
	  desiredSpeed(speed),
	  desiredProximity(proximity),
	  timeOut(timeout)
	{}

	/** Destructor */
	virtual ~Command(void)
	{}

	/**
	Gets the target
	@return the target
	*/
	OBJECT_ID getTarget(void) const
	{
		return target;
	}

	/**
	Gets the desiredSpeed
	@return the desiredSpeed
	*/
	float getDesiredSpeed(void) const
	{
		return desiredSpeed;
	}

	/**
	Gets the desiredProximity
	@return the desiredProximity
	*/
	float getDesiredProximity(void)	const
	{
		return desiredProximity;
	}

	/**
	Gets the position
	@return the position
	*/
	const vec3& getLocation(void) const
	{
		return position;
	}

	/**
	Gets the commandType
	@return the commandType
	*/
	enumCommand getCommand(void) const
	{
		return commandType;
	}

	/**
	Gets the timeOut
	@return the timeOut
	*/
	float getTimeOut(void) const
	{
		return timeOut;
	}

private:
	/** The action to be performed on this waypoint */
	enumCommand commandType;

	/** The absolute position of the waypoint */
	vec3 position;

	/** The object that a target refers to */
	OBJECT_ID target;

	/** Percentage of full speed to navigate to the waypoint at */
	float desiredSpeed;

	/** Distance from waypoint that it can be considered to be reached */
	float desiredProximity;

	/** Milliseconds until the order is cancelled */
	float timeOut;
};



/** AI command that orders the Actor to freeze for a specified timeout */
class CommandFreeze : public Command
{
public:
	/**
	Constructs the FreezeCommand
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandFreeze(float timeout) : Command(FREEZE, INVALID_ID, 0, 0, timeout) {}

	/**
	Constructs the FreezeCommand
	@param timeout Milliseconds until the order is cancelled
	@param target Stare at this target
	*/
	CommandFreeze(float timeout, OBJECT_ID target) : Command(FREEZE, target, 0, 0, timeout) {}
};



/** AI command that orders the Actor to move to a specified location */
class CommandMoveToLocation : public Command
{
public:

	/**
	Constructs the CommandMoveToLocation
	@param pos The position in space
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandMoveToLocation(const vec3 &pos,
		                  float speed=1.0f,
						  float proximity=1.0f,
						  float timeout=defaultTimeout)
	: Command(MOVE_POS, pos, speed, proximity, timeout)
	{}
};



/** AI command that orders the Actor to move to an object */
class CommandMoveToTarget : public Command
{
public:

	/**
	Constructs the CommandMoveToTarget
	@param id The ID of the target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandMoveToTarget(OBJECT_ID id,
			            float speed=1.0f,
			            float timeout=defaultTimeout)
	: Command(MOVE_TARGET, id, speed, FLT_EPSILON, timeout)
	{}

	/**
	Constructs the CommandMoveToTarget
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandMoveToTarget(const Actor &actor,
			            float speed=1.0f,
			            float timeout=defaultTimeout)
	: Command(MOVE_TARGET, actor.m_ID, speed, FLT_EPSILON, timeout)
	{}

	/**
	Constructs the CommandMoveToTarget
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandMoveToTarget(const Actor *actor,
			            float speed=1.0f,
			            float timeout=defaultTimeout)
	: Command(MOVE_TARGET, actor->m_ID, speed, FLT_EPSILON, timeout)
	{}
};



/** AI command that orders the Actor to attack an object */
class CommandAttack : public Command
{
public:

	/**
	Constructs the CommandAttack
	@param id The ID of the target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandAttack(OBJECT_ID id,
			      float speed=1.0f,
			      float timeout=defaultTimeout)
	: Command(ATTACK, id, speed, FLT_EPSILON, timeout)
	{}

	/**
	Constructs the CommandAttack
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandAttack(const Actor &actor,
			      float speed=1.0f,
			      float timeout=defaultTimeout)
	: Command(ATTACK, actor.m_ID, speed, FLT_EPSILON, timeout)
	{}

	/**
	Constructs the CommandAttack
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandAttack(const Actor *actor,
			      float speed=1.0f,
			      float timeout=defaultTimeout)
	: Command(ATTACK, actor->m_ID, speed, FLT_EPSILON, timeout)
	{}
};



/** AI command that orders the Actor to use an object */
class CommandUse : public Command
{
public:

	/**
	Constructs the CommandUse
	@param id The ID of the target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandUse(OBJECT_ID id,
			   float speed=1.0f,
			   float timeout=defaultTimeout)
	: Command(USE, id, speed, 0.0f, timeout)
	{}

	/**
	Constructs the CommandUse
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandUse(const Actor &actor,
			   float speed=1.0f,
			   float timeout=defaultTimeout)
	: Command(USE, actor.m_ID, speed, 0.0f, timeout)
	{}

	/**
	Constructs the CommandUse
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandUse(const Actor *actor,
			   float speed=1.0f,
			   float timeout=defaultTimeout)
	: Command(USE, actor->m_ID, speed, 0.0f, timeout)
	{}
};



/** AI command that orders the Actor to flee from object */
class CommandFlee : public Command
{
public:
	/**
	Constructs the CommandFlee
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandFlee(const Actor *actor,
		    float speed,
		    float proximity,
		    float timeout=defaultTimeout)
	: Command(FLEE, actor->m_ID, speed, proximity, timeout)
	{}

	/**
	Constructs the CommandFlee
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandFlee(const Actor &actor,
		    float speed,
		    float proximity,
		    float timeout=defaultTimeout)
	: Command(FLEE, actor.m_ID, speed, proximity, timeout)
	{}

	/**
	Constructs the CommandFlee
	@param actor The target object
	@param speed Percentage of full speed to navigate to the waypoint at
	@param proximity Distance from waypoint that it can be considered to be reached
	@param timeout Milliseconds until the order is cancelled
	*/
	CommandFlee(OBJECT_ID actor,
		    float speed,
		    float proximity,
		    float timeout=defaultTimeout)
	: Command(FLEE, actor, speed, proximity, timeout)
	{}
};

} // namespace Engine

#endif
