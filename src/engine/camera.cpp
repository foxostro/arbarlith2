/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Author: Richard Halstead
E-Mail: mailto:rhalstea@andrew.cmu.edu

Copyright (c) 2005-2007,2009 Game Creation Society
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

#include "camera.h"



namespace Engine {




void Camera::clear(void)
{
	moved = true;
	orientation.identity();
	position.zero();
}

void Camera::destroy(void)
{
	clear();
}

void Camera::setCamera(void)
{
	// The orientation matrix conveys no position data!
	orientation.setPos(vec3(0,0,0));

	// Load the matrix for the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(getTransformation());

	if(moved)
		frustum = calculateFrustum();

	// reset
	moved = false;
}

PropertyBag Camera::toXml(void) const
{
	PropertyBag xml;

	xml.add("z", orientation.getAxisZ());
	xml.add("y", orientation.getAxisY());
	xml.add("x", orientation.getAxisX());
	xml.add("pos", position);

	return xml;
}

void Camera::fromXml(PropertyBag &xml)
{
	destroy();

	vec4 x, y, z;

	xml.get("z",	x);
	xml.get("y",	y);
	xml.get("x",	z);
	xml.get("pos",	position);

	orientation.set(position, x, y, z);

	moved = true;
}

void Camera::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, orientation);
	glPopMatrix();

	// record the position of the camera
	position = eye;

	moved = true;
}

mat4 Camera::getTransformation(void) const
{
	mat4 transformation;
	mat4 translation;

	translation.translate(-position);

	transformation = orientation * translation;

	return transformation;
}

void Camera::updateFlyingCamera(float deltaTime)
{
	// Get data from 'this' which we will operate upon
	mat4 orientation = this->orientation;
	vec3 position = this->position;

	// Constant values
	const float baseSpeed		= 18.0f;
	const float baseRotateSpeed	= (float)(M_PI);

	// Correctly timed speeds
	const float timeModifier	= deltaTime/1000.0f;
	const float speed			= baseSpeed * timeModifier;
	const float rotateSpeed		= baseRotateSpeed * timeModifier;




	if(g_Keys.isKeyDown(KEY_EDITOR_WALK_FWD)) {
		vec3 delta = orientation.getAxisZ() * (-speed);
		position = position + delta;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_WALK_REV)) {
		vec3 delta = orientation.getAxisZ() * speed;
		position = position + delta;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_WALK_LEFT)) {
		vec3 delta = orientation.getAxisX() * (-speed);
		position = position + delta;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_WALK_RIGHT)) {
		vec3 delta = orientation.getAxisX() * speed;
		position = position + delta;
	}




	if(g_Keys.isKeyDown(KEY_EDITOR_ROTATE_UP)) {
		mat4 r;
		r.rotate(-rotateSpeed, orientation.getAxisX());
		orientation = orientation * r;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_ROTATE_DOWN)) {
		mat4 r;
		r.rotate(rotateSpeed, orientation.getAxisX());
		orientation = orientation * r;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_ROTATE_LEFT)) {
		mat4 r;
		r.rotate(-rotateSpeed, vec3(0,1,0));
		orientation = orientation * r;
	}

	if(g_Keys.isKeyDown(KEY_EDITOR_ROTATE_RIGHT)) {
		mat4 r;
		r.rotate(rotateSpeed, vec3(0,1,0));
		orientation = orientation * r;
	}



	// Set the camera
	setOrientation(orientation);
	setPosition(position);
}

Frustum Camera::getFrustum(void) const
{
	return frustum;
}

Frustum Camera::calculateFrustum(void)
{
	Frustum f;

	const mat4 &modl = getTransformation();

	mat4 proj;
	glGetFloatv(GL_PROJECTION_MATRIX, proj);

	f.CalculateFrustum(modl, proj);

	return f;
}

}; // namespace
