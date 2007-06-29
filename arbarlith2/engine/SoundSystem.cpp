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
#include "soundsystem.h"
#include <fmod.hpp>


namespace Engine { 



FMOD::System *g_FMOD=0;

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
	g_FMOD=0;
	music=0;
	nextHandle=1;
	sounds.clear();
}

void SoundSystem::destroy()
{
	stopAll();

	if(g_FMOD)
		g_FMOD->release();

	clear();
}

void SoundSystem::create()
{
	destroy();

	FMOD_RESULT result = FMOD::System_Create(&g_FMOD);		// Create the main system object.
	if (result != FMOD_OK)
	{
		ERR(_T("failed to create FMOD system"));
		return;
	}

	result = g_FMOD->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	if (result != FMOD_OK)
	{
		ERR(_T("failed to initialize FMOD system"));
		return;
	}
}

void SoundSystem::stopAll(void)
{
	for(SOUNDS_MAP::iterator iter=sounds.begin(); iter!=sounds.end(); ++iter)
	{
		(iter->second)->stop();
	}

	sounds.clear();
	nextHandle=1;
}

void SoundSystem::stop(SOUND_HANDLE handle)
{
	if(sounds.find(handle) != sounds.end())
	{
		sounds[handle]->stop();
		sounds.erase(sounds.find(handle));
	}
}

SOUND_HANDLE SoundSystem::play(const _tstring &fileName, bool looping, float volume)
{
	if(!g_Application.soundEnabled)
	{
		return 0;
	}

	FMOD::Sound *sound;
	FMOD_RESULT result = g_FMOD->createSound(toAnsiString(fileName).c_str(), FMOD_HARDWARE | FMOD_LOOP_NORMAL, 0, &sound);

	ASSERT(result == FMOD_OK, _T("Failed to play sound: ") + fileName);

	if(result == FMOD_OK)
	{
		FMOD::Channel *channel;
		
		sound->setLoopCount(looping?-1:0);
		g_FMOD->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
			
		channel->setVolume(volume);
		channel->setPaused(false);

		// Store the Channel
		SOUND_HANDLE handle = nextHandle;
		sounds.insert(make_pair(handle, channel));
		nextHandle++;
		
		// Return the handle
		return handle;
	}


	// Failure
	return 0;
}

SOUND_HANDLE SoundSystem::play3D(const _tstring &fileName, const vec3 &pos, bool looping, float volume)
{
	if(!g_Application.soundEnabled)
	{
		return 0;
	}

	FMOD::Sound *sound;
	FMOD_RESULT result = g_FMOD->createSound(toAnsiString(fileName).c_str(), (FMOD_MODE)(FMOD_HARDWARE | FMOD_3D), 0, &sound);

	if(result == FMOD_OK)
	{
		FMOD::Channel *channel;
		
		sound->setLoopCount(looping?-1:0);

		g_FMOD->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
		
		channel->setVolume(volume);

		FMOD_VECTOR sound_pos;
		sound_pos.x = pos.x;
		sound_pos.y = pos.y;
		sound_pos.z = pos.z;
		channel->set3DAttributes(&sound_pos, 0);

		// Actually play the sound
		channel->setPaused(false);

		// Store the Channel
		SOUND_HANDLE handle = nextHandle;
		sounds.insert(make_pair(handle, channel));
		nextHandle++;
		
		// Return the handle
		return handle;
	}

	// Failure
	return 0;
}

SOUND_HANDLE SoundSystem::playMusic(const _tstring &fileName)
{
	stop(music);
	music = 0;

	if(g_Application.soundEnabled)
	{
		music = play(fileName, true);
	}

	return music;
}

void SoundSystem::update(float)
{
	Camera &camera = g_Camera;

	const vec3 &position = camera.getPosition();
	const mat4 &orientation = camera.getOrientation();

	FMOD_VECTOR listener_pos;
	FMOD_VECTOR listener_forward;
	FMOD_VECTOR listener_up;

	listener_pos.x = position.x;
	listener_pos.y = position.y;
	listener_pos.z = position.z;

	listener_forward.x = orientation.getAxisZ().x;
	listener_forward.y = orientation.getAxisZ().y;
	listener_forward.z = orientation.getAxisZ().z;

	listener_up.x = orientation.getAxisY().x;
	listener_up.y = orientation.getAxisY().y;
	listener_up.z = orientation.getAxisY().z;

	g_FMOD->set3DListenerAttributes(0, &listener_pos, 0, &listener_forward, &listener_up);     
	g_FMOD->update();
}



}; // namespace
