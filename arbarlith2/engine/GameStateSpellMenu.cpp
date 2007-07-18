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

#include "gl.h"
#include "World.h"
#include "PerformanceLabel.h"
#include "Dimmer.h"
#include "LinearInterpolator.h"
#include "GameStateSpellMenu.h"

namespace Engine {

vec2 Project(const vec3 &p); // stdafx.cpp

GameStateSpellMenu::GameStateSpellMenu(Application &app)
: GameState(app),
  dimness(0.7f)
{
	TRACE(_T("Constructing GameStateSpellMenu... "));

	padlock.loadTexture(_T("data/sprites/padlock.png"), 0);

	for(int i=0; i<MAX_PLAYERS; ++i)
	{
		leftDebounce[i] = false;
		rightDebounce[i] = false;
	}

	TRACE(_T("...finished (Constructing GameStateSpellMenu)"));
}

GameStateSpellMenu::~GameStateSpellMenu(void)
{}

void GameStateSpellMenu::update(float)
{
	if(!application.isWorldLoaded())
	{
		application.changeGameState(GAME_STATE_MENU);
		return;
	}

	World &world = application.getWorld();

	const size_t numOfPlayers = world.getNumOfPlayers();

	for(size_t i=0; i<numOfPlayers; ++i)
	{
		updateForPlayer(i);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	world.draw();
	dim.draw();

	effect_Begin(effect_GUI);

		if(numOfPlayers==1)
		{
			Arbarlith2::MyPlayer &player = dynamic_cast<Arbarlith2::MyPlayer&>(world.getPlayer(0));
			drawForPlayer(player, 512, 384, 250.0f);
		}
		else
		{
			const float radius = 100.0f;

			for(size_t i=0; i<numOfPlayers; ++i)
			{
				Arbarlith2::MyPlayer &player = dynamic_cast<Arbarlith2::MyPlayer&>(world.getPlayer(i));

				vec2 win = Project(player.getPos());

				drawForPlayer(player, win.x, win.y, radius);
			}
		}
	effect_End();
}

void GameStateSpellMenu::updateForPlayer(size_t playerNumber)
{
	ASSERT(playerNumber < MAX_PLAYERS, _T("invalid player number"));

	Arbarlith2::MyPlayer &player = dynamic_cast<Arbarlith2::MyPlayer&>(application.getWorld().getPlayer(playerNumber));

	vector<Arbarlith2::Spell*> spellList = player.getSpellList();
	int activeIdx = player.getActiveSpell();

	// get user input
	if(g_Keys.isKeyDown(player.KEY_PLAYER_WALK_LEFT))
	{
		if(leftDebounce[playerNumber] == false)
		{
			leftDebounce[playerNumber] = true;
			onRotateClockwise(spellList, activeIdx);
		}
	}
	else
	{
		leftDebounce[playerNumber] = false;
	}


	if(g_Keys.isKeyDown(player.KEY_PLAYER_WALK_RIGHT))
	{
		if(rightDebounce[playerNumber] == false)
		{
			rightDebounce[playerNumber] = true;
			onRotateCounterClockwise(spellList, activeIdx);
		}
	}
	else
	{
		rightDebounce[playerNumber] = false;
	}

	player.setActiveSpell(activeIdx);
}

void GameStateSpellMenu::onRotateClockwise(vector<Arbarlith2::Spell*> spellList, int &activeIdx)
{
	activeIdx++;
	if((size_t)activeIdx >= spellList.size()) activeIdx = 0;
	g_SoundSystem.play(_T("data/sound/up.wav"));
}

void GameStateSpellMenu::onRotateCounterClockwise(vector<Arbarlith2::Spell*> spellList, int &activeIdx)
{
	activeIdx--;
	if(activeIdx < 0) activeIdx = (int)spellList.size()-1;
	g_SoundSystem.play(_T("data/sound/down.wav"));
}

void GameStateSpellMenu::drawForPlayer(Arbarlith2::MyPlayer &player, float cx, float cy, float radius) const
{
	vector<Arbarlith2::Spell*> spellList = player.getSpellList();

	for(size_t i=0; i < spellList.size(); ++i)
	{
		Arbarlith2::Spell &spell = *spellList[i];

		float angle = 2 * (float)M_PI * ((float)i / spellList.size());
		float x = cx + radius*cosf(angle);
		float y = cy + radius*sinf(angle);

		ASSERT(player.getActiveSpell()>=0, _T("player.getActiveSpell() was negative, which really doesn't make much sense: ") + itoa(player.getActiveSpell()));

		spell.drawIcon2D(i==(size_t)player.getActiveSpell(), x, y, (80.0f/250.0f) * radius);
	}

	const Arbarlith2::Spell &activeSpell = *spellList[player.getActiveSpell()];
	if(activeSpell.available)
	{
		glPushMatrix();
		glTranslatef(cx - radius, cy + radius + 50, 0);
		application.fontLarge.write(activeSpell.getDescriptionText(), white, FONT_SIZE_NORMAL);
		glPopMatrix();
	}
}

void GameStateSpellMenu::onEnter(void)
{
	application.addTask(new LinearInterpolator(&Dimmer::alphaBlur, 0.0f, dimness, 333));
	g_SoundSystem.play(_T("data/sound/activate.wav"));
}

void GameStateSpellMenu::onExit(void)
{
	application.addTask(new LinearInterpolator(&Dimmer::alphaBlur, dimness, 0.0f, 333));
	g_SoundSystem.play(_T("data/sound/disabled.wav"));
}

void GameStateSpellMenu::release(void)
{}

void GameStateSpellMenu::reaquire(void)
{}

} // namespace Engine
