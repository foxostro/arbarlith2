/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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
#	define STRICT
#	include <windows.h>
#else
#	include <iostream>
#	define IDIGNORE 1
#	define IDRETRY 2
#	define IDABORT 3
#endif

#include "stdafx.h"
#include "logstring.h"
#include "myassert.h"

namespace Engine {

void DebugBreak(void)
{
#ifdef _WIN32
	::DebugBreak();
#endif
}

bool assertionFailed(int lineNumber,
                     const char *pszFileName,
                     const string &message)
{
	bool result = false;
	int response = IDABORT;

	const string fullMessage = message +
				     "\nFile: " + pszFileName +
				     "\nLine: " + itoa(lineNumber);

	Log(message, "Assertion", pszFileName, lineNumber);

#ifndef _WIN32
	std::cout << fullMessage << endl
			  << endl
	          << "Choose a response:" << endl
	          << "\t1) Ignore - Continues execution, risking a crash" << endl
	          << "\t2) Retry  - Trips the debugger, if attached" << endl
	          << "\t3) Abort  - Aborts execution immediately" << endl
	          << ">";
	std::cout.flush();
	std::cin >> response;
	std::cout << endl;
#else
	{
		char *pszFullMessage = strdup(fullMessage);

		response = MessageBox(NULL,
							  pszFullMessage,
							  "Assert",
							  MB_ABORTRETRYIGNORE |
							  MB_ICONERROR |
							  MB_SETFOREGROUND |
							  MB_TOPMOST);

		delete [] pszFullMessage;
		pszFullMessage=0;
	}
#endif

	switch(response)
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
		ERR("Unexpected input! Aborting...");
		abort();
		break;
	};

	return result;
}

} // namespace Engine
