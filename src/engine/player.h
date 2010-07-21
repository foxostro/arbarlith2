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

#ifndef PLAYER_H
#define PLAYER_H

#include "singleton.h"
#include "creature.h"
#include "ActorSet.h"
#include "world.h"
#include "LightManager.h"


namespace Engine {



// class prototype
class World;
class Item;

/** Represents the user's avatar */
class Player : public Creature
{
public:
	GEN_RTTI(Player, "class Engine::Player")

public:
	/**
	Constructor
	@param ID The unique ID to assign to the Player
	*/
	Player(OBJECT_ID ID);

	/** Updates the player */
	virtual void update(float deltaTime);

	/** Set everything to defaults */
	virtual void clear(void);

	/** Response when the creature dies */
	virtual void OnDeath(void);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/** Gets the name of the proper pain animation */
	virtual const string getPainAnim(void) const;

	/** Gets the name of the proper dying animation */
	virtual const string getDyingAnim(void) const;

	/** Gets the name of the proper attack animation */
	virtual const string getAttackAnim(void) const;

	/** Gets the name of the proper idle animation */
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
	virtual const string getRunRightFwdAnim(void) const;

	/** Gets the name of the proper run-rev-left animation */
	virtual const string getRunLeftRevAnim(void) const;

	/** Gets the name of the proper run-rev-right animation */
	virtual const string getRunRightRevAnim(void) const;

	/**
	Gets the name of the current item
	@return Name of the current item
	*/
	string getItemName(void) const;

	/**
	Gets the name of the proper walking animation
	@param speed meters per second speed of the creature
	@return animation name
	*/
	virtual const string getWalkAnim(float speed) const;

	/**
	Put an item in the player's inventory
	@param item The item to pickup
	*/
	virtual void pickupItem(Item *item);

	/**
	Discards an object that is in the inventory
	@param id ID for the object
	*/
	virtual void discardItem(OBJECT_ID id);

	/** Cycle up to the next item */
	virtual void cycleItemNext(void);

	/**
	Selects an object that is in the inventory
	@param id ID for the object
	*/
	virtual void selectSpecificItem(OBJECT_ID id);

	/**
	Determines if an item is in the inventory
	@param id ID for the object
	@return true if the item is in the inventory
	*/
	virtual bool isInventoryItem(OBJECT_ID id) const;

	/**
	Determines whether the ActorSet contains an Actor of the specified name
	@param name The name of the actor
	@param out Returns an ID to the actor, or INVALID_ID
	@return true if the actor was found
	*/
	virtual bool inventoryQuery(const string &name, OBJECT_ID &out) const;

	/**
	Sets or changes the player number
	@param playerNumber Player number starts at zero
	*/
	void setPlayerNumber(int playerNumber);

	/**
	Called in the event that the player number is set or changed
	@param playerNumber Player number starts at zero
	*/
	virtual void OnChangePlayerNumber(int playerNumber);

	/**
	Gets the player number (starting at zero) or -1 if this is not applicable
	@return player number (starting at zero)
	*/
	const int getPlayerNumber(void) const
	{
		return playerNumber;
	}

	/**
	Gets the distance that the player is from the average player position
	return distance from the average player position
	*/
	float getDistanceFromAveragePosition(void) const;

	/**
	Has the Creature walk towards the specified target position
	@param target Target position
	@param speed value between -1 and +1 to describe the speed to use when walking
	*/
	virtual void walkTowards(const vec3 &target, float speed);

	/** Removes all controller actions and bindings for this player */
	void deleteController(void);

	/** Setup controller actions and bindings for this player */
	void setupController(void);

public:
	/** Action code for player walk */
	ACTION_CODE KEY_PLAYER_WALK_FWD;

	/** Action code for player walk */
	ACTION_CODE KEY_PLAYER_WALK_REV;

	/** Action code for player walk */
	ACTION_CODE KEY_PLAYER_WALK_LEFT;

	/** Action code for player walk */
	ACTION_CODE KEY_PLAYER_WALK_RIGHT;

	/** Action code for the player 'use' action */
	ACTION_CODE KEY_PLAYER_USE;

protected:
	/** Light to illuminate the area immediately surrounding the player */
	HLIGHT playerGlow;

	/** debounce the USE key */
	bool useKeyDebounce;

	/** The player's inventory */
	ActorSet inventory;

	/**
	Handle to the inventory item we have selected, or else INVALID_ID
	Should be contained within the ActorSet of the inventory
	*/
	OBJECT_ID selectedItem;

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** Directly control the player */
	virtual void directControl(void);

	/**
	Helper for deleteController. Deletes all controller action names for this player
	@param playerNumber Player number, starts at zero
	*/
	virtual void deleteControllerActions(int playerNumber);

	/**
	Helper for setupController. Sets up controller action names
	@param playerNumber Player number, starts at zero
	*/
	virtual void setupControllerActions(int playerNumber);

	/**
	Helper for setupController.
	Sets up controller bindings once action names are defined
	@param playerNumber Player number, starts at zero
	*/
	virtual void setupControllerBindings(int playerNumber);

	/**
	Polls the world to determine whether a USE action is appropriate.
	If so, then the action may be taken.
	@return true if a USE action was taken, false otherwise
	*/
	virtual bool doUseAction(void);

	/**
	Tests for collisions between this object and all objects in the given actor set
	@return list of all objects this object is colliding with
	*/
	virtual list<Actor*> getCollisions(const ActorSet &set) const;

private:
	/** Player number starts at zero */
	int playerNumber;

	/**
	Determines whether all players in the given zone are dead
	@param zone The zone to examine
	@return true if all players in the specified zone are dead
	*/
	static bool allPlayersAreDead(World &zone);

	/** Enters the game menu screen */
	void enterGameOverScreen(void);
};

} // namespace Engine

#endif
