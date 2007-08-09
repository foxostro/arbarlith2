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

#include <iostream>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Engine {

void PrintStringToLog(const string &s)
{
	static bool firstTime = true;
	static fstream stream;

	if(firstTime)
	{
		firstTime = false;

        const string appDir = getAppDataDirectory();

		const string logFileName = pathAppend(appDir, "log.txt");

		stream.open(logFileName.c_str(), ios::out);

		if(!stream)
		{
			cerr << "Failed to create log file: " << logFileName << endl;
		}
		else
		{
            cout << "Redirecting std::clog to file: " << logFileName << endl;
			clog.rdbuf(stream.rdbuf()); // redirect clog to file
		}

		// Create a header for the log file
		clog << "=============================================" << endl
             << "=                                           =" << endl
#ifdef _DEBUG
             << "=               Debug Build                 =" << endl
#else
             << "=              Release Build                =" << endl
#endif
             << "=                                           =" << endl
             << "=============================================" << endl << endl;
	}

#ifdef _WIN32
	// Print message to debugger message window
	OutputDebugString(string(s + "\n\n").c_str());
#endif

	clog << s << endl << endl;
}

void Log(const string &message,
         const string &function,
         const string &file,
         const int line)
{
	// Format the time stamp.
	time_t curTime = std::time(NULL);
	char timestamp[32];
	std::strftime(timestamp,
	              sizeof(timestamp),
				  "%Y.%m.%dT%H:%M:%S",
				  localtime(&curTime));

	PrintStringToLog
	(
		function + "  ->  " + message +

        "\n\t" + File::getFilenameNoPath(file) + ":" + itoa(line) +

		"\n\t" + timestamp + "\n"
	);
}

} // namespace Engine
