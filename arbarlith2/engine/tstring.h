/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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
using std::string;
using std::wstring;

#include <sstream>
using std::stringstream;

typedef basic_stringstream<wchar_t> wstringstream;

#ifdef _UNICODE
typedef wstring _tstring;
typedef wstringstream _tstringstream;
#else
typedef string _tstring;
typedef stringstream _tstringstream;
#endif

wstring toUnicodeString(const string &ansistr);
wstring toUnicodeString(const wstring &unicodestr);

string toAnsiString(const string &ansistr);
string toAnsiString(const wstring &unicodestr);

char* toAnsiCharArray(const string &ansistr);
char* toAnsiCharArray(const wstring &unicodestr);

#ifdef _UNICODE
#define toTString toUnicodeString
#define STRINGIFY(a) toUnicodeString(#a)
#else
#define toTString toAnsiString
#define STRINGIFY(a) toAnsiString(#a)
#endif

namespace Engine {

/** Represents some value as a _tstring */
_tstring itoa(int i);

/** Represents some value as a _tstring */
_tstring ftoa(float f, int dec=4);

/** Converts a value from a string to a integer */
int stoi(const _tstring &s);

/** Converts a value from a string to a float */
float stof(const _tstring &s);

/** Specifies how a field should be justified in the fitToFieldSize method */
enum JUSTIFY
{
    JUSTIFY_LEFT,
    JUSTIFY_RIGHT,
    JUSTIFY_CENTER
};

/**
Pads a string and justifies it if it is less than the field size. If it is
larger than the field size, then the string is cropped at the field length
@param fieldSize number of character in the field
*/
_tstring fitToFieldSize(const _tstring &in, size_t fieldSize, JUSTIFY justify);

/**
Makes a string all lowercase
@param in The input string
@return The lower case string
*/
_tstring toLowerCase(const _tstring &in);

/**
Tokenizes a string according to given delimiter characters.
Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
    "Trick 15: Serialization Using XML Property Bags". Premier Press. 2002.
@param source Source string
@param tokens Returns the tokens found in the string
@param delimiters each character in this string is treated as a delimiter
*/
void tokenize(const _tstring& source,
              vector<_tstring>& tokens,
              const _tstring& delimiters = _T(" \t\n"));

/**
Replaces all occurrences of a substring with a replacement string.
@param source Source string
@param find substring to search for
@param replace Replacement string.  Substituted over the 'find' string
@return result string
*/
_tstring replace(const _tstring &source,
                 const _tstring &find,
                 const _tstring &replace);

} // namespace Engine

#endif
