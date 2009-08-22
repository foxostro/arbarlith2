/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2005-2007,2009 Game Creation Society
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
#include "SDLwindow.h"
#include "Application.h"
#include "GameStateMenu.h"
#include "MenuOptions.h"

namespace Engine {

MenuOptions::MenuOptions(void)
{
	create();
}

MenuOptions::~MenuOptions(void)
{
	destroy();
}

void MenuOptions::create(void)
{
	Menu::create("data/sprites/backdrops/menu.jpg");

	debounceEnter = true;
}

void MenuOptions::populateElements(void)
{
	elements.clear();

	for(int i=0; i<MENU_OPTIONS_MAX; ++i)
	{
		switch(i)
		{
		case MENU_OPTIONS_RETURN:
			elements.push_back( Element(vec2(200.0f, 600.0f), "Return") );
			break;

		case MENU_OPTIONS_SHADOW:
			switch(g_Application.graphicsMode)
			{
			case Application::SHADOWS_AND_LIGHTING_ENABLED:		elements.push_back(  Element(vec2(200.0f, 500.0f), "Shadows are On")  ); break;
			case Application::LIGHTING_ENABLED:			elements.push_back(  Element(vec2(200.0f, 500.0f), "Lighting is On")  ); break;
			case Application::SHADOWS_AND_LIGHTING_DISABLED:	elements.push_back(  Element(vec2(200.0f, 500.0f), "Lighting is Off")  ); break;
			};
			break;

		case MENU_OPTIONS_MUTE_SOUND:
			elements.push_back(  Element(vec2(200.0f, 400.0f), g_Application.soundEnabled ? "Sound is ENABLED" : "Sound is MUTED")  );
			break;

		case MENU_OPTIONS_RES:
			elements.push_back(  Element(vec2(200.0f, 300.0f), itoa((int)g_Window.GetWidth()) + "x" + itoa((int)g_Window.GetHeight()))  );
			break;

		case MENU_OPTIONS_FS:
			elements.push_back(  Element(vec2(200.0f, 200.0f), g_Window.GetFullscreen() ? "Exit Fullscreen" : "Enter Fullscreen")  );
			break;
		}
	}

	// Save the setings
	g_Application.saveXmlConfigFiles();
}

void MenuOptions::activateElement(int selectedIndex)
{
	switch(selectedIndex)
	{
	case MENU_OPTIONS_RETURN:
		g_SoundSystem.play("data/sound/activate.wav");
		GameStateMenu::GetSingleton().enterGameMenuScreen();
		break;

	case MENU_OPTIONS_SHADOW:
		g_SoundSystem.play("data/sound/activate.wav");
		switch(g_Application.graphicsMode)
		{
		case Application::SHADOWS_AND_LIGHTING_ENABLED:		g_Application.graphicsMode = Application::LIGHTING_ENABLED; break;
		case Application::LIGHTING_ENABLED:			g_Application.graphicsMode = Application::SHADOWS_AND_LIGHTING_DISABLED; break;
		case Application::SHADOWS_AND_LIGHTING_DISABLED:	g_Application.graphicsMode = Application::LIGHTING_ENABLED; break;
		};
		break;

	case MENU_OPTIONS_MUTE_SOUND:
		if(!g_Application.soundEnabled)
		{
			g_Application.soundEnabled = true;
			g_SoundSystem.play("data/sound/activate.wav");
			g_SoundSystem.playMusic(g_Application.menuMusic);
		}
		else
		{
			g_SoundSystem.stopAll();
			g_Application.soundEnabled = false;
		}
		break;

	case MENU_OPTIONS_FS:
		g_SoundSystem.play("data/sound/activate.wav");

		if(g_Window.GetFullscreen())
		{
			g_Window.SetFullscreen(false);
			SDL_ShowCursor(1); // Show the mouse cursor
		}
		else
		{
			g_Window.SetFullscreen(true);
			SDL_ShowCursor(0); // Hide the mouse cursor
		}

		break;

	case MENU_OPTIONS_RES:
		{
			g_SoundSystem.play("data/sound/activate.wav");

			switch(g_Window.GetWidth())
			{
			case 640:	g_Window.Resize(800, 600);	break;
			case 800:	g_Window.Resize(1024, 768);	break;
			case 1024:	g_Window.Resize(640, 480);	break;
			};
		}
		break;
	}

	populateElements();
}


}; // namespace
