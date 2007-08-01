// SearchFile.h: interface for the SearchFile class.
// Desc: Searches for files matching a given pattern
// Author: Andrew Fox
//
// Win32 Version:
// Based on the SearchFile class in the Titan Engine by Ignacio Castaño Aguado
// Also based on the _find, _wfind example from MSDN
//
// Linux Version:
// Not written yet
///////////////////////////////////////////////////////////////////////////////

#ifndef _SEARCHFILE_INCLUDE
#define _SEARCHFILE_INCLUDE


namespace Engine {


class SearchFile
{
public:
	SearchFile(string strPattern="*");

	std::vector<string> m_Files;
};


}; // namespace


#endif
