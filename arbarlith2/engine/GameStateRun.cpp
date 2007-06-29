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
#include "Zone.h"
#include "Player.h"
#include "Dimmer.h"
#include "LinearInterpolator.h"
#include "GameStateRun.h"

namespace Engine { 
	
GameStateRun::GameStateRun(void)
{
	// add a few widgets to the GUI
	g_GUI.addWidget(triggerPrompt = new TriggerPrompt());
	g_GUI.addWidget(performanceLabel = new PerformanceLabel(60, 650));
	g_GUI.addWidget(debugLabel = new DebugLabel(60, 600));
}

GameStateRun::~GameStateRun(void)
{}

void GameStateRun::update(float deltaTime)
{
	if(!World::GetSingletonPtr())
		g_Application.changeGameState(GAME_STATE_MENU);
	else
	{
		ASSERT(0!=performanceLabel, _T("performanceLabel was null"));
		ASSERT(0!=debugLabel, _T("debugLabel was null"));
		
		performanceLabel->m_bVisible = g_Application.displayFPS;
		debugLabel->m_bVisible = g_Application.displayDebugData;

		// update
		g_World.update(deltaTime);
		g_GUI.update(deltaTime);
		
		// draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		g_World.getPlayer().getZone().draw();
		dim.draw();
		g_GUI.draw();

		// Draw the blur over top
		if(g_Application.useBlurEffects)
		{
			blurEffect.update(deltaTime);
			blurEffect.draw();
		}
	}
}

void GameStateRun::onEnter(void)
{}

void GameStateRun::onExit(void)
{}

void GameStateRun::release(void)
{
	blurEffect.release();
}

void GameStateRun::reaquire(void)
{
	blurEffect.reaquire();
}

}; // namespace
