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
#include "profile.h"

#include <fstream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h> // System calls for file info

#ifdef _WIN32
#
#	include <shlobj.h>  // SHGetFolderPath
#	include <shlwapi.h> // PathAppend
#	include <direct.h>  // _tchdir and _tgetcwd
#
#	define stat _stat
#
#   define PATH_SEPARATOR ( '\\' )
#
#   ifndef W_OK
#       define W_OK _S_IREAD
#   endif
#
#   ifndef R_OK
#       define R_OK _S_IWRITE
#   endif
#
#else
#
#   define PATH_SEPARATOR ( '/' )
#
#endif

namespace Engine {

////////////////////////////////////////////////////////////////////////////////
//                                   Functions                                //
////////////////////////////////////////////////////////////////////////////////

string toLowerCase(const string &in); // stdafx.cpp

void createDirectory(const string &path)
{
    TRACE(path);

    int result = -1;

#ifdef _WIN32
	result = _mkdir(path.c_str());
#else
    result = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#endif

	if(result < 0)
	{
		if(errno == EEXIST)
		{
			TRACE("Directory exists, but that's OK: " + path);
		}
		else
		{
			perror("mkdir() failed");
			ERR(string("mkdir() failed: ") + strerror(errno));
		}
	}
}

bool setWorkingDirectory(const string &path)
{
    TRACE(path);

    int result = -1;

#ifdef _WIN32
    result = _chdir(path.c_str());
#else
    result = chdir(path.c_str());
#endif

	if(result < 0)
	{
		perror("chdir() failed");
		ERR("chdir() failed, error logged to std::err");

		return false;
	}
	else
	{
		return true;
	}
}

string getWorkingDirectory(void)
{
	char *pszWorkingDirectory = 0;

#ifdef _WIN32
	pszWorkingDirectory = _getcwd(0,0);
#else
	pszWorkingDirectory = getcwd(0,0);
#endif

	string workingDirectory = pszWorkingDirectory;

	free(pszWorkingDirectory);

	return workingDirectory;
}

string getAppDataDirectory(void)
{
	string finalPath = "./";

#ifdef _WIN32
	char homeDir[MAX_PATH] = {0};


	if (SUCCEEDED(SHGetFolderPath(NULL,
	                              CSIDL_APPDATA | CSIDL_FLAG_CREATE,
	                              NULL,
	                              0,
	                              homeDir)))
	{
		finalPath = pathAppend(homeDir, "arbarlith2");
	}

#else
    struct passwd *pwd = getpwuid(getuid());

    if(pwd == 0)
    {
        perror("Failed to retrieve user information");
        finalPath = "./.arbarlith2";
    }
    else
    {
		finalPath = pathAppend(pw->pw_dir, ".arbarlith2");
    }

#endif

	// Ensure that the directory exists
	createDirectory(finalPath);

	return finalPath;
}

string getApplicationDirectory(void)
{
#ifdef _WIN32

	char pathBuffer[_MAX_PATH];

	if(GetModuleFileName(GetModuleHandle(NULL), pathBuffer, _MAX_PATH-1) != 0)
	{
		// Strip off the filename and extension
		size_t i;
		for(i = strlen(pathBuffer) - 1; i > 0; --i)
		{
			if(pathBuffer[i] == '\\')
			{
				pathBuffer[i]=0;
				break;
			}
		}

		if(i != 0)
		{
			return pathBuffer;
		}
	}

	return ".\\";

#else
    
    return "/home/arfox/arbarlith2/trunk/arbarlith2/bin/";

#endif
}

string pathAppend(const string &path, const string &fileName)
{
	const char lastChar = path.at(path.length()-1);

	if(lastChar != '/' && lastChar != '\\')
	{
		return File::fixFilename(path + PATH_SEPARATOR + fileName);
	}
	else
	{
		return File::fixFilename(path + fileName);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

File::File(const File &)
{
	// Do nothing
	clear();
}

void File::clear(void)
{
	data = 0;
	size = 0;
	cursor = 0;
	fileName.clear();
}

void File::destroy(void)
{
	free(data);
	clear();
}

bool File::openFile(const string &_fileName, bool binary)
{
	const string fileName = fixFilename(_fileName);

	// Does the file exist?
	if(!isFileOnDisk(fileName))
	{
		ERR("File does not exist: " + fileName);

		// continue and try to open the file anyway...
	}

	ifstream file(fileName.c_str(), (binary) ? (ios::in|ios::binary) : (ios::in));

	if(!file)
	{
		FAIL("Failed to open file: " + fileName);
		return false;
	}

	if(binary)
	{
		TRACE(string("Loading binary file: ") + fileName);

		const streamsize size = getBytesOnDisk(fileName);

		if(size>0)
		{
			reserve(size);
			file.read((char*)data, size);

			if(file.gcount() != size)
			{
				FAIL("Failed to load binary file: " + fileName);
				return false;
			}
		}
	}
	else
	{
		string line;

		if(file)
		{
			while(getline(file, line))
			{
				write(line + "\n");
			}
		}

		seek(0, FILE_SEEK_BEGIN);
	}

	this->fileName = fileName;

	TRACE("Loaded text file: " + fileName);
	return true;
}

streamsize File::getBytesOnDisk(const string &fileName)
{
	struct stat info;

	if(stat(fileName.c_str(), &info) == 0)
	{
		return static_cast<streamsize>(info.st_size);
	}

	FAIL("Failed to obtain file info! The file probably doesn't exist");
	return 0;
}

bool File::isFileOnDisk(const string &fileName)
{
	struct stat info;

	// if we can stat the file, then it does exist
	return (stat(fileName.c_str(), &info) == 0);
}

bool File::saveFile(const string &fileName, bool binary)
{
	ofstream file(fileName.c_str(),
	              (binary) ? (ios::out|ios::binary) : (ios::out));

	if(!file)
	{
		FAIL("Failed to open file: " + fileName);
		return false;
	}

	TRACE("Saving file: " + fileName);

	if(!file.write((char*)data, (streamsize)getSize()))
	{
		FAIL("Failed to save file: " + fileName);
		return false;
	}

	return true;
}

unsigned char File::getChar(void)
{
	ASSERT(cursor+1 < getSize(), "read would go past the end of the file");

	unsigned char c = peekChar();
	cursor++;
	return c;
}

unsigned char File::peekChar(void)
{
	unsigned char c = peekChar(tell());
	return c;
}

unsigned char File::peekChar(size_t pos)
{
	ASSERT(pos < getSize(), "Cannot peek out of bounds");

	if(pos < getSize())
	{
		return data[pos];
	}
	else
	{
		ERR("Attempted to access character out of the bounds of the file");
		return 0;
	}
}

bool File::loaded(void) const
{
	return(data != 0 && getSize()>0);
}

bool File::endOfFile(void) const
{
	return(cursor >= getSize());
}

size_t File::seek(size_t offset, FILE_SEEK origin)
{
	switch(origin)
	{
	case FILE_SEEK_BEGIN:
		if(offset==0)
		{
			cursor = 0;
		}
		else
		{
			ASSERT(offset < getSize(), "seek would go past the end of the file");
			cursor = offset;
		}
		break;

	case FILE_SEEK_CURRENT:
		ASSERT(cursor+offset < getSize(), "seek would go past the end of the file");
		cursor += offset;
		break;

	case FILE_SEEK_END:
		ASSERT(getSize()-offset-1 < getSize(), "seek would go past the end of the file");
		cursor = getSize()-offset-1;
		break;
	};

	return cursor;
}

size_t File::read(File &file, size_t count)
{
	ASSERT(cursor+count <= getSize(), "read would go past the end of the file");

	if(count>0)
	{
		file.write(data+tell(), count);
		cursor += count; // may cause EOF
		return count;
	}
	else
	{
		return 0;
	}
}

size_t File::read(unsigned char * buffer, size_t count)
{
	ASSERT(cursor+count <= getSize(), "read would go past the end of the file");

	if(count>0)
	{
		memcpy(buffer, data+cursor, count);
		cursor += count; // may cause EOF
		return count;
	}
	else
	{
		return 0;
	}
}

size_t File::write(unsigned char * buffer, size_t count)
{
	if(tell()+count > getSize())
	{
		reserve(tell()+count);
	}

	memcpy(data+tell(), buffer, count);

	cursor += count; // may cause EOF if we try to read from this position

	return count;
}

size_t File::write(const string &s)
{
	return write((unsigned char*)s.c_str(), s.size());
}

void File::reserve(size_t size)
{
	ASSERT(size>0, "Size was invalid");

	if(getSize() == 0 || data == 0)
	{
		// Allocate a buffer
		data = new unsigned char[size];
		memset(data, 0, sizeof(unsigned char) * size);

		// Record the size of the new buffer
		this->size = size;
	}
	else if(size > getSize())
	{
		// Preserve existing data
		unsigned char *temp = data;

		// Allocate a larger buffer
		data = new unsigned char[size];
		memset(data, 0, sizeof(unsigned char) * size);

		// Replace contents with the existing data
		memcpy(data, temp, this->size);

		// Record the size of the new buffer
		this->size = size;

		// Free the old buffer
		delete[] temp;
	}
}

string File::getPath(const string &fileName)
{
	size_t i;
	string in = fixFilename(fileName);

	ASSERT(!fileName.empty(), "File name is blank");

	for(i=in.size()-1; i>0 && in.at(i)!=PATH_SEPARATOR; --i);

	string out = in.substr(0, i+1);

	return out;
}

string File::getPath(void) const
{
	return getPath(fileName);
}

string File::getFilenameNoPath(const string &fileName)
{
    string in = fixFilename(fileName);
    string out;

    ASSERT(!fileName.empty(), "File name is blank");

    for(size_t i=in.size()-1; i>0 && in.at(i)!=PATH_SEPARATOR; --i)
    {
        out = in.at(i) + out;
    }

    return out;
}

string File::getFilenameNoPath(void) const
{
	return getFilenameNoPath(fileName);
}

string File::getExtension(void) const
{
	return getExtension(fileName);
}

string File::fixFilename(const string &fileName)
{
#ifdef _WIN32
	return replace(fileName, "/", "\\");
#else
	return replace(fileName, "\\", "/");
#endif
}

size_t File::findExtensionDelimeter(const string &fileName)
{
	for(size_t i=0; i<fileName.length(); ++i)
	{
		char c = fileName[fileName.length() - i - 1];

		if(c == '.')
		{
			// Found the delimiter, return its index
			return fileName.length() - i - 1;
		}

		if(c == '\\' || c == '/')
		{
			// Found that there was no delimiter, return the index of the end of the string
			return fileName.length();
		}
	}

	// Found that there was no delimiter, return the index of the end of the string
	return fileName.length();
}

string File::stripExtension(const string &fileName)
{
	return fileName.substr(0, findExtensionDelimeter(fileName));
}

string File::getExtension(const string &fileName)
{
	return fileName.substr(findExtensionDelimeter(fileName), fileName.length());
}

} // namespace Engine
