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

#if _MSC_VER > 1000
#pragma once
#endif



#include <string>
using std::string;
using std::wstring;


#include <sstream>
using std::stringstream;
using std::wstringstream;



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


#ifdef _UNICODE
#define toTString toUnicodeString
#define STRINGIFY(a) toUnicodeString(#a)
#else
#define toTString toAnsiString
#define STRINGIFY(a) toAnsiString(#a)
#endif


#endif