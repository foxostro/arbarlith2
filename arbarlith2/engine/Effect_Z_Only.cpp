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
#include "propbag.h"
#include "Effect_Z_Only.h"

namespace Engine { 

Effect_Z_Only::Effect_Z_Only(void)
{
	clear();
}

Effect_Z_Only::Effect_Z_Only(const Effect_Z_Only &e)
:Effect(e)
{}

void Effect_Z_Only::clear(void)
{
	Effect::clear();
}

void Effect_Z_Only::begin(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColorMask(0, 0, 0, 0);
	glDisable(GL_LIGHTING);	
	Effect::disableAllTextureUnits();
	glEnableClientState(GL_VERTEX_ARRAY);
}

void Effect_Z_Only::end(void)
{
	glPopAttrib();
	CHECK_GL_ERROR();
}

_tstring Effect_Z_Only::getRequiredExtensions(void) const
{
	return _T("");
}

int Effect_Z_Only::getRequiredTextureUnits(void) const
{
	return 0;
}

void Effect_Z_Only::passTextureName(unsigned int, unsigned int)
{}

void Effect_Z_Only::passVertexStream(float *stream)
{
	glVertexPointer(3, GL_FLOAT, 0, stream);
}

void Effect_Z_Only::passNormalStream(float *)
{}

void Effect_Z_Only::passTexCoordStream(float *, unsigned int)
{}

bool Effect_Z_Only::requiresVertexStream(void) const
{
	return true;
}

bool Effect_Z_Only::requiresNormalStream(void) const
{
	return false;
}

bool Effect_Z_Only::requiresTexCoordStream(void) const
{
	return false;
}

_tstring Effect_Z_Only::getName(void) const
{
	return _T("Effect_Z_Only");
}

int Effect_Z_Only::howGood(effect_sig) const
{
	return 0;
}


}; // namespace
