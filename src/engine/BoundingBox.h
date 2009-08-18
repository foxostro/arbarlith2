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

#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "vec4.h"

namespace Engine {

/**
Bounding boxes are used to represent the
volume of an object and test for collisions
@author Andrew Fox
*/
class BoundingBox
{
public:
	/** Position of the box */
	vec3 m_Pos;

	/** Minimum vertex of the box relative to m_Pos */
	vec3 m_Min;

	/** Maximum vertex of the box relative to m_Pos*/
	vec3 m_Max;

	/*
	Determines whether a collision between two boxes has occurred
	@param box The bounding box to test against
	@return Returns true if this bounding box intersects the given bounding box, otherwise, returns false.
	*/
	bool Collision(BoundingBox &box);

	/**
	Determines whether the point is within the vertical projection of the box
	@param p The point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(vec2 p)
	{
		return Collision(p.x, p.y);
	}
	/**
	Determines whether the point is within the vertical projection of the box
	@param x The x-coordinate of the point to test
	@param z The y-coordinate of the point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(float x, float z);

	/**
	Tests whether a sphere intersects the bounding box
	@param center The center of the sphere
	@param radius The radius of the sphere
	@return true if the two intersect
	*/
	bool testBoxVersusSphere(const vec3 &center, float radius) const;
};

} // namespace Engine

#endif
