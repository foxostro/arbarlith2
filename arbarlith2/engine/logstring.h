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

namespace Engine {

/**
Prints a string directly to the log stream.
Also prints it to stdout and (if possible) the debugger output window.
@param s String to log
*/
void PrintStringToLog(const string &s);

/**
Prints a message to the log stream with a string to identify the message origin.
@param message Message string
@param function Message origin.  Usually you pass __FUNCTION__ as the value
@param file File of message origin.  Usually you pass __FILE__ as the value
@param line Line of message origin.  Usually you pass __LINE__ as the value
*/
void Log(const string &message,
         const string &function,
         const string &file,
         const int line);

} // namespace Engine

#define TRACE(msg) Engine::Log(msg, __FUNCTION__, __FILE__, __LINE__);
#define ERR(msg)   Engine::Log(msg, __FUNCTION__, __FILE__, __LINE__);

#endif
