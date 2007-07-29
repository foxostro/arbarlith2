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

#ifndef _LOGSTRING_H_
#define _LOGSTRING_H_

#include <fstream>
#include "misc.h"

namespace Engine {

/** Message Logger */
class LogString
{
private:
	/** Output stream for the log */
	fstream stream;

	/**
	Add a string to the log file
	@param message Log string
	*/
	void addString(const _tstring &s);

public:
	/** Constructor */
	LogString(void);

	/**
	Add a message to the log
	@param origin Originating message
	@param message Log message
	*/
	void log(const _tstring &origin, const _tstring &message);
};

/**
Gets the global message logger
@return message logger
*/
LogString& getMessageLogger(void);

} // namespace Engine


// The TRACE macro will dump a _tstring to the log file in verbose mode
#define TRACE(msg) {     ::Engine::getMessageLogger().log( _tstring(_T(__FUNCTION__)), _tstring(msg) );     }

// TRACE-like macro that only shows up in DEBUG mode
#ifdef _DEBUG
#define DEBUG_TRACE(msg) {     ::Engine::getMessageLogger().log( _tstring(_T(__FUNCTION__)), _tstring(msg) );     }
#else 
#define DEBUG_TRACE(msg) ;
#endif

// The ERR macro will dump a _tstring to the log file in verbose mode
#define ERR(msg) {     ::Engine::getMessageLogger().log( _tstring(_T(__FUNCTION__)), _tstring(msg) );     }


#endif
