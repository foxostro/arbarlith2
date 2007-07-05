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
#include "propbag.h"
#include "Effect_Red_FFP.h"


namespace Engine { 



Effect_Red_FFP::Effect_Red_FFP(void)
{
	clear();
}

Effect_Red_FFP::Effect_Red_FFP(const Effect_Red_FFP &e)
:Effect(e)
{}

void Effect_Red_FFP::clear(void)
{
	Effect::clear();
}

void Effect_Red_FFP::begin(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	// Disable textures we don't need
	if(g_bUseMultitexture)
	{
		for(int i=getRequiredTextureUnits(); i<g_MultitextureUnits; ++i)
		{
			glActiveTextureARB(textureStages[i]);
			glDisable(GL_TEXTURE_2D);
		}
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}

	// Set the appropriate client states
	glEnableClientState(GL_VERTEX_ARRAY);

	// Select a red color
	glColor4fv(red);
}

void Effect_Red_FFP::end(void)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPopAttrib();
	CHECK_GL_ERROR();
}

_tstring Effect_Red_FFP::getRequiredExtensions(void) const
{
	return _tstring();
}

int Effect_Red_FFP::getRequiredTextureUnits(void) const
{
	return 0;
}

void Effect_Red_FFP::passTextureName(unsigned int, unsigned int)
{}

void Effect_Red_FFP::passVertexStream(float *stream)
{
	glVertexPointer(3, GL_FLOAT, 0, stream);
}

void Effect_Red_FFP::passNormalStream(float *)
{}

void Effect_Red_FFP::passTexCoordStream(float *, unsigned int)
{}

bool Effect_Red_FFP::requiresVertexStream(void) const
{
	return true;
}

bool Effect_Red_FFP::requiresNormalStream(void) const
{
	return true;
}

bool Effect_Red_FFP::requiresTexCoordStream(void) const
{
	return false;
}

_tstring Effect_Red_FFP::getName(void) const
{
	return _T("Effect_Red_FFP");
}

int Effect_Red_FFP::howGood(effect_sig signature) const
{
	switch(signature)
	{
	case effect_RED:	return 100;
	default:			return 0;
	};
}


}; // namespace
