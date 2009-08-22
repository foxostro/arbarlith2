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
#include "world.h"
#include "player.h"
#include "PerformanceLabel.h"
#include "Dimmer.h"
#include "LinearInterpolator.h"
#include "GameStateCredits.h"

namespace Engine {

GameStateCredits::GameStateCredits(Application &app)
: GameState(app),
  dimness(0.4f),
  scrollSpeed(70.0f),
  y(600.0f)
{
	File file("credits.txt", false);

	if(!file.loaded())
	{
		FAIL("credits.txt failed to load");
	}

	const size_t size = file.getSize();

	text.resize(size);

	char *buffer = new char[size];

	// Copy file data
	memset(buffer, 0, size);
	file.read(buffer, size);
	text = buffer;

	TRACE(text);

	// Release the buffer
	delete [] buffer;
}

GameStateCredits::~GameStateCredits(void)
{}

void GameStateCredits::update(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(application.isWorldLoaded())
	{
		application.getWorld().draw();
		dim.draw();
	}

	y += scrollSpeed * (deltaTime/1000.0f); // scroll a bit

	effect_Begin(effect_GUI);

		glPushMatrix();
		glTranslatef(200.0f, y, 0.1f);
		g_Application.fontLarge.write(text, white, FONT_SIZE_HUGE);
		glPopMatrix();

	effect_End();
}

void GameStateCredits::onEnter(void)
{
	g_Application.addTask(new LinearInterpolator(&Dimmer::alphaBlur, dimness, 0.8f, 667));
	y = 500.0f;
}

void GameStateCredits::onExit(void)
{
	g_Application.addTask(new LinearInterpolator(&Dimmer::alphaBlur, 0.8f, dimness, 667));
}

void GameStateCredits::release(void)
{}

void GameStateCredits::reaquire(void)
{}

}; // namespace
