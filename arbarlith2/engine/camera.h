/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "mat4.h"
#include "PropertyBag.h"
#include "frustum.h"

namespace Engine {

/**
Flying camera
@author Andrew Fox
@version December 2005
*/
class Camera
{
private:
	/** Indicates that the camera has moved since the last call to setCamera */
	bool moved;

	/** Camera transformation matrix */
	mat4 orientation;

	/** Position of the camera */
	vec3 position;

	/** Most recently cached camera frustum */
	Frustum frustum;

	/**
	Calculates the camera frustum
	@return Camera frtustum
	*/
	Frustum calculateFrustum(void);

public:
	/**
	Loads the camera from an XML source
	@param tag XML source
	*/
	Camera(PropertyBag &tag)
	{
		clear();
		fromXml(tag);
	}

	/** Default constructor */
	Camera(void)
	{
		clear();
	}

	/** Destructor */
	virtual ~Camera(void)
	{
		destroy();
	}

	/** Resets the camera to a just-constructed state */
	void clear(void);

	/** Cleanly destroys the camera and clears it */
	void destroy(void);

	/**
	Sets the camera position and orientation.
	Changes the current matrix mode to GL_MODELVIEW.
	*/
	void setCamera(void);

	/**
	Gets the orientation of the camera
	@return orientation matrix
	*/
	const mat4& getOrientation(void) const
	{
		return orientation;
	}

	/**
	Gets the orientation of the camera
	@return orientation matrix
	*/
	mat4 getTransformation(void) const;

	/**
	Sets the orientation of the camera
	@param mat The new matrix for the camera
	*/
	void setOrientation(const mat4 &mat)
	{
		orientation = mat;
		moved = true;
	}

	/**
	Gets the position of the camera
	@return position vector
	*/
	const vec3& getPosition(void) const
	{
		return position;
	}

	/**
	Sets the position of the camera
	@param x The new position for the camera
	@return position vector
	*/
	void setPosition(const vec3 &x)
	{
		position = x;
	}

	/**
	Creates an xml tag to represent the object
	@return The tag representing this object
	*/
	virtual PropertyBag toXml(void) const;

	/**
	Creates the object from an xml tag
	@param xml The tag representing the object
	*/
	virtual void fromXml(PropertyBag &xml);

	/**
	Determines whether the camera has moved since the last tick
	@return true if the camera has moved, false otherwise
	*/
	bool hasMoved(void) const
	{
		return moved;
	}

	/**
	The lookAt method creates a viewing matrix derived from an eye point,
	a reference point indicating the center of the scene, and an up vector.
	The matrix maps the reference point to the negative z-axis and the eye
	point to the origin, so that when you use a typical projection matrix,
	the center of the scene maps to the center of the viewport. Similarly,
	the direction described by the up vector projected onto the viewing plane
	is mapped to the positive y-axis so that it points upward in the viewport.
	The up vector must not be parallel to the line of sight from the eye to
	the reference point.

	@param eye		The position of the eye point.
	@param center	The position of the reference point.
	@param up		The direction of the up vector.
	*/
	void lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);

	/**
	Updates the flying camera
	@param deltaTime The milliseconds since the last update
	*/
	void updateFlyingCamera(float deltaTime);

	/**
	Gets the camera frustum
	return camera frustum
	*/
	Frustum getFrustum(void) const;
};

} // namespace Engine

#endif
