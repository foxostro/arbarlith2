/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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
#include "Boss.h"
#include "engine/CallbackInterpolator.h"
#include "engine/Dimmer.h"
#include "engine/GameStateMenu.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(Boss, "class Arbarlith2::Boss")

Boss::Boss(OBJECT_ID ID)
: Monster(ID)
{
	clear();
}

void Boss::OnDeath(void)
{
	Monster::OnDeath();

	const string &zoneName = toLowerCase(getZone().getName());

	// Unlock the next world
	if(zoneName == "world2")
	{
		g_Application.unlockWorld(3);
		g_Application.saveXmlConfigFiles();
	}
	else if(zoneName == "world1")
	{
		g_Application.unlockWorld(2);
		g_Application.saveXmlConfigFiles();
	}

	// Dim the screen and Return to the menu after a short delay
	float delay = 4000.0f;
	boost::function<void (void)> fn = boost::bind(&Boss::returnToMenu, this);
	Task *task = Engine::makeCallbackInterpolator(&Dimmer::alphaBlur, 0.0f, 1.0f, delay, fn);
	g_Application.addTask(task);
}

void Boss::returnToMenu(void)
{
	const string &zoneName = toLowerCase(getZone().getName());

	// Enter the lead-out menu
	     if(zoneName == "world3")
		GameStateMenu::GetSingleton().enterGameMenuWorldLeadOut(3);
	else if(zoneName == "world2")
		GameStateMenu::GetSingleton().enterGameMenuWorldLeadOut(2);
	else if(zoneName == "world1")
		GameStateMenu::GetSingleton().enterGameMenuWorldLeadOut(1);
	else
	{
		FAIL("There shouldn't be a boss present in a world unless that world is named \'world1\' or \'world2\' or \'world3\'");
		GameStateMenu::GetSingleton().enterGameMenuWorldSelection();
	}

	g_Application.changeGameState(GAME_STATE_MENU);
}

} // namespace Arbarlith2
