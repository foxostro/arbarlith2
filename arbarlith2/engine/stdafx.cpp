/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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
#include "text.h"
#include "3dmath.h"

namespace Engine {

int stoi(const _tstring &s)
{
      int ret = 0;
      _tstringstream stream;

      stream << s;
      stream >> ret;

      return ret;
}

float stof(const _tstring &s)
{
      float ret = 0.0f;
      _tstringstream stream;

      stream << s;
      stream >> ret;

      return ret;
}

_tstring itoa(int i)
{
	_tstring ret;
	_tstringstream stream;

	stream << i;
	stream >> ret;

	return ret;
}

_tstring ftoa(float f, int dec)
{
	_tstring ret;
	_tstringstream stream;

	stream.precision(dec);
	stream << f;
	stream >> ret;

	return ret;
}

_tstring fitToFieldSize(const _tstring &in, size_t fieldSize, JUSTIFY justify)
{
	if(in.size() > fieldSize)
	{
		return in.substr(0, fieldSize);
	}
	else if(in.size() == fieldSize)
	{
		return in;
	}
	else
	{
		size_t charsRemaining = fieldSize-in.size();
		size_t leftCharsRemaining = (size_t)floor((fieldSize-in.size()) / 2.0);
		size_t rightCharsRemaining = (size_t)ceil((fieldSize-in.size()) / 2.0);

		_tstring pad;			for(size_t i=0; i<charsRemaining; ++i) pad += _T(" ");
		_tstring padLeftHalf;	for(size_t i=0; i<leftCharsRemaining; ++i) padLeftHalf += _T(" ");
		_tstring padRightHalf;	for(size_t i=0; i<rightCharsRemaining; ++i) padRightHalf += _T(" ");

		switch(justify)
		{
			case JUSTIFY_CENTER:	return padLeftHalf + in + padRightHalf;
			case JUSTIFY_RIGHT:		return pad + in;
			case JUSTIFY_LEFT:
			default:				return in + pad;
		};
	}
}

_tstring toLowerCase(const _tstring &in)
{
	_tstring str(in);
	for(_tstring::iterator iter = str.begin(); iter != str.end(); ++iter)
		(*iter) = (TCHAR)tolower(*iter);
	return str;
}

/**
Projects a vector into screen coordinates
@param winx X-Coord on the window
@param winy Y-Coord on the window
@param winz The depth of the ray into the screen
@return World-coordinates of the unprojected vector
*/
vec3 Project(const vec3 &p)
{
	double modelMat[16], projMat[16];
	int viewport[4];
	double winx = 0, winy = 0, winz = 0;
	double objx = p.x, objy = p.y, objz = p.z;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

		viewport[0] = 0;
		viewport[1] = 0;
		viewport[2] = 1024;
		viewport[3] = 800;

		// Use the game camera's modelview matrix
		const mat4 &modl = g_Camera.getTransformation();
		for(int i=0; i<16; ++i)
		{
			modelMat[i] = modl[i];
		}

		// Use the perspective transformation for the projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			gluPerspective(45, 1024.0f / 768.0f, 0.01f, 5000.0f);
			glGetDoublev(GL_PROJECTION_MATRIX, projMat);
		glPopMatrix();

		// Now project the point to window coords
		gluProject(objx, objy, objz, modelMat, projMat, viewport, &winx, &winy, &winz);

	glPopAttrib();

	return vec3((float)winx, (float)winy, (float)winz);
}

/**
Projects a ray onto the screen from the mouse cursor's position
@param winx X-Coord on the window
@param winy Y-Coord on the window
@param winz The depth of the ray into the screen
@return World-coordinates of the unprojected vector
*/
vec3 UnProject(int winx, int winy, float winz)
{

	GLdouble x=0.0, y=0.0, z=0.0;

	GLint MouseX = winx;
	GLint MouseY = winy;

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	// Get the modelview matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	// Get the projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	// Fill out the viewport matrix
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Get the window coordinates of the mouse cursor
	GLdouble wx = GLdouble(MouseX);
	GLdouble wy = GLdouble( viewport[3] - MouseY - 1 );

	// Perform the unproject
	gluUnProject(wx, wy, winz, modelMatrix, projMatrix, viewport, &x, &y, &z);

	return vec3( float(x), float(y), float(z) );
}

/**
Projects a ray onto the screen from the mouse cursor's position
@param winz The depth of the ray into the screen
@return World-coordinates of the unprojected vector
*/
vec3 UnProject(float winz)
{
	GLdouble x=0.0, y=0.0, z=0.0;

	GLint MouseX = g_Input.MouseX;
	GLint MouseY = g_Input.MouseY;

	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	// Get the modelview matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

	// Get the projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	// Fill out the viewport matrix
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Get the window coordinates of the mouse cursor
	GLdouble winx = GLdouble(MouseX);
	GLdouble winy = GLdouble( viewport[3] - MouseY - 1 );

	// Perform the unproject
	gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, &x, &y, &z);

	return vec3( float(x), float(y), float(z) );
}

}; // namespace

