/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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
#include "gl.h"
#include "GameStateMenu.h"
#include "Dimmer.h"
#include "LinearInterpolator.h"
#include "MenuGameOver.h"
#include "MenuOptions.h"
#include "MenuWorldSelection.h"
#include "MenuWorldLeadOut.h"

namespace Engine {

GameStateMenu::GameStateMenu(Application &app)
: GameState(app)
{
	TRACE("Creating the various pages of the menu...");

	gameMenu = new Menu;
	TRACE("Created gameMenu");

	gameOverMenu = new MenuGameOver;
	TRACE("Created gameOverMenu");

	gameMenuOptions = new MenuOptions;
	TRACE("Created gameMenuOptions");

	gameMenuWorldSelection = new MenuWorldSelection;
	TRACE("Created gameMenuWorldSelection");

	gameMenuWorldLeadOut[0] = new MenuWorldLeadOut("data/sprites/backdrops/world1-leadout.jpg");
	TRACE("Created gameMenuWorldLeadOut[0]");

	gameMenuWorldLeadOut[1] = new MenuWorldLeadOut("data/sprites/backdrops/world2-leadout.jpg");
	TRACE("Created gameMenuWorldLeadOut[1]");

	gameMenuWorldLeadOut[2] = new MenuWorldLeadOut("data/sprites/backdrops/world3-leadout.jpg");
	TRACE("Created gameMenuWorldLeadOut[2]");

	menu = gameMenu; // select the default menu

	TRACE("...finished");
}

GameStateMenu::~GameStateMenu(void)
{
	delete gameMenu;
	delete gameOverMenu;
	delete gameMenuOptions;
	delete gameMenuWorldSelection;
	delete gameMenuWorldLeadOut[0];
	delete gameMenuWorldLeadOut[1];
	delete gameMenuWorldLeadOut[2];
}

void GameStateMenu::update(float)
{
	ASSERT(menu!=0, "Menu does not exist");

	menu->update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	menu->draw();
}

void GameStateMenu::onEnter(void)
{
	g_SoundSystem.playMusic(application.menuMusic);
	menu->populateElements();
}

void GameStateMenu::onExit(void)
{
	// Fade in the screen
	application.addTask(new LinearInterpolator(&Dimmer::alphaBlur, 1.0f, 0.0f, 333));
}

void GameStateMenu::release(void)
{}

void GameStateMenu::reaquire(void)
{}

void GameStateMenu::enterGameOverScreen(void)
{
	menu = gameOverMenu;
}

void GameStateMenu::enterGameMenuScreen(void)
{
	menu = gameMenu;
}

void GameStateMenu::enterGameMenuOptions(void)
{
	menu = gameMenuOptions;
}

void GameStateMenu::enterGameMenuWorldSelection(void)
{
	menu = gameMenuWorldSelection;
}

void GameStateMenu::enterGameMenuWorldLeadOut(int worldNum)
{
	ASSERT(worldNum>=1 && worldNum<=3, "Parameter \'worldNum\' is invalid");
	menu = gameMenuWorldLeadOut[worldNum-1];
}

void GameStateMenu::repopulateMenu(void)
{
	TRACE("Repopulating elements of the menu...");

	ASSERT(0 != gameMenu,			"gameMenu was null");
	ASSERT(0 != gameOverMenu,		"gameOverMenu was null");
	ASSERT(0 != gameMenuOptions,		"gameMenuOptions was null");
	ASSERT(0 != gameMenuWorldSelection,	"gameMenuWorldSelection was null");
	ASSERT(0 != gameMenuWorldLeadOut[0],	"gameMenuWorldLeadOut[0] was null");
	ASSERT(0 != gameMenuWorldLeadOut[1],	"gameMenuWorldLeadOut[1] was null");
	ASSERT(0 != gameMenuWorldLeadOut[2],	"gameMenuWorldLeadOut[2] was null");

	gameMenu->populateElements();
	gameOverMenu->populateElements();
	gameMenuOptions->populateElements();
	gameMenuWorldSelection->populateElements();
	gameMenuWorldLeadOut[0]->populateElements();
	gameMenuWorldLeadOut[1]->populateElements();
	gameMenuWorldLeadOut[2]->populateElements();

	TRACE("...finished");
}

} // namespace Engine
