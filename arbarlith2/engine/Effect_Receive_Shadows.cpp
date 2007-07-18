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
#include "PropertyBag.h"
#include "Effect_Receive_Shadows.h"

namespace Engine { 

static float shadowAlphaThreshold = 0.90f;

Effect_Receive_Shadows::Effect_Receive_Shadows(void)
{
	clear();
}

Effect_Receive_Shadows::Effect_Receive_Shadows(const Effect_Receive_Shadows &e)
:Effect(e)
{}

void Effect_Receive_Shadows::clear(void)
{
	Effect::clear();
}

void Effect_Receive_Shadows::begin(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_LIGHTING);

	// Discard fragments that fail the shadow test
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, shadowAlphaThreshold);

	Effect::disableExtraTextureUnits();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void Effect_Receive_Shadows::end(void)
{
	glPopAttrib();
	CHECK_GL_ERROR();
}

_tstring Effect_Receive_Shadows::getRequiredExtensions(void) const
{
	return _T("GL_ARB_multitexture");
}

int Effect_Receive_Shadows::getRequiredTextureUnits(void) const
{
	return 2;
}

void Effect_Receive_Shadows::passTextureName(unsigned int textureName, unsigned int textureUnit)
{
	glActiveTextureARB(textureStages[textureUnit]);
	Effect::setTextureFilters();
	glBindTexture(GL_TEXTURE_2D, textureName);
	glEnable(GL_TEXTURE_2D);
}

void Effect_Receive_Shadows::passVertexStream(float *stream)
{
	glVertexPointer(3, GL_FLOAT, 0, stream);
}

void Effect_Receive_Shadows::passNormalStream(float *stream)
{
	glNormalPointer(GL_FLOAT, 0, stream);
}

void Effect_Receive_Shadows::passTexCoordStream(float *stream, unsigned int textureUnit)
{
	if(g_bUseMultitexture)
	{
		glClientActiveTextureARB(textureStages[textureUnit]);
	}

	glTexCoordPointer(2, GL_FLOAT, 0, stream);
}

bool Effect_Receive_Shadows::requiresVertexStream(void) const
{
	return true;
}

bool Effect_Receive_Shadows::requiresNormalStream(void) const
{
	return true;
}

bool Effect_Receive_Shadows::requiresTexCoordStream(void) const
{
	return true;
}

_tstring Effect_Receive_Shadows::getName(void) const
{
	return _T("Effect_Receive_Shadows");
}

int Effect_Receive_Shadows::howGood(effect_sig) const
{
	return 0;
}


}; // namespace
