/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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
#include "mat4.h"


namespace Engine { 



vec4 mat4::transformVector(const vec4 &v) const
{
	// the return vec3
	vec4 temp;

	// temp = v * mt;
	temp.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
	temp.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
	temp.z = v.x*m[2] + v.y*m[6] + v.z*m[10]+ m[14];
	temp.w = v.x*m[3] + v.y*m[7] + v.z*m[11]+ m[15];
	
	//temp = temp + getPos();

	// and return it
	return temp;
}

void mat4::rotate(float radians, const vec3 &axis)
{
	identity();

	float degrees = radians * 180.0f / (float)M_PI;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(degrees, axis.x, axis.y, axis.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
}

void mat4::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 z = vec3(eye-center).getNormal();
	vec3 y = up;
	vec3 x = up.cross(z);

	identity();
	setAxisX(x);
	setAxisY(y);
	setAxisZ(z);
	setPos(eye);
}


}; // namespace

