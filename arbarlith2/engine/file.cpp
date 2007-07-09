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

#include <sys/types.h>
#include <sys/stat.h> // System calls for file info

#ifdef _WIN32

#include <shlobj.h> // SHGetFolderPath
#include <shlwapi.h> // PathAppend
#include <direct.h> // _tchdir and _tgetcwd
#define stat _stat

#ifdef _UNICODE
#define _tstrcpy_s wcscpy_s
#else
#define _tstrcpy_s strcpy_s
#endif

#else

#define strcpy _tstrcpy_s

#endif

#ifndef W_OK
#define W_OK _S_IREAD
#endif

#ifndef R_OK
#define R_OK _S_IWRITE
#endif

namespace Engine {

////////////////////////////////////////////////////////////////////////////////
//                                   Functions                                //
////////////////////////////////////////////////////////////////////////////////

_tstring toLowerCase(const _tstring &in); // stdafx.cpp

void createDirectory(const _tstring &path)
{
#ifdef _WIN32
	_tmkdir(path.c_str());
#else
	mkdir(toAnsiString(path).c_str());
#endif
}

bool setWorkingDirectory(const _tstring &path)
{
#ifdef _WIN32
	return _tchdir(path.c_str()) != 0;
#else
	return chdir(toAnsiString(path).c_str()) != 0;
#endif
}

_tstring getWorkingDirectory(void)
{
#ifdef _WIN32
	TCHAR *pszWorkingDirectory = _tgetcwd(0,0);
#else
	char *pszWorkingDirectory = getcwd(0,0);
#endif

	_tstring workingDirectory = toTString(pszWorkingDirectory);

	free(pszWorkingDirectory);

	return workingDirectory;
}

_tstring getAppDataDirectory(void)
{
	TCHAR homeDir[MAX_PATH] = {0};

	bool result = false;

#ifdef _WIN32
	result = SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, homeDir));
#else
	result = true;
	strcpy(homeDir, "~/");
#endif

	if(result)
	{
		_tstring finalPath = pathAppend(homeDir, _T("arbarlith2"));
		createDirectory(finalPath); // Ensure that the directory exists
		return finalPath;
	}
	else
	{
		return _tstring(_T("./"));
	}
}

_tstring getApplicationDirectory(void)
{
#ifdef _WIN32
	TCHAR pathBuffer[_MAX_PATH];

	if(GetModuleFileName(GetModuleHandle(NULL), pathBuffer, _MAX_PATH-1) != 0)
	{
		// Strip off the filename and extension
		size_t i;
		for(i = _tcslen(pathBuffer) - 1; i > 0; --i)
		{
			if(pathBuffer[i]==_T('\\'))
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

	return _T(".\\");
#else
#pragma error("Need LINUX port of function: getApplicationDirectory")
return _T("./");
#endif
}

_tstring pathAppend(const _tstring &path, const _tstring &fileName)
{
	const TCHAR lastChar = path.at(path.length()-1);

	if(lastChar != _T('/') && lastChar != _T('\\'))
	{
		return File::fixFilename(path + _T("/") + fileName);
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

bool File::openFile(const _tstring &fileName, bool binary)
{
	PROFILE

	if(fileName.empty())
	{
		FAIL(_T("Empty filename"));
		return false;
	}


	this->fileName = fileName;


	// Does the file exist?
	if(!isFileOnDisk(fileName))
	{
		return false;
	}


	// Check for read permissions
	if(!hasAccess(fileName, ACCESS_MODE_READ))
	{
		FAIL(_T("Failed to obtain read permissions for file: ") + fileName);
		return false;
	}


	// Open the file
	FILE *fp = 0;

	if(binary)
	{
		fp = _tfopen(fileName.c_str(), _T("rb"));
	}
	else
	{
		fp = _tfopen(fileName.c_str(), _T("r"));
	}

	if(fp==0 || ferror(fp)!=0 || feof(fp))
	{
		FAIL(_tstring(_T("Failed to load file: ")) + fileName);
		return false;
	}

#ifdef _DEBUG
	TRACE(_tstring(_T("Loading file: ")) + fileName);
#endif

	// Get the length of the file
	size_t size = getFileLength(fp);

	if(size>0)
	{
		reserve(size);
		fread(data, 1, size, fp);
	}

	fclose(fp);
	return true;
}

size_t File::getFileLength(FILE *fp)
{
	long size=0;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp)+1;
	fseek(fp, 0, SEEK_SET);

	return (size_t)size;
}

bool File::isFileOnDisk(const _tstring &fileName)
{
	return hasAccess(fileName, ACCESS_MODE_EXISTENCE);
}

bool File::hasAccess(const _tstring &fileName, File::ACCESS_MODE mode)
{
	struct stat info;

	const string ansiFileName = toAnsiString(fileName);
	const char *pszFileName = ansiFileName.c_str();

	if(stat(pszFileName, &info) == 0)
	{
		switch(mode)
		{
		case ACCESS_MODE_EXISTENCE: return true; // we have determined, by retrieving file stats, that the file exists
		case ACCESS_MODE_READ:      return 0 != (info.st_mode & R_OK);
		case ACCESS_MODE_WRITE:     return 0 != (info.st_mode & W_OK);
		case ACCESS_MODE_RW:        return 0 != (info.st_mode & (R_OK | W_OK));
		};
	}

	// Failed to obtain file info... The file probably doesn't exist
	return false;
}

bool File::saveFile(const _tstring &fileName, bool binary)
{
	bool fileExists = isFileOnDisk(fileName);
	bool writableFile = hasAccess(fileName, ACCESS_MODE_WRITE);

	if(fileExists)
	{
		if(!writableFile)
		{
			FAIL(_tstring(_T("Failed to obtain write permissions for file: ")) + fileName);
			return false;
		}
	}/*
	else
	{
		const _tstring pathName = File::getPath(fileName);
		bool writableDirectory = hasAccess(pathName, ACCESS_MODE_WRITE);

		if(!writableDirectory)
		{
			ASSERT(false, _tstring(_T("Failed to obtain permissions to create file: ")) + fileName);
			ERR(_tstring(_T("Failed to obtain permissions to create file: ")) + fileName);
			return false;
		}
	}*/

	FILE *fp = _tfopen(fileName.c_str(), binary?_T("wb"):_T("w"));

	if(fp==0 || ferror(fp)!=0)
	{
		fclose(fp);
		FAIL(_T("Failed to save file: ") + fileName);
		return false;
	}
	else
	{
		#ifdef _DEBUG
			TRACE(_tstring(_T("Saving file: ")) + fileName);
		#endif

		fwrite(data, 1, getSize(), fp);
		fclose(fp);
		return true;
	}
}

unsigned char File::getChar(void)
{
	ASSERT(cursor+1 < getSize(), _T("read would go past the end of the file"));

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
	ASSERT(pos < getSize(), _T("Attempted to access character out of the bounds of the file"));

	if(pos < getSize())
	{
		return data[pos];
	}
	else
	{
		ERR(_T("Attempted to access character out of the bounds of the file"));
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
			ASSERT(offset < getSize(), _T("seek would go past the end of the file"));
			cursor = offset;
		}
		break;

	case FILE_SEEK_CURRENT:
		ASSERT(cursor+offset < getSize(), _T("seek would go past the end of the file"));
		cursor += offset;
		break;

	case FILE_SEEK_END:
		ASSERT(getSize()-offset-1 < getSize(), _T("seek would go past the end of the file"));
		cursor = getSize()-offset-1;
		break;
	};

	return cursor;
}

size_t File::read(File &file, size_t count)
{
	ASSERT(cursor+count <= getSize(), _T("read would go past the end of the file"));

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
	ASSERT(cursor+count <= getSize(), _T("read would go past the end of the file"));

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

size_t File::write(const _tstring &s)
{
	const string &ansi = toAnsiString(s);
	const char * ansiptr = ansi.c_str();

	size_t ansiptr_len = strlen(ansiptr);

	ASSERT(ansiptr_len==ansi.size(), _T("Failed to convert string from Unicode to ANSI"));

	return write((unsigned char*)ansiptr, ansiptr_len);
}

void File::reserve(size_t size)
{
	ASSERT(size>0, _T("Size was invalid"));

	if(getSize() == 0 || data == 0)
	{
		// Allocate a buffer
		data = new unsigned char[size];

		// Record the size of the new buffer
		this->size = size;
	}
	else if(size > getSize())
	{
		// Preserve existing data
		unsigned char *temp = data;

		// Allocate a larger buffer
		data = new unsigned char[size];

		// Replace contents with the existing data
		memcpy(data, temp, this->size);

		// Record the size of the new buffer
		this->size = size;

		// Free the old buffer
		delete[] temp;
	}
}

_tstring File::getPath(const _tstring &fileName)
{
	size_t i;
	_tstring in = fixFilename(fileName);

	ASSERT(!fileName.empty(), _T("File name is blank"));

	for(i=in.size()-1; i>0 && in.at(i)!=_T('\\'); --i);

	_tstring out = in.substr(0, i+1);

	return out;
}

_tstring File::getPath(void) const
{
	return getPath(fileName);
}

_tstring File::getFilenameNoPath(void) const
{
	_tstring in = fixFilename(fileName);
	_tstring out;

	ASSERT(!fileName.empty(), _T("File name is blank"));

	for(size_t i=in.size()-1; i>0 && in.at(i)!=_T('\\'); --i)
	{
		out = in.at(i) + out;
	}

	return out;
}

_tstring File::getExtension(void) const
{
	return getExtension(fileName);
}

_tstring File::fixFilename(const _tstring &fileName)
{
	_tstring t = fileName;
	for_each(t.begin(), t.end(), if_(_T('/')==_1)[_1=_T('\\')]);
	return t;
}

size_t File::findExtensionDelimeter(const _tstring &fileName)
{
	for(size_t i=0; i<fileName.length(); ++i)
	{
		TCHAR c = fileName[fileName.length() - i - 1];

		if(c == _T('.'))
		{
			// Found the delimeter, return its index
			return fileName.length() - i - 1;
		}

		if(c == _T('\\') || c == _T('/'))
		{
			// Found that there was no delimeter, return the index of the end of the string
			return fileName.length();
		}
	}

	// Found that there was no delimeter, return the index of the end of the string
	return fileName.length();
}

_tstring File::stripExtension(const _tstring &fileName)
{
	return fileName.substr(0, findExtensionDelimeter(fileName));
}

_tstring File::getExtension(const _tstring &fileName)
{
	return fileName.substr(findExtensionDelimeter(fileName), fileName.length());
}

} // namespace Engine
