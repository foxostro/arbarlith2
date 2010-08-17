/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2005-2007,2009,2010 Game Creation Society
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
#include "COLOR.h"
#include "Light.h"

namespace Engine { 

/** Controls global light levels */
float Light::BRIGHTNESS = 1.0f;

void Light::clear()
{
	enable = true;
	causesShadows = true;

	diffuse = white; // white * 0.7f;
	ambient = black;
	specular = white * 0.7f; // white
	lightPosition = vec3(0.0f, 2.0f, 0.0f);

	constantAttenuation = 0.0f;
	linearAttenuation = 0.01f;
	quadraticAttenuation = 0.05f;

	pointLight = true;
	lightDirection = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	spotAngle = 45.0f;
	spotExponent = 16.0f;

	lightProjectionMatrix.identity();
	lightViewMatrix.identity();
}

void Light::copy(const Light &light)
{
	causesShadows = light.causesShadows;
	diffuse = light.diffuse;
	ambient = light.ambient;
	specular = light.specular;
	lightPosition = light.lightPosition;
	constantAttenuation = light.constantAttenuation;
	linearAttenuation = light.linearAttenuation;
	quadraticAttenuation = light.quadraticAttenuation;
	pointLight = light.pointLight;
	lightDirection = light.lightDirection;
	spotAngle = light.spotAngle;
	spotExponent = light.spotExponent;

	lightProjectionMatrix = light.lightProjectionMatrix;
	lightViewMatrix = light.lightViewMatrix;
}

void Light::destroy()
{
	clear();
}

void Light::create()
{
	destroy();
}

bool Light::fromXml(PropertyBag &xml)
{
	destroy();

	xml.get("constantAttenuation", constantAttenuation);
	xml.get("linearAttenuation", linearAttenuation);
	xml.get("quadraticAttenuation", quadraticAttenuation);
	xml.get("lightPosition", lightPosition);;
	xml.get("pointLight", pointLight);
	xml.get("lightDirection", lightDirection);
	xml.get("spotAngle", spotAngle);
	xml.get("spotExponent", spotExponent);
	xml.get("enable", enable);

	calculateMatrices();

    return true;
}

PropertyBag Light::toXml(void)
{
	PropertyBag xml;

	xml.add("constantAttenuation", constantAttenuation);
	xml.add("linearAttenuation", linearAttenuation);
	xml.add("quadraticAttenuation", quadraticAttenuation);
	xml.add("lightPosition", lightPosition);;
	xml.add("pointLight", pointLight);
	xml.add("lightDirection", lightDirection);
	xml.add("spotAngle", spotAngle);
	xml.add("spotExponent", spotExponent);
	xml.add("enable", enable);
	
	return xml;
}

void Light::bind(unsigned int lightName) const
{
	float pos[4] = {lightPosition.x, lightPosition.y, lightPosition.z, 1.0f};
	float dir[4] = {-lightDirection.x, -lightDirection.y, -lightDirection.z, 1.0f};


	glLightfv(lightName,	GL_AMBIENT,					BRIGHTNESS * ambient);
	glLightfv(lightName,	GL_DIFFUSE,					BRIGHTNESS * diffuse);
	glLightfv(lightName,	GL_SPECULAR,				BRIGHTNESS * specular);
	glLightf(lightName,		GL_CONSTANT_ATTENUATION,	constantAttenuation);
	glLightf(lightName,		GL_LINEAR_ATTENUATION,		linearAttenuation);
	glLightf(lightName,		GL_QUADRATIC_ATTENUATION,	quadraticAttenuation);
	glLightf(lightName,		GL_SPOT_EXPONENT,			spotExponent);
	glLightfv(lightName,	GL_POSITION,				pos);


	if(pointLight)
	{
		glLightf(lightName, GL_SPOT_CUTOFF, 180.0f);
	}
	else
	{
		glLightfv(lightName, GL_SPOT_DIRECTION, dir);
		glLightf(lightName, GL_SPOT_CUTOFF, spotAngle);
	}
}

void Light::calculateMatrices(void)
{
	const vec4 lightCenter = lightPosition - lightDirection;
	const vec4 lightUp = vec4(0, 1, 0, 1);

	// save old matrices
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

		// Shadow matrices			
		glLoadIdentity();
		gluPerspective(spotAngle, 1.f, .1f, 50.f);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);
		
		glLoadIdentity();
		gluLookAt(	lightPosition.x, lightPosition.y,	lightPosition.z,
					lightCenter.x,	 lightCenter.y,		lightCenter.z,
					lightUp.x,		 lightUp.y,			lightUp.z);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
	
	// restore old matrices
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	frustum.CalculateFrustum(lightViewMatrix, lightProjectionMatrix);
}

const Frustum& Light::getFrustum(void) const
{
	return frustum;
}

void Light::beginClipping(void) const
{
	getFrustum().beginClipping();		
}

}; // namespace

