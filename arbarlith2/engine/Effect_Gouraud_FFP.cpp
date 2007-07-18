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
#include "PropertyBag.h"
#include "Effect_Gouraud_FFP.h"


namespace Engine { 



Effect_Gouraud_FFP::Effect_Gouraud_FFP(void)
{
	clear();
}

Effect_Gouraud_FFP::Effect_Gouraud_FFP(const Effect_Gouraud_FFP &e)
:Effect(e)
{}

void Effect_Gouraud_FFP::clear(void)
{
	Effect::clear();
}

void Effect_Gouraud_FFP::begin(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor4fv(white);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	Effect::disableExtraTextureUnits();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void Effect_Gouraud_FFP::end(void)
{
	glPopAttrib();
	CHECK_GL_ERROR();
}

_tstring Effect_Gouraud_FFP::getRequiredExtensions(void) const
{
	return _tstring();
}

int Effect_Gouraud_FFP::getRequiredTextureUnits(void) const
{
	return 1; // only 1 texture unit required
}

void Effect_Gouraud_FFP::passTextureName(unsigned int textureName, unsigned int textureUnit)
{
	// Select the primary texture unit
	if(g_bUseMultitexture)
	{
		glActiveTextureARB(textureStages[textureUnit]);
		glEnable(GL_TEXTURE_2D);
	}

	// Set the texture filtering according to global performance settings
	Effect::setTextureFilters();

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureName);

	// Finally, ensure the texture is enabled
	glEnable(GL_TEXTURE_2D);
}

void Effect_Gouraud_FFP::passVertexStream(float *stream)
{
	glVertexPointer(3, GL_FLOAT, 0, stream);
}

void Effect_Gouraud_FFP::passNormalStream(float *stream)
{
	glNormalPointer(GL_FLOAT, 0, stream);
}

void Effect_Gouraud_FFP::passTexCoordStream(float *stream, unsigned int textureUnit)
{
	if(g_bUseMultitexture)
	{
		glClientActiveTextureARB(textureStages[textureUnit]);
	}

	glTexCoordPointer(2, GL_FLOAT, 0, stream);
}

bool Effect_Gouraud_FFP::requiresVertexStream(void) const
{
	return true;
}

bool Effect_Gouraud_FFP::requiresNormalStream(void) const
{
	return true;
}

bool Effect_Gouraud_FFP::requiresTexCoordStream(void) const
{
	return true;
}

_tstring Effect_Gouraud_FFP::getName(void) const
{
	return _T("Effect_Gouraud_FFP");
}

int Effect_Gouraud_FFP::howGood(effect_sig signature) const
{
	switch(signature)
	{
	case effect_DEFAULT:
	case effect_TEXTURE_LIT:
		return 50;

	case effect_TEXTURE_REPLACE:	return 0;
	case effect_NO_TEXTURE_LIT:		return 0;
	default:						return 0;
	};
}


}; // namespace

