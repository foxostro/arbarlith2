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
#include "PropertyBag.h"
#include "Effect_Project_Shadows.h"

namespace Engine {

Effect_Project_Shadows::Effect_Project_Shadows(void)
{
	clear();
}

Effect_Project_Shadows::Effect_Project_Shadows(const Effect_Project_Shadows &e)
:Effect(e)
{}

void Effect_Project_Shadows::clear(void)
{
	Effect::clear();
}

void Effect_Project_Shadows::begin(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnableClientState(GL_VERTEX_ARRAY);

	glColorMask(0, 0, 0, 0);

	glDisable(GL_LIGHTING);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Use a depth offset to correct Z-aliasing
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(3, 3);

	// Disable textures we don't need
	Effect::disableAllTextureUnits();
}

void Effect_Project_Shadows::end(void)
{
	glPopAttrib();
	CHECK_GL_ERROR();
}

string Effect_Project_Shadows::getRequiredExtensions(void) const
{
	return string();
}

int Effect_Project_Shadows::getRequiredTextureUnits(void) const
{
	return 0;
}

void Effect_Project_Shadows::passTextureName(unsigned int, unsigned int)
{}

void Effect_Project_Shadows::passVertexStream(float *stream)
{
	glVertexPointer(3, GL_FLOAT, 0, stream);
}

void Effect_Project_Shadows::passNormalStream(float*)
{}

void Effect_Project_Shadows::passTexCoordStream(float *, unsigned int)
{}

bool Effect_Project_Shadows::requiresVertexStream(void) const
{
	return true;
}

bool Effect_Project_Shadows::requiresNormalStream(void) const
{
	return false;
}

bool Effect_Project_Shadows::requiresTexCoordStream(void) const
{
	return false;
}

string Effect_Project_Shadows::getName(void) const
{
	return "Effect_Project_Shadows";
}

int Effect_Project_Shadows::howGood(effect_sig) const
{
	return 0;
}

}; // namespace
