/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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
#include "frustum.h"
#include "world.h"
#include "Shadow.h"

namespace Engine {

const float nearClip = 0.1f;
const float farClip = 20.0f;

void Shadow::clear(void)
{
	lightProjectionMatrix.identity();
	lightViewMatrix.identity();
	textureMatrix.identity();
	shadowMapSize = 0;
	shadowMapTexture = 0;
	actorID = INVALID_ID;
	light = 0;
	periodicTimer = 0.0f;
	receivers.clear();
}

void Shadow::destroy(void)
{
	release();
	clear();
}

void Shadow::create(void)
{
	shadowMapSize = 128;

	reaquire();
}

void Shadow::release(void)
{
	glDeleteTextures(1, &shadowMapTexture);
	shadowMapTexture = 0;
}

void Shadow::reaquire(void)
{
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Shadow::update(const ActorSet &zoneActors, float deltaTime)
{
	if(light!=0 && zoneActors.isMember(actorID))
	{
		const Actor &actor = zoneActors.get(actorID);

		// Update the shadow when something has changed
		needsUpdate |= (actor.hasAnimated || actor.hasMoved);

		// Update when necessary or requested
		if(needsUpdate)
		{
			float lx=0, ly=0;

			calculateAngularSpread(actor, lightViewMatrix, lx, ly);

			calculateMatrices(*light, actor, shadowMapSize, lightProjectionMatrix, lightViewMatrix, textureMatrix, lx, ly);

			frustum = calculateFrustum(lightProjectionMatrix, lightViewMatrix);

			if(g_Application.displayDebugData)
			{
				calculateFrustumVertices(*light, actor, lx, ly, ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr);
			}

			renderToShadow(shadowMapTexture, shadowMapSize, actor, lightProjectionMatrix, lightViewMatrix);

			needsUpdate=false;
		}

		// Periodically take some time to determine the actors that be receiving this shadow
		periodicTimer-=deltaTime;
		if(periodicTimer<0)
		{
			periodicTimer = 250.0f + FRAND_RANGE(100.0f, 250.0f); // stagger
			receivers = calculateReceivers(zoneActors, lightProjectionMatrix, lightViewMatrix);
		}
	}
}

ActorSet Shadow::calculateReceivers(const ActorSet &zoneActors, const mat4& lightProjectionMatrix, const mat4& lightViewMatrix)
{
	Frustum f;
	f.CalculateFrustum(lightViewMatrix, lightProjectionMatrix);

	const ActorSet s = zoneActors.isWithin(f);

	return s;
}

void Shadow::setActor(OBJECT_ID actorID)
{
	if(this->actorID != actorID)
	{
		this->actorID = actorID;
		needsUpdate=true;
	}
}

void Shadow::setLight(const Light *light)
{
	if(this->light != light)
	{
		this->light = light;
		needsUpdate=true;
	}
}

void Shadow::calculateMatrices(const Light &light, const Actor &actor, int shadowMapSize, mat4& lightProjectionMatrix, mat4& lightViewMatrix, mat4& textureMatrix, float lx, float ly)
{
	const vec3 &lightPosition = light.getPosition();
	const vec3 &lightCenter = actor.getPos();
	const vec3 lightUp(0.0f, 1.0f, 0.0f);

// Calculate the model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(	lightPosition.x,	lightPosition.y,	lightPosition.z,
				lightCenter.x,		lightCenter.y,		lightCenter.z,
				lightUp.x,			lightUp.y,			lightUp.z);
	lightViewMatrix.zero();
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
	glPopMatrix();

// Calculate the projection matrix
	// row 1
	lightProjectionMatrix.m[0] = (0.995f-1.0f/(float)shadowMapSize)/lx;
	lightProjectionMatrix.m[1] = 0.0f;
	lightProjectionMatrix.m[2] = 0.0f;
	lightProjectionMatrix.m[3] = 0.0f;
	// row 2
	lightProjectionMatrix.m[4] = 0.0f;
	lightProjectionMatrix.m[5] = (0.995f-1.0f/(float)shadowMapSize)/ly;
	lightProjectionMatrix.m[6] = 0.0f;
	lightProjectionMatrix.m[7] = 0.0f;
	// row 3
	lightProjectionMatrix.m[8]  = 0.0f;
	lightProjectionMatrix.m[9]  = 0.0f;
	lightProjectionMatrix.m[10] = (farClip + nearClip) / (nearClip - farClip);
	lightProjectionMatrix.m[11] = -1.0f;
	// row 4
	lightProjectionMatrix.m[12] = 0.0f;
	lightProjectionMatrix.m[13] = 0.0f;
	lightProjectionMatrix.m[14] = 2.0f * (farClip * nearClip) / (nearClip - farClip);
	lightProjectionMatrix.m[15] = 0.0f;

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	const float biasMatrixf[] = {	0.5f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.5f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.5f, 0.0f,
						0.5f, 0.5f, 0.5f, 1.0f};	//bias from [-1, 1] to [0, 1]
	mat4 biasMatrix = biasMatrixf;

	// Calculate the texture matrix
	textureMatrix = biasMatrix * lightProjectionMatrix * lightViewMatrix;
}

Frustum Shadow::calculateFrustum(const mat4& lightProjectionMatrix, const mat4& lightViewMatrix)
{
	Frustum frustum;
	frustum.CalculateFrustum(lightViewMatrix, lightProjectionMatrix);
	return frustum;
}

void Shadow::calculateFrustumVertices(const Light &light, const Actor &actor, float lx, float ly, vec3 &ntl, vec3 &ntr, vec3 &nbl, vec3 &nbr, vec3 &ftl, vec3 &ftr, vec3 &fbl, vec3 &fbr)
{
	const vec3 &lightPosition = light.getPosition();
	const vec3 &lightCenter = actor.getPos();
	const vec3 lightUp(0.0f, 1.0f, 0.0f);

	float tanX = tanf(lx);
	float tanY = tanf(ly);
	float nh = nearClip * tanY;
	float nw = nearClip * tanX;
	float fh = farClip  * tanY;
	float fw = farClip  * tanX;

	vec3 dir,nc,fc,X,Y,Z;

	Z = lightPosition - lightCenter;
	Z.normalize();

	X = lightUp.cross(Z);
	X.normalize();

	Y = Z.cross(X);

	nc = lightPosition - Z * nearClip;
	fc = lightPosition - Z * farClip;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;
}

void Shadow::renderToShadow(unsigned int shadowMapTexture, int shadowMapSize, const Actor &actor, const mat4& lightProjectionMatrix, const mat4& lightViewMatrix)
{
CHECK_GL_ERROR();

	glClear(GL_DEPTH_BUFFER_BIT);

	effect_Begin(effect_CLASS_PROJECT_SHADOWS);

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(lightProjectionMatrix);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadMatrixf(lightViewMatrix);

			//Use viewport the same size as the shadow map
			glViewport(0, 0, shadowMapSize, shadowMapSize);

			//Draw the scene
			actor.drawObjectToDepthBuffer();

			//Read the depth buffer into the shadow map texture
			glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

			// Pop the modelview matrix we set up
			glPopMatrix();

			// pop the projection matrix we set up
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			// go back to modelview mode
			glMatrixMode(GL_MODELVIEW);

	effect_End();

CHECK_GL_ERROR();
}

void Shadow::bind(const ActorSet &zoneActors, unsigned int textureUnit) const
{
	if(light!=0 && zoneActors.isMember(actorID))
	{
		CHECK_GL_ERROR();

		// Bind shadow texture
		glActiveTextureARB(textureStages[textureUnit]);
		glClientActiveTextureARB(textureStages[textureUnit]);
		glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
		glEnable(GL_TEXTURE_2D);

		// Define the matrix
		const float row1[4] = {textureMatrix.m[0], textureMatrix.m[4], textureMatrix.m[8], textureMatrix.m[12]};
		const float row2[4] = {textureMatrix.m[1], textureMatrix.m[5], textureMatrix.m[9], textureMatrix.m[13]};
		const float row3[4] = {textureMatrix.m[2], textureMatrix.m[6], textureMatrix.m[10], textureMatrix.m[14]};
		const float row4[4] = {textureMatrix.m[3], textureMatrix.m[7], textureMatrix.m[11], textureMatrix.m[15]};

		// Enable tex coord generation
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable(GL_TEXTURE_GEN_Q);

		// Define the parameters of the generation
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

		// Pass the texture matrix we defined above
		glTexGenfv(GL_S, GL_EYE_PLANE, row1);
		glTexGenfv(GL_T, GL_EYE_PLANE, row2);
		glTexGenfv(GL_R, GL_EYE_PLANE, row3);
		glTexGenfv(GL_Q, GL_EYE_PLANE, row4);

		// Enable shadow comparison
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

		CHECK_GL_ERROR();
	}
}

void Shadow::testDepthTexture(void) const
{
	glPushAttrib(GL_ENABLE_BIT);

	// Get ready for 2D
	glColor4fv(white);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_FOG);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Save the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Set up the projection matrix for 2D
	glLoadIdentity();
	glOrtho(0.0f, 1024, 0.0f, 768, -1.0f, 1.0f);

	// Save the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Set up the model view matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -0.2f);

	// Bind the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	//Disable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	// Render a textured quad on the screen
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(400,   0, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(400, 400, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(  0, 400, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(  0,   0, 0);
	glEnd();

	// Restore the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Use modelview mode
	glMatrixMode(GL_MODELVIEW);

	// Restore settings
	glPopAttrib();
}

void Shadow::drawFrustum(void) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4fv(red);

	glBegin(GL_QUADS);
	//near plane
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);

	//far plane
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);

	//bottom plane
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);

	//top plane
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);

	//left plane
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);

	// right plane
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);

	glEnd();

	glPopAttrib();
}

void Shadow::calculateAngularSpread(const Actor &actor, const mat4& lightViewMatrix, float &lx, float &ly)
{
	const AnimationController *mdl = actor.getModel();

	if(mdl!=0)
	{
		const AnimationSequence &animation = mdl->getAnimation();
		mat4 m = lightViewMatrix * (actor.toWorldSpace());
		animation.calculateFOV(m, lx, ly);
	}
}

bool Shadow::isInUse(void) const
{
	return (actorID!=INVALID_ID) && (light!=0);
}

} // namespace Engine
