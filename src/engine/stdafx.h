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

#ifndef _ENGINE_STDAFX_H_
#define _ENGINE_STDAFX_H_

#if defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning( disable: 4512 ) // cannot generate assignment operator
#endif

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

/* STL */
#include <list>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <sstream>
#include <new>
#include <algorithm>
#include <exception>

using namespace std;

/* Standard C Library */
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <float.h>

/* Boost Library */
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace boost::lambda;

#include "tstring.h"       // functions to operate on strings
#include "logstring.h"     // Message logger
#include "myassert.h"      // Custom assertion routine
#include "macros.h"        // useful macros
#include "singleton.h"     // singleton pattern
#include "random.h"        // Various Random number routines
#include "file.h"          // Virtual file system
#include "Controller.h"    // Key bindings handler
#include "Application.h"   // The application class runs the game
#include "opengl.h"        // Interface to the rendering library
#include "Effect.h"

#endif
