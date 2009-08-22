/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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
#include "Effect.h"


namespace Engine {

void Effect::setTextureFilters(void)
{
	switch(g_Application.textureFilter)
	{
	case 2: // aniostropic
		if(supportsAniostropy)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_Application.aniostropy);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;

	case 1: // trilinear
		if(supportsAniostropy)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f); // disables aniostropic filtering
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;

	default: // bilinear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	};
}

void Effect::disableAllTextureUnits(void)
{
	if(!g_bUseMultitexture)
		return;

	for(int i=0; i<g_MultitextureUnits; ++i)
	{
		glActiveTextureARB(textureStages[i]);
		glDisable(GL_TEXTURE_2D);
	}

	for(int i=0; i<g_MultitextureUnits; ++i)
	{
		glClientActiveTextureARB(textureStages[i]);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Effect::disableExtraTextureUnits(void)
{
	if(!g_bUseMultitexture)
		return;

	disableAllTextureUnits();

	// Enable the primary texture unit
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

Effect::Effect(void)
{
	clear();
}

Effect::Effect(const Effect &e)
{
	copy(e);
}

Effect::~Effect(void)
{
	destroy();
}

void Effect::copy(const Effect &)
{}

void Effect::clear(void)
{}

void Effect::destroy(void)
{
	clear();
}

void Effect::passTextureName(unsigned int, unsigned int)
{}

void Effect::passVertexStream(float *)
{}

void Effect::passNormalStream(float *)
{}

void Effect::passTexCoordStream(float *, unsigned int)
{}

}; // namespace
