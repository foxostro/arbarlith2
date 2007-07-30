/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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
#include "Application.h"
#include "world.h"
#include "SDLwindow.h"
#include "windowwidget.h"

namespace Engine {

WindowWidget::WindowWidget(float x, float y, const _tstring &imageFile)
{
	setRelativePos(x,y);
	depressed = false;
	hover = false;
	mouseDown = false;

	TextureHandle *handle = g_TextureMgr.Load(imageFile);
	matRegular.setTexture(handle, 0);
	setWidth(unstretchedDim.x = (float)handle->getWidth());
	setHeight(unstretchedDim.y = (float)handle->getHeight());

	matDepressed.loadTexture(imageFile, 0);
	matHover.loadTexture(imageFile, 0);
}

WindowWidget::WindowWidget(float x, float y, const _tstring &imageFile, const _tstring &depressedFile)
{
	setRelativePos(x,y);
	depressed = false;
	hover = false;
	mouseDown = false;

	TextureHandle *handle = g_TextureMgr.Load(imageFile);
	matRegular.setTexture(handle, 0);
	setWidth(unstretchedDim.x = (float)handle->getWidth());
	setHeight(unstretchedDim.y = (float)handle->getHeight());

	matDepressed.loadTexture(depressedFile, 0);
	matHover.loadTexture(imageFile, 0);
}

WindowWidget::WindowWidget(float x, float y, const _tstring &imageFile, const _tstring &depressedFile, const _tstring &hoverFile)
{
	setRelativePos(x,y);
	depressed = false;
	hover = false;
	mouseDown = false;

	TextureHandle *handle = g_TextureMgr.Load(imageFile);
	matRegular.setTexture(handle, 0);
	setWidth(unstretchedDim.x = (float)handle->getWidth());
	setHeight(unstretchedDim.y = (float)handle->getHeight());

	matDepressed.loadTexture(depressedFile, 0);
	matHover.loadTexture(hoverFile, 0);
}

WindowWidget::~WindowWidget(void)
{
	matHover.destroy();
	matDepressed.destroy();
	matRegular.destroy();
}

void WindowWidget::draw(void) const
{
	glColor4fv(white);
	glEnable(GL_TEXTURE_2D);
	getMaterial().bind();

	if(stretchToFit)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(getWidth(),  getHeight(),  0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,        getHeight(),  0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f,        0.0f,         0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(getWidth(),  0.0f,         0.0f);
		glEnd();
	}
	else
	{
		float cropX = getWidth() / unstretchedDim.x;
		float cropY = getHeight() / unstretchedDim.y;

		glBegin(GL_QUADS);
			glTexCoord2f(cropX, cropY); glVertex3f(getWidth(),  getHeight(),  0.0f);
			glTexCoord2f(0.0f,  cropY); glVertex3f(0.0f,        getHeight(),  0.0f);
			glTexCoord2f(0.0f,   0.0f); glVertex3f(0.0f,        0.0f,         0.0f);
			glTexCoord2f(cropX,  0.0f); glVertex3f(getWidth(),  0.0f,         0.0f);
		glEnd();
	}

	glColor4fv(white);
}

const Material& WindowWidget::getMaterial(void) const
{
	if(hover)
	{
		if(depressed)	return matDepressed;
		else			return matHover;
	}
	else
	{
		return matRegular;
	}
}

}; // namespace
