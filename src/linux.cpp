/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009,2010 Game Creation Society
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

#ifndef _WIN32 // FIXME: What about the Mac?

#include "stdafx.h"

#include <signal.h>
#include <execinfo.h>

extern "C" void printBackTrace(FILE *fp)
{
	int i;
	void *bt[256];
	char **strings;
	int size;

	fprintf(fp, "################### Backtrace ####################\n");

	size = backtrace(bt, 256);
	strings = backtrace_symbols(bt, size);

	fprintf(fp, "Number of elements in backtrace: %d\n", size);

	for(i = 0; i < size; ++i)
	{
		fprintf(fp, "%s\n", strings[i]);
	}

	fprintf(fp, "##################################################\n");
}

extern "C" void handler(int, siginfo_t*, void*)
{
	printBackTrace(stdout);
	printf("Handler done.\n");
	abort();
}

void setupSignalHandler(void)
{
	struct sigaction SignalAction;

	SignalAction.sa_sigaction = handler;
	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &SignalAction, 0);
}

int main(int argc, char *argv[])
{
	setupSignalHandler();

    g_pApplication = new Engine::Application();
    g_pApplication->start();
    g_pApplication->run();
    delete g_pApplication;

	return EXIT_SUCCESS;
}

#endif
