/*
Author: Andrew Fox
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
//#include <winsock.h>

int WINAPI WinMain(HINSTANCE ,  // hInstance
                   HINSTANCE ,  // hPrevInstance
                   LPSTR     ,  // lpCmdLine
                   int       )  // nCmdShow
{
	// In dual core systems, restrict to one processor
	SetThreadAffinityMask(NULL, 1);

	setWorkingDirectory(getApplicationDirectory());

	srand((unsigned int)GetTickCount()); // Seed the random number generator

	// Setup sockets
	/*WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);*/

	// Are we running in safe mode?
	if(GetSystemMetrics(SM_CLEANBOOT)!=0)
	{
		MessageBox(0, _T("You are running this application in Safe mode!"), _T("Warning!"), MB_OK);
	}

	// Are we running from a network terminal?
	if(GetSystemMetrics(SM_REMOTECONTROL)!=0 || GetSystemMetrics(SM_REMOTESESSION)!=0)
	{
		MessageBox(0, _T("You are running this application from a remote session, performance will be poor."), _T("Warning!"), MB_OK);
	}

	// Run the game
	Engine::Application app;

	// exit
    return 0;  
}
