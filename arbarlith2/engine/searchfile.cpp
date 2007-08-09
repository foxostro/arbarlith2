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

#include <iostream>
using namespace std;

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif

#include "myassert.h"
#include "file.h"
#include "searchfile.h"

vector<string> Engine::SearchFile(const string &_searchDirectory,
                                  const string &fileExtension)
{
	vector<string> filesFound;

	// Ensure standard file path separators, etc
	string searchDirectory = File::fixFilename(_searchDirectory);

#ifdef _WIN32
	// Search for all files matching the pattern
	struct _finddata_t file;
	long hFile = (long)_findfirst(searchDirectory.c_str(), &file);

	if(hFile != -1L)
	{
		do
		{
			if(File::getExtension(file.name) == fileExtension)
			{
				filesFound.push_back(file.name);
			}
		}
		while(_findnext( hFile, &file ) == 0);

		// We are done with the handle
		_findclose(hFile);
	}
#else
	struct dirent *directoryEntry = 0;
	DIR *directory = opendir(searchDirectory.c_str());

	while((directoryEntry = readdir(directory)) != 0)
	{
		if(File::getExtension(directoryEntry->d_name) == fileExtension)
		{
			filesFound.push_back(directoryEntry->d_name);
		}
	}

	closedir(directory);
#endif

	return filesFound;
}
