/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009 Game Creation Society
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
#include "TextureHandle.h"


namespace Engine {



TextureHandle::TextureHandle(void)
{
	fileName="(nill)";
	width=height=0;
	alpha=false;
	id=0;
}

TextureHandle::TextureHandle(const string &fileName, int width, int height, bool alpha, GLuint id)
{
	this->fileName = fileName;
	this->width = width;
	this->height = height;
	this->alpha = alpha;
	this->id = id;
}

void TextureHandle::release(void)
{
	glDeleteTextures(1, &id);
	id=0;
}

void TextureHandle::reaquire(void)
{
	release();

	Image img(fileName);
	int depth = img.getDepth();

	// get a new id
	glGenTextures(1, &id);

	// and bind it as the present texture
	glBindTexture(GL_TEXTURE_2D, id);

	// Set the texture filtering according to global performance settings
	Effect::setTextureFilters();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// now build mipmaps from the texture data
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  depth,
					  img.getWidth(),
					  img.getHeight(),
					  depth==4 ? GL_RGBA : GL_RGB,
					  GL_UNSIGNED_BYTE,
					  img.getImage());
}

}; // namespace
