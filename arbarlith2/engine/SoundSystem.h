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

#ifndef _SOUNDSYSTEM_H_
#define _SOUNDSYSTEM_H_

#include "task.h"

namespace Engine {

typedef size_t SOUND_HANDLE;

/** Controls the loading and playing of sounds */
class SoundSystem : public Task
{
public:
	/** Destructor */
	virtual ~SoundSystem(void);

	/** Constructor */
	SoundSystem(void);

	/** Initializes the SoundSystem */
	void create(void);

	/** Clears all data members */
	void clear(void);

	/** Deletes all loaded sound files from memory. */
	void destroy(void);

	/**
	Updates the FMOD system.  Call once per tick
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime);

	/**
	Plays a sound file
	@param fileName The name of the sound file
	@param looping true if the sound should loop until we switch Zones
	@param volume Volume level multiplier
	@return Returns a handle to the sound
	*/
	SOUND_HANDLE play(const string &fileName, bool looping=false, float volume=1.0f);

	/**
	Plays a sound file with a 3D position
	@param fileName The name of the sound file
	@param pos The position of the sound in space
	@param looping true if the sound should loop until we switch Zones
	@return Returns a handle to the sound
	*/
	SOUND_HANDLE play3D(const string &fileName, const vec3 &pos, bool looping=false, float volume=1.0f);

	/**
	Stops a specific sound
	@param handle The specific sound to stop
	*/
	void stop(SOUND_HANDLE handle);

	/** Stops all sounds */
	void stopAll(void);

	/**
	Plays music
	@param fileName The name of a music file
	@return Returns a handle to the sound
	*/
	SOUND_HANDLE playMusic(const string &fileName);
};

} // namespace Engine

#endif
