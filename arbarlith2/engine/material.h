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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "COLOR.h"
#include "image.h"
#include "TextureHandle.h"


namespace Engine {


/**
A material is responsible for data specific to an object that may be used as
input to an Effect.  This input is stored in VRAM in the form of texture
images, however, there is no reason why textures can not simply be used as a
two dimensional array of data input that is mapped to certain regions of
geometry. This is actually the case when a gloss map or a bump map is
specified in a texture.  Or when one is used to say, create a normalization
cube map for FFP bump mapping.
*/
class Material
{
public:
	/** Default constructor */
	Material(void);

	/**
	Copy constructor
	@param mat The object to copy
	*/
	Material(const Material &mat);

	/**
	Immediately loads a texture into texture unit one
	@param materialFileName Texture file name
	*/
	Material(const _tstring &materialFileName);

	/** Destructor */
	~Material(void);

	/**
	Copies across from another material
	@param mat The object to copy
	*/
	void copy(const Material &mat);

	/** Clears the state of the Material and resets it */
	void clear(void);

	/** Cleanly destroys the Material */
	void destroy(void);

	/**
	Loads a texture
	@param fileName The file name of the image file containing the texture
	@param textureUnit The texture unit to bind the texture with
	*/
	void loadTexture(const _tstring &fileName, unsigned int textureUnit);

	/**
	Loads a texture
	@param fileName The image object containing the texture
	@param textureUnit The texture unit to bind the texture with
	*/
	void loadTexture(Image &image, unsigned int textureUnit);

	/**
	Adds a texture to the list of textures to apply during multitexturing
	@param textureHandle A handle to the texture
	@param textureUnit The texture unit to bind the layer at
	*/
	void setTexture(TextureHandle *textureHandle, unsigned int textureUnit);

	/**
	Retrieve the name of the material
	@return The name of the material
	*/
	const _tstring& getName(void) const
	{
		return myName;
	}

	/**
	Changes the name of the material
	@param name The new name of the material
	*/
	void setName(const _tstring &name)
	{
		myName = name;
	}

	/**
	Sets the signature of the effect that this material is designed to work with.
	@param effect effect signature
	*/
	void setEffectSignature(int effect)
	{
		this->effect = effect;
	}

	/**
	Gets the signature of the effect that this material is designed to work with.
	@return effect signature
	*/
	int getEffectSignature(void) const
	{
		return effect;
	}

	/**	Passes Material information to the currently bound Effect. */
	void bind(void) const;

public:
	bool glow;

	/**ambient part*/
	COLOR ambient;

	/**diffuse part*/
	COLOR diffuse;

	/**specular part*/
	COLOR specular;

	/**shininess of material*/
	float shininess;

private:
	/** Name of te material */
	_tstring myName;

	/** The signature of the effect that this material is designed to work with */
	int effect;

	/** an array of texture handles that is indexed by the appropriate texture unit to use that texture with. */
	TextureHandle *textures[32];
};

} // namespace Engine

#endif

