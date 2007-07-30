/*
Original Author: Andrew Fox
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
#include "gl.h"
#include "SDLwindow.h"
#include "EffectManager.h"
#include "world.h"
#include "Blur.h"



namespace Engine {




Blur::Blur(int width, int height)
{
	clear();
	create(width, height);
}

Blur::~Blur(void)
{
	destroy();
}

void Blur::clear(void)
{
	update = false;
	scene = 0;
	alphaBlur = 0.0f;
	BLUR_TEXTURE_WIDTH = 0;
	BLUR_TEXTURE_HEIGHT = 0;
	alphaBlur = 1.0f;
	zone = 0;
}

void Blur::release(void)
{
	glDeleteTextures(1, &scene);
	scene=0;
}

void Blur::reaquire(void)
{
	release();

	// Create the capture textures
	glGenTextures(1, &scene);
	glBindTexture(GL_TEXTURE_2D, scene);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, BLUR_TEXTURE_WIDTH, BLUR_TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Blur::create(int width, int height)
{
	destroy();

	BLUR_TEXTURE_WIDTH = width;
	BLUR_TEXTURE_HEIGHT = height;

	reaquire();
}

void Blur::destroy(void)
{
	release();
	clear();
}

void Blur::capture(void)
{
	if(update)
	{
		update = false;

		// Render the scene in the correct resolution
		glViewport(0, 0, BLUR_TEXTURE_WIDTH, BLUR_TEXTURE_HEIGHT);
		zone->draw();

		// Read the frame buffer into a texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, scene);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, BLUR_TEXTURE_WIDTH, BLUR_TEXTURE_HEIGHT);

		// Reset the scene
		glViewport(0, 0, SDLWindow::GetSingleton().GetWidth(), SDLWindow::GetSingleton().GetHeight());
	}
}

void Blur::draw(void)
{
	if(alphaBlur > FLT_EPSILON)
	{
		glPushAttrib(GL_ENABLE_BIT);

		glColor4f(1, 0, 0, alphaBlur);
		glDisable(GL_LIGHTING);
		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, scene);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1024.0f,   0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1024.0f, 768.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, 768.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,   0.0f, 0.0f);
		glEnd();

		// Restore the model view matrix
		glPopMatrix();

		// Restore the projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		// Use modelview mode
		glMatrixMode(GL_MODELVIEW);

		// Restore settings
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, 1);
		glPopAttrib();
	}
	else
	{
		alphaBlur = 0.0f;
	}
}

}; // namespace
