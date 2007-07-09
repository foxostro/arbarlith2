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
#include "resource.h" // Resource Header

#include "stdafx.h"
#include "logstring.h"
#include "myassert.h"

namespace Engine {

enum ASSERT_RESPONSE
{
ASSERT_ABORT,
ASSERT_DEBUG,
ASSERT_IGNORE,
ASSERT_IGNORE_ALL
};

ASSERT_RESPONSE g_AssertResponse;

_tstring g_strAssertLine;
_tstring g_strAssertFile;
_tstring g_strAssertMsg;

/**
assertion -> ignore?
We will use this to determine if the assertion is to be ignored when encountered
*/
map<_tstring, bool> AllAssertions;

LRESULT CALLBACK AssertProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM)
{
	switch(message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, IDC_FILE_LABEL,    g_strAssertFile.c_str());
		SetDlgItemText(hwnd, IDC_LINE_LABEL,    g_strAssertLine.c_str());
		SetDlgItemText(hwnd, IDC_MESSAGE_LABEL, g_strAssertMsg.c_str());
		//SetDlgItemText(hwnd, IDC_MSG_LABEL,     g_strAssertMsg.c_str());
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDABORT:
			g_AssertResponse = ASSERT_ABORT;
			EndDialog(hwnd, TRUE);
			return TRUE;

		case IDDEBUG:
			g_AssertResponse = ASSERT_DEBUG;
			EndDialog(hwnd, TRUE);
			return TRUE;

		case IDIGNORE:
			g_AssertResponse = ASSERT_IGNORE;
			EndDialog(hwnd, TRUE);
			return TRUE;

		case IDIGNOREALL:
			g_AssertResponse = ASSERT_IGNORE_ALL;
			EndDialog(hwnd, TRUE);
		};

	case WM_SYSCOMMAND:
		if(wParam == SC_CLOSE)
		{
			// assume the user wants to continue
			g_AssertResponse = ASSERT_IGNORE;

			EndDialog(hwnd, TRUE);
			return TRUE;
		}
	};

    return FALSE;
}

ASSERT_RESPONSE CreateAssertBox(int iLine, _tstring strFile, _tstring strMsg)
{
	g_strAssertLine = itoa(iLine);
	g_strAssertFile = strFile;
	g_strAssertMsg  = strMsg;

	ERR(_T("Assertion in File: \"") + g_strAssertFile + _T("\"\n") +
	    _T("Line #") + g_strAssertLine + _T("\n") +
	    _T("Message: \"") + g_strAssertMsg + _T("\""));

	DialogBox(GetModuleHandle(NULL), (LPCTSTR)IDD_ASSERT_DLG, NULL, (DLGPROC)AssertProc);

	return g_AssertResponse;
}

void assertionFailed(int iLine, const TCHAR *pszfileName, const _tstring &strMsg)
{
	map<_tstring, bool>::iterator iter;

	const _tstring &key = _tstring(pszfileName) + _T(" -> ") + itoa(iLine);

	// Has IGNORE ALL been requested previously?
	if((iter=AllAssertions.find(key))==AllAssertions.end() || (iter->second)==false)
	{
		// OK, so bring up a Dialog Box
		switch(CreateAssertBox(iLine, pszfileName, strMsg))
		{
		case ASSERT_IGNORE_ALL:
			if(AllAssertions.find(key) == AllAssertions.end())
				AllAssertions.insert(make_pair(key,true));
			break;
		case ASSERT_ABORT:  abort();      break;
		case ASSERT_DEBUG:  DebugBreak(); break;
		case ASSERT_IGNORE:               break;
		};
	}
}

} // namespace Engine

#endif
