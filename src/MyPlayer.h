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

#ifndef _ARB2_PLAYER_H_
#define _ARB2_PLAYER_H_

#include "engine/world.h"
#include "engine/player.h"
#include "Spell.h"

using namespace Engine;

namespace Arbarlith2 {

class MyPlayer : public Player
{
public:
	GEN_RTTI(MyPlayer, "class Arbarlith2::MyPlayer")

public:
	/**
	Constructor
	@param ID The unique ID to assign to the Player
	*/
	MyPlayer(OBJECT_ID ID);

	/** Updates the player */
	virtual void update(float deltaTime);

	/** Set everything to defaults */
	virtual void clear(void);

	/** Destroys the player instance */
	virtual void destroy(void);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Gets the list of available spells
	@return the spells
	*/
	vector<Spell*> getSpellList(void) const
	{
		return spellList;
	}

	/**
	Gets the index of the active spell
	@return spell index
	*/
	int getActiveSpell(void) const
	{
		return activeIdx;
	}

	/**
	Sets the active spell
	@param x Active spell's index
	*/
	void setActiveSpell(int x)
	{
		ASSERT(x >= 0 && (size_t)x < spellList.size(), "Spell index was out of bounds");
		activeIdx = x;
	}

	/**
	Gets a spell from the spell list
	@param spellIdx Index of the spell
	@return the active spell
	*/
	Spell& getSpell(size_t spellIdx) const
	{
		ASSERT(spellIdx < spellList.size(), "Spell index was out of bounds");
		return *spellList[spellIdx];
	}

	/**
	Gets the active spell
	@return the active spell
	*/
	Spell& getSpell(void) const
	{
		return getSpell(activeIdx);
	}

	/**
	Indicates that the player has picked up the certain spell ability
	@param spellIdx Index of the spell
	@return true if the player has picked up the certain spell ability
	*/
	bool getCanCast(int spellIdx) const
	{
		return getSpell(spellIdx).available;
	}

	/**
	Enable or disable the spell
	@param spellIdx Spell index
	@param enable true if the spell is to be enabled
	*/
	void setCanCast(int spellIdx, bool enable = true)
	{
		getSpell(spellIdx).available = enable;
	}

	/** Transforms and renders the player */
	virtual void drawObject(void) const;

	/** Used by drawObject to render Debug data */
	virtual void drawObjectDebugData(void) const;

	/** Draws the count down for the current state */
	virtual void drawStateCountdown(void) const;

	/** Gets the name of the proper idle animation */
	virtual const string getSpellCastAnim(void) const;

	/** Perform collision response to the objects we detected interesection with */
	virtual void DoCollisionResponse(void);

	/**
	Called in the event that the player number is set or changed
	@param playerNumber Player number starts at zero
	*/
	virtual void OnChangePlayerNumber(int playerNumber);

protected:
	/** Action code for casting a player spell */
	ACTION_CODE KEY_PLAYER_CAST_SPELL;

	/**
	Determines whether the player can move
	@return the player can move
	*/
	virtual bool canMove(void) const;

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

private:
	/** The list of available spells*/
	vector<Spell*> spellList;

	/**
	The active spell:
		0 - Fireball
		1 - Arctic Wind
		2 - Incinerate
		3 - Chill
		4 - Heal
		5 - Ice Blast
	*/
	int activeIdx;

	/** debounce the spell cast key */
	bool spellCastDebounce;
};

} // namespace Arbarlith2

#endif
