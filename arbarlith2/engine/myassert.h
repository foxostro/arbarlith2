/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright � 2003-2007 Game Creation Society
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

//#ifndef _ASSERT_H_
//#define _ASSERT_H_


namespace Engine {

/**
Tests an assertion
@param line The line of the assertion
@param pszfileName The file name of the file in which the assertion occurred
@param message A message describing the assertion
*/
void assertionFailed(int line, const TCHAR *pszfileName, const _tstring &message);

} // namespace Engine



// Enable the assertion macro in release mode too
#ifndef _ENABLE_ASSERTS_
#define _ENABLE_ASSERTS_ 0
#endif

// The ASSERT macro
#ifndef ASSERT

#if(_ENABLE_ASSERTS_ || _DEBUG)
#	ifdef _WIN32
#		define ASSERT(exp, msg) if((bool)(exp) == false)  {  ::Engine::assertionFailed((int)(__LINE__), _T(__FILE__), (msg));  }
#	else
#		include <assert.h>
#		define ASSERT(exp, msg) assert(exp);
#	endif
#else
#	define ASSERT(exp, str)   ;
#endif

#endif



// Causes an assertion failure (even in Release mode)
#ifndef FAIL
#define FAIL(msg) \
{                                                                                       \
	::Engine::getMessageLogger().log( _tstring(_T(__FUNCTION__)), _tstring(msg) );  \
	Engine::assertionFailed((int)(__LINE__), _T(__FILE__), (msg));                  \
}
#endif


//#endif
