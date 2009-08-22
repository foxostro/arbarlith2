/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2006-2007 Game Creation Society
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

#ifndef _CLIENT_STDAFX_H_
#define _CLIENT_STDAFX_H_

#ifdef _WIN32
#	if defined(_MSC_VER)
#		define _CRT_SECURE_NO_DEPRECATE
#		pragma warning( disable: 4512 ) // cannot generate assignment operator
#	endif
#	include <windows.h>
#	include <direct.h>
#else
#	include <unistd.h>
#endif

#include <queue>
#include <vector>
#include <map>
#include <list>
using namespace std;

#include "engine/macros.h"
#include "engine/vec4.h"
#include "engine/PropertyBag.h"
#include "engine/ListElementTweaker.h"
#include "engine/Controller.h"
#include "engine/WaitScreen.h"
#include "engine/creature.h"
#include "engine/player.h"
#include "engine/world.h"
#include "engine/Application.h"

using namespace Engine;

#endif
