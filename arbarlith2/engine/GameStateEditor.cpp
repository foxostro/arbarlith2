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
#include "OpenGL.h"
#include "gl.h"
#include "World.h"
#include "Zone.h"
#include "Player.h"
#include "WaitScreen.h"
#include "GameStateEditor.h"

namespace Engine { 
	
GameStateEditor::GameStateEditor(void)
:editorToolBar(0)
{}

void GameStateEditor::update(float deltaTime)
{
	if(!World::GetSingletonPtr())
		g_Application.changeGameState(GAME_STATE_MENU);
	else
	{
		// update
		g_GUI.update(deltaTime);
		g_Camera.updateFlyingCamera(deltaTime);

		// draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(World::GetSingletonPtr()) World::GetSingleton().getPlayer().getZone().draw();
		editorToolBar->drawInWorldSpace();
		g_GUI.draw();
	}
}

void GameStateEditor::onEnter(void)
{
	TRACE(_T("Entering Editor Mode"));
	g_WaitScreen.Render();

	if(editorToolBar == 0)
	{
		// Create the editor tool bar
		editorToolBar = new EditorToolBar;
		ASSERT(editorToolBar!=0, _T("editor toolbar did not load correctly"));
		editorToolBar->create();
		g_GUI.addWidget(editorToolBar);
	}
	editorToolBar->m_bVisible = true;

	OpenGL::GetSingleton().SetClippingPlanes(0.01f, 1000.0f);

	// Reveal the mouse cursor
	SDL_ShowCursor(1);
}

void GameStateEditor::onExit(void)
{
	g_World.getPlayer().getZone().getMap().reaquire();

	// hide the editor toolbar
	editorToolBar->m_bVisible = false;
	editorToolBar->hideActorPane();

	if(g_Window.GetFullscreen())
	{
		// hide the mouse cursor
		SDL_ShowCursor(0);
	}
}

void GameStateEditor::release(void)
{}

void GameStateEditor::reaquire(void)
{}

}; // namespace