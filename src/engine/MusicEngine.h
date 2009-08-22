/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _MUSIC_ENGINE_H_
#define _MUSIC_ENGINE_H_

#include "PropertyBag.h"

namespace Engine {

/** Maintains a collection of music files for a realm. */
class MusicEngine
{
private:
	/** The file names for the different music clips */
	vector<string> clips;

	/**
	Saves a tag containing a collection of music
	@param music Music clip file names
	@return Returns the XML source
	*/
	PropertyBag saveMusicTag(const vector<string> &music) const;

	/**
	Loads a tag containing a collection of music
	@param bag XML source
	@param music Returns the music segemts contained within
	*/
	void loadMusicTag(const PropertyBag &bag, vector<string> &music) const;

public:
	/**
	Loads the names of music segments from XML
	@param bag XML source
	*/
	void load(const PropertyBag &bag);

	/**
	Save the names of music segments
	@return saved state
	*/
	PropertyBag save(void) const;

	/** Called when a new music segment should be played. */
	void update(void);
};

} // namespace Engine

#endif

