// SearchFile.h: interface for the SearchFile class.
// Desc: Searches for files matching a given pattern
// Author: Andrew Fox
// 
// Based on the SearchFile class in the Titan Engine by Ignacio Castaño Aguado
// Also based on the _find, _wfind example from MSDN
//
// Win32 Specific calls are used:
//		_finddata_t
//		_findfirst
//		_findnext
//		_findclose
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <io.h> // for _findfile functions

#include "searchfile.h"


namespace Engine { 



SearchFile::SearchFile(_tstring strPattern)
{
	// Reset the archive name vector
	m_Files.clear();

	// Search for all files matching the pattern
	struct _tfinddata_t file;
	long hFile = (long)_tfindfirst(strPattern.c_str(), &file);

	if(hFile != -1L)
	{
		// Get the files matching the pattern
		do m_Files.push_back(file.name);
		while(_tfindnext( hFile, &file ) == 0);

		// We are done with the handle
		_findclose(hFile);
	}
}


}; // namespace
