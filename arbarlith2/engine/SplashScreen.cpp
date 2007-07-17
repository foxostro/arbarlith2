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
#include "SDLwindow.h"
#include "Image.h"
#include "TextureHandle.h"
#include "SplashScreen.h"

namespace Engine {

SplashScreen::SplashScreen(void)
{
	const _tstring nill = _T("");
	const _tstring splashScreenImage = _T("data/sprites/backdrops/splash.png");

	splash.create(nill,
	              splashScreenImage,
	              nill,
	              nill);
}

SplashScreen::~SplashScreen(void)
{}

void SplashScreen::draw(float intensity)
{
	CHECK_GL_ERROR();

	// reset for the following frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	effect_Begin(effect_GUI);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// Set up the model view matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.0f);

	// Render a textured quad over the screen
	splash.getTexture().bind();
	glDisable(GL_ALPHA_TEST);
	glColor4fv(COLOR(intensity, intensity, intensity, 1.0f));
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1024.0f,   0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1024.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,   0.0f, 0.0f);
	glEnd();

	glPopAttrib();
	effect_End();

	// Swap buffers
	glFlush();

	g_Window.Flip();


// Message Pumping
	g_Input.Pump();

	// check for sizing
	if(g_Input.ResizePending())
	{
		int width = g_Input.ResizeWidth(),
			height = g_Input.ResizeHeight(); // screen dimensions

		// Resize the window
		g_Window.Resize(width,height);
		OpenGL::GetSingleton().ReSizeGLScene(width,height);
	}

	CHECK_GL_ERROR();
}

void SplashScreen::doSplash(DWORD splashTime)
{
	doSplashConstant(splashTime-1000);
	doSplashFade();
}

void SplashScreen::doSplashConstant(DWORD splashTime)
{
	DWORD startTime = GetTickCount();
	DWORD time=0;

	do
	{
		time = GetTickCount();

		draw(1.0f);

	}while(time-startTime < splashTime && !g_Keys.isAnyKeyDown());
}

void SplashScreen::doSplashFade(void)
{
	DWORD startTime = GetTickCount();
	DWORD time=0;

	do
	{
		time = GetTickCount();

		const DWORD x = time-startTime;
		draw((float)pow(M_E, -SQR(0.002146 * x)));

	}while(time-startTime < 1000);
}

} // namespace Engine
