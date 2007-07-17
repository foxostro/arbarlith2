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

#ifndef _BLUR_EFFECT_H_
#define _BLUR_EFFECT_H_

#include "Blur.h"
#include "COLOR.h"

namespace Engine {

/** Causes the screen to become blurred and then regain focus over time */
class BlurEffect
{
public:
	/**	Constructs the Blur effect */
	BlurEffect(void);

	/**
	Starts the screen blur effect
	@param timePeriod The time that the screen is blurred for
	@param color The color to shift the blur towards
	*/
	void start(float timePeriod, COLOR color);

	/** Draw the blur effect */
	void draw(void);

	/**
	Updates the timer on the blur effect
	@param deltaTime The time since the last tick (milliseconds)
	*/
	void update(float deltaTime);

	/** Release assets */
	void release(void);

	/** Reaquire assets */
	void reaquire(void);

	/**
	Sets the current zone for the Blur to re-render it as necessary
	@param zone the current zone
	*/
	void setZone(World * zone)
	{
		blur.setZone(zone);
	}

private:
	/** Screen capture */
	Blur blur;

	/** The length of the action */
	float timeLimit;

	/** The age of the blur effect */
	float myAge;
};

} // namespace Engine

#endif
