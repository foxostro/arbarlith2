/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2004-2007,2009 Game Creation Society
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

namespace Engine {

/** Controls the loading and playing of sounds */
class SoundSystem
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
	Plays a sound file
	@param fileName The name of the sound file
	@return Returns a handle to the sound
	*/
	void play(const string &fileName);

	/** Stops all sounds */
	void stopAll(void);

	/**
	Plays music
	@param fileName The name of a music file
	@return Returns a handle to the sound
	*/
	void playMusic(const string &fileName);

    /**
    Set the volume level for sound effects
	@param volume Volume level [0.0, 1.0]
    */
    void setSoundEffectVolume(float volume);

    /**
    Set the volume level for music
	@param volume Volume level [0.0, 1.0]
    */
    void setMusicVolume(float volume);

private:
    /** Global sound effects volume [0.0, 1.0] */
    float soundVolume;

    /** Global music volume [0.0, 1.0] */
    float musicVolume;

	/** Quick reference to loaded sound effects */
	map<string, void*> loadedSounds;

	/** Pointer to the music chunk */
	void *music;
};

} // namespace Engine

#endif
