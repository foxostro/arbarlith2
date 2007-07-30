/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#ifndef _BACKDROP_TEXTURE_H_
#define _BACKDROP_TEXTURE_H_

#include "material.h"


namespace Engine {


/** Backdrop texture for a screen.  The image chosen depends upon the screen resolution. */
class BackdropTexture
{
public:
	/** Destructor */
	~BackdropTexture(void);

	/** Constructor */
	BackdropTexture(void);

	/** Resets all members */
	void clear(void);

	/** Destroys all resources */
	void destroy(void);

	/**
	Allocates all resources
	@param imageFileName_640x480 Image file name for the backdrop at 640x480 resolution
	@param imageFileName_800x600 Image file name for the backdrop at 800x600 resolution
	@param imageFileName_1024x768 Image file name for the backdrop at 1024x768 resolution
	@param imageFileName_1280x768 Image file name for the backdrop at 1280x768 resolution
	*/
	void create(const _tstring &imageFileName_640x480,
				const _tstring &imageFileName_800x600,
				const _tstring &imageFileName_1024x768,
				const _tstring &imageFileName_1280x768);

	/**
	Gets the texture for this resolution
	@return Texture for the texture at this resolution
	*/
	Material& getTexture(void);

private:
	/** Material for the backdrop at 640x480 resolution */
	Material* texture_640x480;

	/** Material for the backdrop at 800x600 resolution */
	Material* texture_800x600;

	/** Material for the backdrop at 1024x768 resolution */
	Material* texture_1024x768;

	/** Material for the backdrop at 1280x768 resolution */
	Material* texture_1280x768;
};

} // namespace Engine

#endif
