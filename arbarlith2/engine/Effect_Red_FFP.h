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

#ifndef EFFECT_RED_FFP_H
#define EFFECT_RED_FFP_H

#include "Effect.h"
#include "Singleton.h"


namespace Engine {



/**
This Effect shades geometry so that it is untextured, red, and lit
*/
class Effect_Red_FFP : public Effect, public Singleton<Effect_Red_FFP>
{
public:
	/** Default constructor */
	Effect_Red_FFP(void);

	/**
	Copy constructor
	@param e The object to copy
	*/
	Effect_Red_FFP(const Effect_Red_FFP &e);

	/** Clears the state of the Effect and resets it */
	virtual void clear(void);

	/**	Sets up the states necessary for the Effect and begins shading */
	virtual void begin(void);

	/** Allows the effect to clean up after itself */
	virtual void end(void);

	/**
	Gets the OpenGL extensions required to execute the Effect
	@return OpenGL extension strings
	*/
	virtual _tstring getRequiredExtensions(void) const;

	/**
	Gets the number of texture units necessary to properly execute the Effect
	@return The number of texture units required
	*/
	virtual int getRequiredTextureUnits(void) const;

	/**
	Passes the texture name to the Effect so that the texture may be bound and and sent down the graphics pipeline.
	@param textureName The OpenGL name of the texture
	@param textureUnit The texture unit that the texture is assigned to
	*/
	virtual void passTextureName(unsigned int textureName, unsigned int textureUnit);

	/**
	Pass vertex position data to the Effect
	@param verts An stream of floats containing vertex positions in triplets
	*/
	virtual void passVertexStream(float *stream);

	/**
	Pass vertex normal data to the Effect
	@param stream An stream of floats containing vertex normals in triplets
	*/
	virtual void passNormalStream(float *stream);

	/**
	Pass vertex tex-coord data to the Effect
	@param stream An stream of floats containing vertex tex-coord in pairs
	@param textureUnit The texture unit that the texture coordinates are assigned to
	*/
	virtual void passTexCoordStream(float *stream, unsigned int textureUnit);

	/**
	Determines whether vertex position data is necessary for the proper execution of the Effect
	@return true if the stream is required
	*/
	virtual bool requiresVertexStream(void) const;

	/**
	Determines whether vertex normal data is necessary for the proper execution of the Effect
	@return true if the stream is required
	*/
	virtual bool requiresNormalStream(void) const;

	/**
	Determines whether vertex tex-coord data is necessary for the proper execution of the Effect
	@return true if the stream is required
	*/
	virtual bool requiresTexCoordStream(void) const;

	/**
	Retrieve the name of the Effect
	@return The name of the Effect
	*/
	virtual _tstring getName(void) const;

	/**
	Has the Effect determine how well it can implement an effect signature
	@param signature The effectsignature to test
	@return the priority of the effect implentation, 0 is perfectly WRONG, 100 is perfect
	*/
	virtual int howGood(effect_sig signature) const;
};

} // namespace Engine

#endif
