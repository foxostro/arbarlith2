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
#include <SDL/SDL_mixer.h>
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
{
	loadedSounds.clear();
}

void SoundSystem::destroy()
{
	stopAll();

	while(!loadedSounds.empty())
	{
		Mix_FreeChunk((Mix_Chunk*)loadedSounds.begin()->second);
		loadedSounds.erase(loadedSounds.begin());
	}

	Mix_CloseAudio();

	clear();
}

void SoundSystem::create()
{
	destroy();

	int rate = 22050;
	Uint16 format = AUDIO_S16SYS;
	int channels = 2;
	int buffers = 4096;

	if(Mix_OpenAudio(rate, format, channels, buffers) != 0)
	{
		ERR(string("Unable to initialize audio: ") + Mix_GetError());
		g_Application.soundEnabled = false;
	}

	TRACE("Sound System initialized");
}

void SoundSystem::stopAll(void)
{
	Mix_FadeOutChannel(-1, 500); // 500ms to fade to silence and halt audio
	TRACE("Stopped all sounds");
}

void SoundSystem::play(const string &fileName, float volume)
{
	Mix_Chunk *sound = 0;

	if(!g_Application.soundEnabled)
	{
		return;
	}

	// Find the chunk in the cache?
	map<string, void*>::const_iterator sIter = loadedSounds.find(fileName);

	if(sIter == loadedSounds.end())
	{
		sound = Mix_LoadWAV(fileName.c_str());

		if(sound == 0)
		{
			ERR(string("Failed to load sound file: ") + Mix_GetError());
			return;
		}

		// cache the chunk	now that its loaded
		loadedSounds.insert(make_pair(fileName, sound));

		TRACE("Loaded and cached sound file:" + fileName);
	}
	else
	{
		sound = (Mix_Chunk*)(sIter->second);
	}

	// Play the chunk
	if(Mix_PlayChannel(-1, sound, 0) == -1)
	{
		ERR(string("Failed to play sound file: ") + Mix_GetError());
		return;
	}
}

void SoundSystem::play3D(const string &fileName, const vec3 &, float)
{
	// stub
	play(fileName);
}

void SoundSystem::playMusic(const string &)
{
	ERR("playMusic() failed");
}

void SoundSystem::update(float)
{}

} // namespace Engine
