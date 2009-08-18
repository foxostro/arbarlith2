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

#include "stdafx.h" // Master Header
#include "gl.h"

#include "material.h"
#include "Effect.h"
#include "EffectManager.h"


namespace Engine {



Material::Material(void)
{
	clear();
}

Material::Material(const Material &mat)
{
	copy(mat);
}

Material::Material(const string &materialFileName)
{
	clear();
	loadTexture(materialFileName, 0);
	setName(materialFileName);
}

Material::~Material(void)
{
	destroy();
}

void Material::copy(const Material &mat)
{
	clear();

	ambient  = mat.ambient;
	diffuse  = mat.diffuse;
	specular = mat.specular;

	shininess=mat.shininess;

	glow=mat.glow;

	myName=mat.myName;
	effect=mat.effect;

	// Copy the texture handles
	memcpy(textures, mat.textures, sizeof(textures));
}

void Material::clear(void)
{
	ambient  = black;
	diffuse  = COLOR(0.8f,0.8f,0.8f,1.0f);
	specular = COLOR(0.8f,0.8f,0.8f,1.0f);

	shininess=16.0f;

	myName = "not-set";

	effect=0;

	glow=false;

	memset(textures, 0, sizeof(textures));
}

void Material::destroy(void)
{
	clear();
}

void Material::loadTexture(const string &fileName, unsigned int textureUnit)
{
	Image image(fileName);
	loadTexture(image, textureUnit);
}

void Material::loadTexture(Image &image, unsigned int textureUnit)
{
	TextureHandle *handle = g_TextureMgr.Create(&image);
	setTexture(handle, textureUnit);
}

void Material::setTexture(TextureHandle *handle, unsigned int textureUnit)
{
	ASSERT(textureUnit < sizeof(textures), "Material::setTexture  ->  Invalid array index in textureUnit");
	ASSERT(handle!=0, "handle was null");

	textures[textureUnit] = handle;
}

void Material::bind(void) const
{
	CHECK_GL_ERROR();

	Effect &effect = EffectManager::GetSingleton().getEffect();

	// Pass texture data to the Effect
	for(int i=0; i<g_MultitextureUnits; ++i)
	{
		TextureHandle *handle = textures[i];
		if(handle!=0)
			effect.passTextureName(handle->getID(), i);
	}

	// set the material properties
	glMaterialfv(GL_FRONT,	GL_AMBIENT,		ambient);
	glMaterialfv(GL_FRONT,	GL_DIFFUSE,		diffuse);
	glMaterialfv(GL_FRONT,	GL_SPECULAR,	specular);
	glMaterialf(GL_FRONT,	GL_SHININESS,	shininess);

	CHECK_GL_ERROR();
}


}; // namespace

