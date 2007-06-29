/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright � 2006-2007 Game Creation Society
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
	size_t lenW = wcslen(unicodestr.c_str());
	char *ansistr = new char[lenW+1];

	for(size_t i=0; i<lenW; ++i)
	{
		wchar_t c = unicodestr[i];

		if(c > 0 && c < 128)
		{
			ansistr[i] = (char)c;
		}
		else
		{
			ansistr[i] = '?';
		}
	}
	ansistr[lenW] = 0;

	string s = ansistr;

	size_t len1 = strlen(ansistr);
	size_t len2 = s.size();
	ASSERT(len1==len2, _T("len1==len2"));
	ASSERT(len1==lenW, _T("len1==lenW"));

	// ...use the strings, then free their memory:
	delete [] ansistr;

	return s;
}
