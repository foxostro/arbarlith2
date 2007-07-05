/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2005-2007 Game Creation Society
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
#include "application.h"
#include "SoundSystem.h"
#include "musicengine.h"


namespace Engine { 



CPropBag MusicEngine::saveMusicTag(vector<_tstring> &music)
{
	CPropBag bag;

	for(size_t i=0; i<music.size(); ++i)
	{
		_tstring clip = music[i];
		bag.Add(_T("clip"), clip);
	}

	return bag;
}

void MusicEngine::save(CPropBag &bag)
{
	bag.Add(_T("clips"), saveMusicTag(clips));
}

void MusicEngine::loadMusicTag(CPropBag &bag, vector<_tstring> &music)
{
	music.clear();

	int n = bag.GetNumInstances(_T("clip"));

	for(int i=0; i<n; ++i)
	{
		_tstring file;

		if(bag.Get(_T("clip"), file, i))
		{
			music.push_back(file);
		}
	}
}

void MusicEngine::load(CPropBag &bag)
{
	CPropBag clipsBag;

	if(bag.Get(_T("clips"), clipsBag))
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
		
		_tstring clip = clips[idx];

		// Play that clip
		TRACE(_tstring(_T("Playing music clip: ")) + clip);
		g_SoundSystem.stopAll();
		g_SoundSystem.playMusic(clip);
	}
}

}; // namespace
