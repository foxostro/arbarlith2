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
#include "WaitScreen.h"
#include "Application.h"
#include "world.h"
#include "GameStateMenu.h"
#include "MenuWorldSelection.h"

namespace Engine {

MenuWorldSelection::MenuWorldSelection(void)
{
	create();
}

MenuWorldSelection::~MenuWorldSelection(void)
{
	destroy();
}

void MenuWorldSelection::create(void)
{
	Menu::create("data/sprites/backdrops/menu.jpg");

	debounceEnter = true;
}

void MenuWorldSelection::populateElements(void)
{
	elements.clear();

	for(int i=0; i<MENU_WORLD_SELECTION_MAX; ++i)
	{
		switch(i)
		{
		case MENU_WORLD_SELECTION_RETURN:
			elements.push_back(  Element(vec2(200.0f, 600.0f), "Return")  );
			break;

		case MENU_WORLD_SELECTION_WORLD1: elements.push_back(getLabel(1)); break;
		case MENU_WORLD_SELECTION_WORLD2: elements.push_back(getLabel(2)); break;
		case MENU_WORLD_SELECTION_WORLD3: elements.push_back(getLabel(3)); break;
		}
	}
}

void MenuWorldSelection::activateElement(int selectedIndex)
{
	switch(selectedIndex)
	{
	case MENU_WORLD_SELECTION_RETURN:
		g_SoundSystem.play("data/sound/activate.wav");
		GameStateMenu::GetSingleton().enterGameMenuScreen();
		break;

	case MENU_WORLD_SELECTION_WORLD1: enterWorld(0); break;
	case MENU_WORLD_SELECTION_WORLD2: enterWorld(1); break;
	case MENU_WORLD_SELECTION_WORLD3: enterWorld(2); break;
	}

	populateElements();
}

Menu::Element MenuWorldSelection::getLabel(int worldNum)
{
	bool unlocked = worldNum<=g_Application.unlockedWorld;

	return Element(vec2(200.0f, 500.0f - (worldNum-1)*100.0f),
	               string((unlocked)?"World ":"[Locked] World ")+itoa(worldNum),
	               unlocked);
}

void MenuWorldSelection::enterWorld(int worldNum)
{
	g_SoundSystem.play("data/sound/activate.wav");
	g_Application.enterWorld(worldNum);
}

}; // namespace
