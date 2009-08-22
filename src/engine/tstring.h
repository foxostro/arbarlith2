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

#ifndef TSTRING_H
#define TSTRING_H

#include <string>
#include <vector>

namespace Engine {

/**
Copies the given std::string into a new C std::string allocated on the heap.d
@param s source std::string
@return C std::string (Null-terminated) allocated in the heap. Free it with delete[]
*/
char* strdup(const std::string &s);

/** Represents some value as a std::string */
std::string itoa(int i);

/** Represents some value as a std::string */
std::string ftoa(float f, int dec=4);

/** Converts a value from a std::string to a integer */
int stoi(const std::string &s);

/** Converts a value from a std::string to a float */
float stof(const std::string &s);

/** Specifies how a field should be justified in the fitToFieldSize method */
enum JUSTIFY
{
    JUSTIFY_LEFT,
    JUSTIFY_RIGHT,
    JUSTIFY_CENTER
};

/**
Pads a std::string and justifies it if it is less than the field size. If it is
larger than the field size, then the std::string is cropped at the field length
@param fieldSize number of character in the field
*/
std::string fitToFieldSize(const std::string &in, size_t fieldSize, JUSTIFY justify);

/**
Makes a std::string all lowercase
@param in The input std::string
@return The lower case std::string
*/
std::string toLowerCase(const std::string &in);

/**
Replaces all occurrences of a substring with a replacement std::string.
@param source Source std::string
@param find substring to search for
@param replace Replacement std::string.  Substituted over the 'find' std::string
@return result std::string
*/
std::string replace(const std::string &source,
                    const std::string &find,
                    const std::string &replace);

} // namespace Engine

#endif
