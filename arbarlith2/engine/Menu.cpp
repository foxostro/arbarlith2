/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2005-2007 Game Creation Society
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
#include "LinearInterpolator.h"
#include "gl.h"
#include "SDLwindow.h"
#include "WaitScreen.h"
#include "Dimmer.h"
#include "Menu.h"
#include "GameStateMenu.h"
#include "LightManager.h"
#include "World.h"

namespace Engine { 

Menu::Menu(void)
{
	clear();
	create(_T("data/sprites/backdrops/menu.jpg"));
}

Menu::Menu(const _tstring &imageFileName)
{
	clear();
	create(imageFileName);
}

Menu::~Menu(void)
{
	destroy();
}

void Menu::clear(void)
{
	selectedIndex = 0;
	debounceEnter = false;
	debounceUp = false;
	debounceDown = false;
	elements.clear();
	menuBackdrop.clear();
	font = 0;
}

void Menu::create(const _tstring &imageFileName)
{
	destroy();

	font = &g_Application.fontLarge;

	menuBackdrop.create(_T(""), imageFileName, _T(""), _T(""));
	
	// Populate the menu elements
	populateElements();
}

void Menu::destroy(void)
{
	menuBackdrop.destroy();

	clear();
}

void Menu::draw(void)
{
	effect_Begin(effect_GUI);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

	// Save the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Set up the projection matrix for 2D
	glLoadIdentity();
	glOrtho(0.0f, 1024.0f, 0.0f, 768.0f, -1.0f, 1.0f);

	// Save the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set up the model view matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -0.2f);

	// Render a textured quad over the screen
	menuBackdrop.getTexture().bind();
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glColor4fv(white);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1024.0f,   0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1024.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,   0.0f, 0.0f);
	glEnd();
	
	for(size_t i=0; i<elements.size(); ++i)
	{
		Element &item = elements[i];
		const _tstring &label = item.getLabel();
		bool enabled = item.isEnabled();
		bool selected = (size_t)selectedIndex==i;
		COLOR color = enabled ? (((size_t)selectedIndex==i) ? yellow : white) : (selected ? (yellow*0.7f) : (white*0.7f));

		glPushMatrix();
			glLoadIdentity();
			glTranslatef(item.getPosition().x, item.getPosition().y, 0.1f);
			font->write(label, color, FONT_SIZE_HUGE);
		glPopMatrix();
	}	
	
	// Restore the model view matrix
	glPopMatrix();

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Use modelview mode
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();

	effect_End();
}

void Menu::populateElements(void)
{
	elements.clear();
	elements.push_back( Element(vec2(200.0f, 600.0f), _T("New Game"), true)                          );
	elements.push_back( Element(vec2(200.0f, 500.0f), _T("Continue"), g_Application.isWorldLoaded()) );
	elements.push_back( Element(vec2(200.0f, 400.0f), _T("Options"),  true)                          );
	elements.push_back( Element(vec2(200.0f, 300.0f), _T("Credits"),  true)                          );
	elements.push_back( Element(vec2(200.0f, 200.0f), _T("Quit"),     true)                          );
}

void Menu::activateElement(int selectedIndex)
{
	const _tstring gameFileName = pathAppend(getAppDataDirectory(), _T("game.sav"));

	switch(selectedIndex)
	{
	case 0:
		// Go to the world selection screen
		g_SoundSystem.play(_T("data/sound/activate.wav"));
		GameStateMenu::GetSingleton().enterGameMenuWorldSelection();
		break;
	
	case 1:
		g_SoundSystem.play(_T("data/sound/activate.wav"));
		g_Application.changeGameState(GAME_STATE_RUN);
		break;
	
	case 2:
		// Go to the options screen
		g_SoundSystem.play(_T("data/sound/activate.wav"));
		GameStateMenu::GetSingleton().enterGameMenuOptions();
		break;
	
	case 3:
		g_SoundSystem.play(_T("data/sound/activate.wav"));
		g_Application.changeGameState(GAME_STATE_CREDITS);
		break;
	
	case 4:
		g_SoundSystem.play(_T("data/sound/activate.wav"));
		g_WaitScreen.Render();
		g_Input.Quit = true;
		break;
	};


	populateElements();
}

void Menu::update(void)
{
	if(g_Keys.isKeyDown(KEY_ENTER))
	{
		if(!debounceEnter)
		{
			debounceEnter = true;

			if(elements[selectedIndex].isEnabled())
			{
				activateElement(selectedIndex);
			}
			else
			{
				g_SoundSystem.play(_T("data/sound/disabled.wav"));
			}
		}
	}
	else
	{
		debounceEnter = false;
	}

	if(g_Keys.isKeyDown(KEY_MENU_LEFT) || g_Keys.isKeyDown(KEY_MENU_UP))
	{
		if(!debounceUp)
		{
			debounceUp = true;

			selectedIndex--;
			selectedIndex = selectedIndex < 0 ? (signed int)elements.size()-1 : selectedIndex;
			
			g_SoundSystem.play(_T("data/sound/down.wav"));
		}
	}
	else
	{
		debounceUp = false;
	}
	
	if(g_Keys.isKeyDown(KEY_MENU_RIGHT) || g_Keys.isKeyDown(KEY_MENU_DOWN))
	{
		if(!debounceDown)
		{
			debounceDown = true;

			selectedIndex++;
			selectedIndex = selectedIndex >= (signed int)elements.size() ? 0 : selectedIndex;

			g_SoundSystem.play(_T("data/sound/up.wav"));
		}
	}
	else
	{
		debounceDown = false;
	}
}


}; // namespace

