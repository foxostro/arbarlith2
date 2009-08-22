/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2005,2007,2009 Game Creation Society
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
#include "random.h"
#include "Application.h"
#include "SoundSystem.h"
#include "MusicEngine.h"

namespace Engine {

PropertyBag MusicEngine::saveMusicTag(const vector<string> &music) const
{
	PropertyBag bag;

	for(size_t i=0; i<music.size(); ++i)
	{
		bag.add("clip", music[i]);
	}

	return bag;
}

void MusicEngine::loadMusicTag(const PropertyBag &bag, vector<string> &music) const
{
	music.clear();

	const size_t n = bag.getNumInstances("clip");
	for(size_t i=0; i<n; ++i)
	{
		string file;

		if(bag.get("clip", file, i))
		{
			music.push_back(file);
		}
	}
}

PropertyBag MusicEngine::save(void) const
{
	PropertyBag bag;
	bag.add("clips", saveMusicTag(clips));
	return bag;
}

void MusicEngine::load(const PropertyBag &bag)
{
	PropertyBag clipsBag;

	if(bag.get("clips", clipsBag))
	{
		loadMusicTag(clipsBag, clips);
	}
}

void MusicEngine::update(void)
{
	if(!clips.empty())
	{
		// Choose a clip from the selected collection
		size_t min = 0;
		size_t max = clips.size()-1;
		size_t idx = 0;

		if(max>min)
			IRAND_RANGE(min, max);

		string clip = clips[idx];

		// Play that clip
		TRACE(string("Playing music clip: ") + clip);
		g_SoundSystem.stopAll();
		g_SoundSystem.playMusic(clip);
	}
}

}; // namespace
