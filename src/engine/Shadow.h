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

#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "object.h"
#include "ActorSet.h"
#include "Light.h"

namespace Engine {

/** Represents a single shadow */
class Shadow
{
public:
	Shadow(void)
	{
		clear();
	}

	~Shadow(void)
	{
		destroy();
	}

	/** Clears the manager to a just-constructed state */
	void clear(void);

	/** Cleanly destroys and clears the shadow */
	void destroy(void);

	/** Reconstructs the Shadow */
	void create(void);

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Update the shadows in the world
	@param zoneActors Set of actors to draw the shadowed creature out of
	@param deltaTime Time elapsed since the last update
	*/
	void update(const ActorSet &zoneActors, float deltaTime);

	/**
	Binds the shadow to a specific actor
	@param actorID The id of an actor which generates the shadow
	*/
	void setActor(OBJECT_ID actorID);

	/**
	Binds the shadow to a specific light
	@param light The handle of the light that this shadow is bound to
	*/
	void setLight(const Light *light);

	/**
	Determines if a shadow is in use and ready
	@return true if the shadow is in use and ready
	*/
	bool isInUse(void) const;

	/**
	Gets the light that the shadow is assigned to, or null
	@return the light the shadow is assigned to or null
	*/
	const Light* getLight(void) const
	{
		return light;
	}

	/** Requests an update on the next tick */
	void requestUpate(void)
	{
		needsUpdate=true;
	}

	/** renders the shadow texture onto the screen to test it */
	void testDepthTexture(void) const;

	/**
	Binds the shadow map
	@param zoneActors Set of actors to draw the shadowed creature out of
	@param textureUnit Texture stage of the shadow
	*/
	void bind(const ActorSet &zoneActors, unsigned int textureUnit) const;

	/**
	Gets the shadow receivers
	@return the actor the shadow is assigned to or null
	*/
	const ActorSet& getShadowReceivers(void) const
	{
		return receivers;
	}

	/**
	Gets the shadow frustum
	@return shadow frustum
	*/
	const Frustum& getFrustum(void) const
	{
		return frustum;
	}

	/** draw the shadow frustum */
	void drawFrustum(void) const;

private:
	/** Shadow Frustum */
	Frustum frustum;

	/** times milliseconds until our periodic calculations */
	float periodicTimer;

	/** Set of all actors that will receive the shadow */
	ActorSet receivers;

	/** ID of the object that generates this shadow */
	OBJECT_ID actorID;

	/** Handle of the light that generates this shadow */
	const Light * light;

	/** Projection matrix from the light's perspective */
	mat4 lightProjectionMatrix;

	/** Model-View matrix from the light's perspective */
	mat4 lightViewMatrix;

	/** texture projection matrix */
	mat4 textureMatrix;

	/** Size of the shadow map texture */
	int shadowMapSize;

	/** Handle to the shadow map texture */
	unsigned int shadowMapTexture;

	/** Indicates that an update is required on the next tick */
	bool needsUpdate;

	/** Vertices of the shadow frustum */
	vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

	/**
	Calculate the matrices for the shadow
	@param light Light that will be generating the shadow
	@param actor Actor that will be casting the shadow
	@param shadowMapSize Size of the shadow map that we will be projecting onto
	@param lightProjectionMatrix Projection matrix from the light's perspective
	@param lightViewMatrix Model-View matrix from the light's perspective
	@param textureMatrix texture projection matrix
	@param lx Angle of the frustum in the x direction
	@param ly Angle of the frustum in the y direction
	*/
	static void calculateMatrices(const Light &light, const Actor &actor, int shadowMapSize, mat4& lightProjectionMatrix, mat4& lightViewMatrix, mat4& textureMatrix, float lx, float ly);

	/**
	Calculate the objects that should receive the shadow
	@param zoneActors Set of actors to draw the shadowed creature out of
	@param lightProjectionMatrix Projection matrix from the light's perspective
	@param lightViewMatrix Model-View matrix from the light's perspective
	@return the set of actors that might receive the shadow
	*/
	static ActorSet calculateReceivers(const ActorSet &zoneActors, const mat4& lightProjectionMatrix, const mat4& lightViewMatrix);

	/**
	Recalculates frustum vertices
	@param light Light that will be generating the shadow
	@param actor Actor that will be casting the shadow
	@param lx Angle of the frustum in the x direction
	@param ly Angle of the frustum in the y direction
	@param ntl
	@param ntr
	@param nbl
	@param nbr
	@param ftl
	@param ftr
	@param fbl
	@param fbr
	*/
	static void calculateFrustumVertices(const Light &light, const Actor &actor, float lx, float ly, vec3 &ntl, vec3 &ntr, vec3 &nbl, vec3 &nbr, vec3 &ftl, vec3 &ftr, vec3 &fbl, vec3 &fbr);

	/**
	Calculates the light frustum
	@param lightProjectionMatrix Light projection matrix
	@param lightViewMatrix Light's modelview matrix
	@return the frustum of the light cone
	*/
	static Frustum calculateFrustum(const mat4& lightProjectionMatrix, const mat4& lightViewMatrix);

	/**
	Renders the shadow into the specified texture
	@param shadowMapTexture Handle to the shadow map texture
	@param shadowMapSize Size of the shadow map that we will be projecting onto
	@param actor Actor that will be casting the shadow
	@param lightProjectionMatrix Projection matrix from the light's perspective
	@param lightViewMatrix Model-View matrix from the light's perspective
	Actually re-renders the shadow map
	*/
	static void renderToShadow(unsigned int shadowMapTexture, int shadowMapSize, const Actor &actor, const mat4& lightProjectionMatrix, const mat4& lightViewMatrix);

	/**

	Calculates the angular spread of the model
	@param actor actor who owns the model
	@param lightViewMatrix Model-View matrix from the light's perspective
	@param lx returns the angle across the x direction
	@param ly returns the angle across the y direction
	*/
	static void calculateAngularSpread(const Actor &actor, const mat4& lightViewMatrix, float &lx, float &ly);
};

} // namespace Engine

#endif
