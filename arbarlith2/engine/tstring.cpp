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

#include "stdafx.h"

wstring toUnicodeString(const string &str)
{
#ifdef _WIN32

	const char *ansistr = str.c_str();

	int lenA = lstrlenA(ansistr);

	if(lenA > 0)
	{
		int lenW = ::MultiByteToWideChar(CP_ACP, 0, ansistr, lenA, 0, 0);

		if(lenW > 0)
		{
			BSTR unicodestr = ::SysAllocStringLen(0, lenW);

			if(unicodestr != 0)
			{
				::MultiByteToWideChar(CP_ACP, 0, ansistr, lenA, unicodestr, lenW);
				wstring s = unicodestr;
				::SysFreeString(unicodestr);
				return s;
			}
		}
	}

	return wstring();

#else

	/*
	Slower than the Windows version (???), but at least easier to read!
	*/

	wstring r;

	r.reserve(str.size());

	for(size_t i=0; i<str.size(); ++i)
	{
		r += (wchar_t)str[i];
	}

	return r;

#endif
}

wstring toUnicodeString(const wstring &unicodestr)
{
	return unicodestr;
}

string toAnsiString(const string &ansistr)
{
	return ansistr;
}

string toAnsiString(const wstring &unicodestr)
{
	char *ansistr = toAnsiCharArray(unicodestr);
	string s = ansistr;
	delete [] ansistr;
	return s;
}

char* toAnsiCharArray(const string &ansistr)
{
	char *dst = new char[ansistr.size()+1];

	for(size_t i=0; i<ansistr.size(); ++i) dst[i] = ansistr[i];
	dst[ansistr.size()] = 0;

	ASSERT(strcmp(dst, ansistr.c_str())==0, _T("Failed to duplicate ANSI string \"") + ansistr + _T("\""));

	return dst;
}

char* toAnsiCharArray(const wstring &unicodestr)
{
	size_t lenW = wcslen(unicodestr.c_str());
	char *ansistr = new char[lenW+1];

	for(size_t i=0; i<lenW; ++i)
	{
		ansistr[i] = static_cast<char>(unicodestr[i]);
	}
	ansistr[lenW] = 0;

	ASSERT(strlen(ansistr)==lenW, _T("strlen(ansistr) != lenW"));

	return ansistr;
}

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

_tstring replace(const _tstring &source,
                 const _tstring &find,
                 const _tstring &replace)
{
    _tstring output = source;

    for(size_t j = 0; (j=source.find(find, j)) != _tstring::npos; ++j)
    {
        output.replace(j, find.length(), replace);
    }

    return output;
}

void tokenize(const _tstring& str,
              vector<_tstring>& tokens,
              const _tstring& delimiters)
{
    // Skip delimiters at beginning.
    _tstring::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    _tstring::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (_tstring::npos != pos || _tstring::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);

        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

} // namespace Engine
