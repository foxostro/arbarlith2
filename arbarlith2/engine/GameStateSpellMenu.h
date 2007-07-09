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

#ifndef _GAME_STATE_SPELL_MENU_H_
#define _GAME_STATE_SPELL_MENU_H_

#include "Singleton.h"
#include "Dimmer.h"
#include "GameState.h"
#include "Material.h"

#include "../MyPlayer.h"
#include "../Spell.h"

namespace Engine {

/** State of the game where the spell selection menu is displayed */
class GameStateSpellMenu : public GameState, public Singleton<GameStateSpellMenu>
{
public:
	/** Constructor */
	GameStateSpellMenu(void);

	/** Destructor */
	virtual ~GameStateSpellMenu(void);

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	virtual void update(float deltaTime);

	/** Called when the state is entered */
	virtual void onEnter(void);

	/** Called when the state is exitted */
	virtual void onExit(void);

	/** Release assets */
	virtual void release(void);

	/** Reaquire assets */
	virtual void reaquire(void);

	/** Padlock texture */
	Material padlock;

private:
	/** Screen Dimmer */
	Dimmer dim;

	/** Intensity of the screen while in this game state */
	float dimness;

	/** left key debounce */
	bool leftDebounce[MAX_PLAYERS];

	/** right key debounce */
	bool rightDebounce[MAX_PLAYERS];

	/**
	Updates the menu from input from the given player
	@param playerNumber player we are updating for
	*/
	void updateForPlayer(size_t playerNumber);

	/**
	Draws the menu for the given player
	@param player player we are drawing for
	@param cx X position of the center of the wheel
	@param cy Y position of the center of the wheel
	@param radius Radius of the wheel
	*/
	void drawForPlayer(Arbarlith2::MyPlayer &player, float cx, float cy, float radius) const;

	/**
	Rotates the menu clockwise
	@param spellList Lists spells the player has access to
	@param activeIdx Active spell's index in the spell list
	*/
	void onRotateClockwise(vector<Arbarlith2::Spell*> spellList, int &activeIdx);

	/**
	Rotates the menu counter clockwise
	@param spellList Lists spells the player has access to
	@param activeIdx Active spell's index in the spell list
	*/
	void onRotateCounterClockwise(vector<Arbarlith2::Spell*> spellList, int &activeIdx);
};

} // namespace Engine

#endif
