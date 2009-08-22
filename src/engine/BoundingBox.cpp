/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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
#include "BoundingBox.h"

namespace Engine {

bool BoundingBox::testBoxVersusSphere(const vec3 &center, float radius) const
{
	float s, d = 0;

	const vec3 pos = m_Pos;
	const vec3 min = m_Min + pos;
	const vec3 max = m_Max + pos;

	//find the square of the distance
	//from the sphere to the box

	// X
	if( center.x < min.x )
	{
		s = center.x - min.x;
		d += s*s;
	}
	else if( center.x > max.x )
	{
		s = center.x - max.x;
		d += s*s;
	}
	
	// Z
	if( center.z < min.z )
	{
		s = center.z - min.z;
		d += s*s;
	}
	else if( center.z > max.z )
	{
		s = center.z - max.z;
		d += s*s;
	}

	return(d <= SQR(radius));
}

bool BoundingBox::Collision(BoundingBox &box)
{
	float left1   = m_Min.x + m_Pos.x;
	float right1  = m_Max.x + m_Pos.x;
	float top1    = m_Max.x + m_Pos.x;
	float bottom1 = m_Min.x + m_Pos.x;
	float left2   = box.m_Min.x +box. m_Pos.x;
	float right2  = box.m_Max.x + box.m_Pos.x;
	float top2    = box.m_Max.z + box.m_Pos.z;
	float bottom2 = box.m_Min.z + box.m_Pos.z;

	return
	(
		!(bottom1 < top2)
			&&
		!(top1 > bottom2)
			&&
		!(right1 < left2)
			&&
		!(left1 > right2)
	);
}

bool BoundingBox::Collision(float x, float y)
{
	float left   = m_Min.x + m_Pos.x;
	float bottom = m_Min.z + m_Pos.z;
	float width  = m_Max.x - m_Min.x;
	float height = m_Max.z - m_Min.z;
	float right  = left + width;
	float top    = bottom + height;

	return(x > left && x < right && y > top && y < bottom);
}

} // namespace Engine
