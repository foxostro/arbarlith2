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

#ifdef _WIN32
#define STRICT
#include <windows.h>
#endif

#include "stdafx.h"
#include "logstring.h"
#include "myassert.h"

namespace Engine {

bool assertionFailed(int lineNumber, const TCHAR *pszFileName, const _tstring &message)
{
	const _tstring fullMessage = message +
				     _T("\nFile: ") + pszFileName +
				     _T("\nLine: ") + itoa(lineNumber);

	::Engine::getMessageLogger().log(_T("Assertion"), fullMessage);

	bool result = false;

#ifndef _WIN32
	abort();
#else
	char *pszMessage = toAnsiCharArray(message);
	char *pszFullMessage = toAnsiCharArray(fullMessage);

#ifdef _MSC_VER
	result = (1 == _CrtDbgReport(_CRT_ASSERT, pszFileName, lineNumber, NULL, pszMessage));
#else
	switch(MessageBox(NULL,
	                  pszFullMessage,
	                  IsDebuggerPresent() ? "Assert - Debugger Attached"
	                                      : "Assert",
	                  MB_ABORTRETRYIGNORE |
	                  MB_ICONERROR |
	                  MB_SETFOREGROUND |
	                  MB_TOPMOST))
	{
	case IDIGNORE:
		result = false; // allow execution to continue
		break;

	case IDRETRY:
		result = true; // signals that we should call DebugBreak
		break;

	case IDABORT:
		abort();
		break;

	default:
		ERR(_T("Unexpected input from MessageBox, aborting..."));
		abort();
		break;
	};
#endif

	delete [] pszMessage;
	pszMessage=0;

	delete [] pszFullMessage;
	pszFullMessage=0;
#endif

	return result;
}

} // namespace Engine
