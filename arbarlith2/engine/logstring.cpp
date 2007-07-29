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

#include "stdafx.h"
#include "file.h"
#include "LogString.h"

#ifdef _WIN32
#include "windows.h"
#endif

namespace Engine {

LogString& getMessageLogger(void)
{
	static LogString *logger = new LogString; // HACK: this memory is never free'd
	return(*logger);
}

LogString::LogString(void)
{
	const string logFileName = toAnsiString(pathAppend(getAppDataDirectory(), _T("log.txt")));

	stream.open(logFileName.c_str(), ios::out);

	// Create a log file header
	stream << "=============================================\n"
	       << "=                                           =\n"
#ifdef _DEBUG
	       << "=               Debug Build                 =\n"
#else
	       << "=              Release Build                =\n"
#endif
	       << "=                                           =\n"
	       << "=============================================\n\n";
	stream.flush();
}

void LogString::addString(const _tstring &s)
{
	stream << s;
	stream.flush(); // flush message to disk

#ifdef _WIN32
	OutputDebugString(s.c_str());
#endif
}

void LogString::log(const _tstring &origin, const _tstring &message)
{
	addString(origin + _T("  ->  ") + message + _T("\n\n"));
}

} // namespace Engine
