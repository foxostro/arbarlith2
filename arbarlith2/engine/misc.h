/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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

#ifndef MISC_H
#define MISC_H

#include <cmath>
#include <float.h>

#include <vector>
using std::vector;

#include "tstring.h"

namespace Engine {

#ifndef M_PI
#define M_PI ((double)3.14159265)
#endif

#ifndef M_E
#define M_E ((double)2.71828183)
#endif

/** Functor that deletes the 2nd part of the pair */
struct delete_second {
	template <class U> struct sig { typedef void type; };
	template <class FIRST, class SECOND>
	void operator()(pair<FIRST,SECOND> &a) const
	{
		delete a.second;
	}
};

/** Functor that deletes the argument */
struct delete_arg {
	template <class U> struct sig { typedef void type; };
	template <class ARG>
	void operator()(ARG &a) const
	{
		delete a;
	}
};

/**
Converts a value from a string to a float
@param s The _tstring representation of the number
@return Some numerical value
*/
float stof(const _tstring &s);

/**
Represents some value as a _tstring
@param i Some value
@return The _tstring representation of the number
*/
_tstring itoa(int i);

/**
Represents some value as a _tstring
@param f Some value
@param dec Places after the decimal place
@return The _tstring representation of the number
*/
_tstring ftoa(float f, int dec=4);

/** Specifes how field should be justified as they are padded in the fitToFieldSize method */
enum JUSTIFY
{
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER
};

/**
Pads a string and justifies it if it is less than the field size.
If it i larger than the field size, then the string is cropped at the field length
@param fieldSize number of character in the field
*/
_tstring fitToFieldSize(const _tstring &in, size_t fieldSize, JUSTIFY justify);

/**
Makes a string all lowercase
@param in The input string
@return The lower case string
*/
_tstring toLowerCase(const _tstring &in);

void Tokenize(const _tstring& str, vector<_tstring>& tokens, const _tstring& delimiters = _T(" \t\n"));

_tstring replace(const _tstring &source, const _tstring &find, const _tstring &replace);

} // namespace


#endif
