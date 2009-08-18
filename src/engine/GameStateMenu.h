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

#ifndef _GAME_STATE_MENU_H_
#define _GAME_STATE_MENU_H_

#include "singleton.h"
#include "Menu.h"
#include "GameState.h"

namespace Engine {

/** State of the game where the menu is displayed */
class GameStateMenu : public GameState, public Singleton<GameStateMenu>
{
public:
	/**
	Constructor
	@param app Game engine kernel. The application framework that is running the game.
	*/
	GameStateMenu(Application &app);

	/** Destructor */
	virtual ~GameStateMenu(void);

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

	/** Enters the game over screen */
	virtual void enterGameOverScreen(void);

	/** Enter the game menu screen */
	virtual void enterGameMenuScreen(void);

	/** Enter the game menu options */
	virtual void enterGameMenuOptions(void);

	/** Enter the game world selection screen */
	virtual void enterGameMenuWorldSelection(void);

	/**
	Enter the game world's lead-out screen
	@param worldNum The world number that we are leading out of.  Starts counting at 1 with world #1, not zero.
	*/
	virtual void enterGameMenuWorldLeadOut(int worldNum);

	/**
	Instructs the game menus to reconstruct themselves.
	Useful for circumstances where data is changed and menu options have become stale
	*/
	virtual void repopulateMenu(void);

private:
	/** The current menus */
	Menu *menu;

	/** The paused game menu */
	Menu *gameMenu;

	/** The game options menu */
	Menu *gameMenuOptions;

	/** The game over menu */
	Menu *gameOverMenu;

	/** The game world selection menu */
	Menu *gameMenuWorldSelection;

	/** The game world lead-out menu */
	Menu *gameMenuWorldLeadOut[3];
};

} // namespace Engine

#endif
