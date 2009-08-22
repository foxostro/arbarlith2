/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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

#include "tstring.h"
#include "myassert.h"

#include <cmath>
#include <sstream>
#include <cstring>

namespace Engine {

char* strdup(const std::string &src)
{
	char *dst = new char[src.size()+1];

	for(size_t i=0; i<src.size(); ++i) dst[i] = src[i];
	dst[src.size()] = 0; // null-terminate the std::string

	ASSERT(strcmp(dst, src.c_str())==0,
		   "Failed to duplicate string: \"" + src + "\"");

	return dst;
}

int stoi(const std::string &s)
{
    int ret = 0;
    std::stringstream stream;

    stream << s;
    stream >> ret;

    return ret;
}

float stof(const std::string &s)
{
    float ret = 0.0f;
    std::stringstream stream;

    stream << s;
    stream >> ret;

    return ret;
}

std::string itoa(int i)
{
    std::string ret;
    std::stringstream stream;

    stream << i;
    stream >> ret;

    return ret;
}

std::string ftoa(float f, int dec)
{
    std::string ret;
    std::stringstream stream;

    stream.precision(dec);
    stream << f;
    stream >> ret;

    return ret;
}

std::string fitToFieldSize(const std::string &in, size_t fieldSize, JUSTIFY justify)
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

        std::string pad;			for(size_t i=0; i<charsRemaining; ++i) pad += " ";
        std::string padLeftHalf;	for(size_t i=0; i<leftCharsRemaining; ++i) padLeftHalf += " ";
        std::string padRightHalf;	for(size_t i=0; i<rightCharsRemaining; ++i) padRightHalf += " ";

        switch(justify)
        {
        case JUSTIFY_CENTER:	return padLeftHalf + in + padRightHalf;
        case JUSTIFY_RIGHT:		return pad + in;
        case JUSTIFY_LEFT:
        default:				return in + pad;
        };
    }
}

std::string toLowerCase(const std::string &in)
{
    std::string str(in);

    for(std::string::iterator iter = str.begin(); iter != str.end(); ++iter)
        (*iter) = (char)tolower(*iter);

    return str;
}

std::string replace(const std::string &source,
                 const std::string &find,
                 const std::string &replace)
{
    std::string output = source;

    for(size_t j = 0; (j=source.find(find, j)) != std::string::npos; ++j)
    {
        output.replace(j, find.length(), replace);
    }

    return output;
}

} // namespace Engine
