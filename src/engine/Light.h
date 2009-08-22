/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2005-2007 Game Creation Society
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

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vec4.h"
#include "mat4.h"
#include "PropertyBag.h"
#include "frustum.h"

namespace Engine {

/**
Encapsulates the state of a typical OpenGL light.
The state can be bound to an OpenGL light, given the light handle.
*/
class Light
{
public:
	/** Global brightness level for fading the lights all at once */
	static float BRIGHTNESS;

	/** Constant attenuation of the Light */
	float constantAttenuation;

	/** Linear attenuation of the Light */
	float linearAttenuation;

	/** Quadratic attenuation of the Light */
	float quadraticAttenuation;

	/** Diffuse color of the light */
	COLOR diffuse;

	/** Ambient color of the light */
	COLOR ambient;

	/** Specular color of the light */
	COLOR specular;

	/** Indicates that the Light is a point Light */
	bool pointLight;

    /**
	If the Light is to be used as a spot Light, then this specifies the angle of the light cone.
	If the angle is et to 180.0, then the Light is treated as a point Light anyway.
	*/
	float spotAngle;

	/** The spot light exponent */
	float spotExponent;

	/**
	When true, the lighting engine will consider adding the light ot the scene when possible.
	Otherwise, the light will never be added to the scene.
	*/
	bool enable;

	/** Indicates that the Light will cause shadows to be cast */
	bool causesShadows;

private:
	/** Position of the light */
	vec3 lightPosition;

	/** If the Light is not a point Light, then this determines the direction of the Light */
	vec4 lightDirection;

	/** Projection matrix from the light's perspective */
	mat4 lightProjectionMatrix;

	/** Model-View matrix from the light's perspective */
	mat4 lightViewMatrix;

	/** Light frustum (if spot light) */
	Frustum frustum;

public:
	/** Default constructor */
	Light(void)
	{
		clear();
	}

	/**
	Create and setup the Light using settings loaded from XML
	@param xml The XML data source
	*/
	Light(PropertyBag &xml)
	{
		clear();
		fromXml(xml);
	}

	/**
	Create and setup the Light using settings copied from another light
	@param light The light to make a copy of
	*/
	Light(const Light &light)
	{
		copy(light);
	}

	/** Destructor */
	~Light(void)
	{}

	/** Reset the Light settings to defaults */
	void clear(void);

	/** Cleanly destroy and clear the Light */
	void destroy(void);

	/** Create and setup the Light using the current settings */
	void create(void);

	/**
	Create and setup the Light using settings copied from another light
	@param light The light to make a copy of
	*/
	void copy(const Light &light);

	/**
	Assignment operator
	@param rh The right-hand side
	@return The new light
	*/
	Light operator=(const Light &rh)
	{
		copy(rh);
		return(*this);
	}

	/**
	Create and setup the Light using settings loaded from XML
	@param xml The XML data source
	*/
	bool fromXml(PropertyBag &xml);

	/**
	Saves Light settings to XML
	@return XML data
	*/
	PropertyBag toXml(void);

	/**
	Bind the light state to an OpenGL light
	@param lightName The OpenGL light name
	*/
	void bind(unsigned int lightName) const;

	/**
	Gets the Light direction for the spot Light.
	If the Light is not a spot Light, then this is meaningless.
	@return direction
	*/
	inline const vec3& getDirection(void) const
	{
		return lightDirection;
	}

	/**
	Gets the light position
	@return position
	*/
	inline const vec3& getPosition(void) const
	{
		return lightPosition;
	}

	/**
	Sets the light position
	@param x The position of the light
	*/
	inline void setPosition(const vec3 &x)
	{
		lightPosition = x;
		calculateMatrices();
	}

	/**
	If the Light is a spot light, then this method positions and orients the Light.
	The Light will be placed at the location specified by 'position' and caused to
	point in the direction of the location specified by 'center.'
	@param position	The position of the Light.
	@param center	A point that the spot Light is focused on.
	*/
	inline void lookAt(const vec3 &position, const vec3 &center)
	{
		lightPosition = position;
		lightDirection = lightPosition - center;

		calculateMatrices();
	}

	/** Clip along the light frustum */
	void beginClipping(void) const;

	/**
	Gets the camera frustum
	@return camera frustum
	*/
	const Frustum& getFrustum(void) const;

private:
	/**
	Calculates the projection and modelview matrices
	@return lightProjectionMatrix*lightViewMatrix
	*/
	mat4 calculatePVMatrix(void);

	/** Recalculate light projection and model-view matrices */
	void calculateMatrices(void);
};

} // namespace Engine

#endif
