/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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

#ifndef _BLUR_H_
#define _BLUR_H_

namespace Engine {

class World;

/**
Blurs the screen
*/
class Blur
{
public:
	/**
	Constructs the Blur
	@param width The width of the screen capture
	@param height The height of the screen capture
	*/
	Blur(int width, int height);

	/** Destrutor. Cleans up texture resources */
	~Blur(void);

	/** Resets all data members */
	void clear(void);

	/**
	Creates the Blur
	@param width The width of the screen capture
	@param height The height of the screen capture
	*/
	void create(int width, int height);

	/** Destroys the Blur */
	void destroy(void);

	/** Capture the scene to a texture */
	void capture(void);

	/** Draw the captured scene as an overlay */
	void draw(void);

	/** Do a blur capture next tick */
	inline void doCapture(void)
	{
		update = true;
	}

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Sets the current zone for the Blur to re-render it as necessary
	@param zone the current zone
	*/
	inline void setZone(World * zone)
	{
		this->zone = zone;
	}

	/** The blur is enabled as long as this is positive, non-zero */
	float alphaBlur;

private:
	/** A handle to thetexture where the scene is captured */
	unsigned int scene;

	/** A screen capture will be performed on the next frame */
	bool update;

	/** The texture width */
	int BLUR_TEXTURE_WIDTH;

	/** The texture height */
	int BLUR_TEXTURE_HEIGHT;

	/** The current zone */
	World *zone;
};

} //namespace Engine

#endif
