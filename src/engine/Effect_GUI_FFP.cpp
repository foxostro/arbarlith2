/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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
#include "PropertyBag.h"
#include "Effect_GUI_FFP.h"


namespace Engine {



Effect_GUI_FFP::Effect_GUI_FFP(void)
{
	clear();
}

Effect_GUI_FFP::Effect_GUI_FFP(const Effect_GUI_FFP &e)
:Effect(e)
	{}

void Effect_GUI_FFP::clear(void)
{
	Effect::clear();
}

void Effect_GUI_FFP::begin(void)
{
	glPushAttrib(GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	// Enable alpha test for widgets
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.10f);

	// Disable all texture units initially
	if(g_bUseMultitexture)
	{
		for(int i=0; i<g_MultitextureUnits; ++i)
		{
			glActiveTextureARB(textureStages[i]);
			glDisable(GL_TEXTURE_2D);
		}
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}

	// Select a white color by default
	glColor4fv(white);

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
}

void Effect_GUI_FFP::end(void)
{

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Restore the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPopAttrib();

	CHECK_GL_ERROR();
}

string Effect_GUI_FFP::getRequiredExtensions(void) const
{
	return string();
}

int Effect_GUI_FFP::getRequiredTextureUnits(void) const
{
	return 1; // only 1 texture unit required
}

void Effect_GUI_FFP::passTextureName(unsigned int textureName, unsigned int textureUnit)
{
	if(textureUnit == 0)
	{
		// Select the primary texture unit
		if(g_bUseMultitexture)
		{
			glActiveTextureARB(GL_TEXTURE0_ARB);
		}

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textureName);

		// Finally, ensure the texture is enabled
		glEnable(GL_TEXTURE_2D);
	}
}

void Effect_GUI_FFP::passVertexStream(float *)
{}

void Effect_GUI_FFP::passNormalStream(float *)
{}

void Effect_GUI_FFP::passTexCoordStream(float *, unsigned int)
{}

bool Effect_GUI_FFP::requiresVertexStream(void) const
{
	return false;
}

bool Effect_GUI_FFP::requiresNormalStream(void) const
{
	return false;
}

bool Effect_GUI_FFP::requiresTexCoordStream(void) const
{
	return false;
}

string Effect_GUI_FFP::getName(void) const
{
	return "Effect_GUI_FFP";
}


int Effect_GUI_FFP::howGood(effect_sig signature) const
{
	switch(signature)
	{
	case effect_GUI: return 100;
	default:		 return 0;
	};
}

}; // namespace
