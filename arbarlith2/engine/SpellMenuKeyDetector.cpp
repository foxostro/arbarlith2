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
#include "SpellMenuKeyDetector.h"

namespace Engine { 

SpellMenuKeyDetector::SpellMenuKeyDetector(void)
:menuToggleKey(INVALID_ACTION_CODE),
debounce(false)
{
	menuToggleKey = g_Keys.createAction(_T("Spell-Menu"));

	for(int i=0; i<3; ++i)
	{
		g_Keys.addBinding(menuToggleKey, _T("JOY") + Engine::itoa(i+1) + _T("_BUTT_5"));
	}
	
	g_Keys.addBinding(menuToggleKey, _T("L_Control"));
}

void SpellMenuKeyDetector::update(float)
{
	if(g_Keys.isKeyDown(menuToggleKey) )
	{
		if(debounce == false)
		{
			debounce = true;

			toggleMenu();
		}
	}
	else
	{
		debounce = false;
	}
}

void SpellMenuKeyDetector::toggleMenu(void)
{
	if(g_Application.getState() == GAME_STATE_SPELL_MENU)
	{
		g_Application.changeGameState(GAME_STATE_RUN);
	}
	else
	{
		g_Application.changeGameState(GAME_STATE_SPELL_MENU);
	}
}

}; //namespace
