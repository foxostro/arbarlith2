/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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
#include "SoundSystem.h"

namespace Engine {

SoundSystem::SoundSystem(void)
{
	clear();
	create();
}

SoundSystem::~SoundSystem(void)
{
	destroy();
}

void SoundSystem::clear()
{}

void SoundSystem::destroy()
{
	stopAll();

	clear();
}

void SoundSystem::create()
{
	destroy();
}

void SoundSystem::stopAll(void)
{}

void SoundSystem::stop(SOUND_HANDLE)
{}

SOUND_HANDLE SoundSystem::play(const string &, bool, float)
{
	if(!g_Application.soundEnabled)
	{
		return 0;
	}

	// Failure
	return 0;
}

SOUND_HANDLE SoundSystem::play3D(const string &, const vec3 &, bool, float)
{
	if(!g_Application.soundEnabled)
	{
		return 0;
	}

	// Failure
	return 0;
}

SOUND_HANDLE SoundSystem::playMusic(const string &)
{
	return 0;
}

void SoundSystem::update(float)
{}

} // namespace Engine
