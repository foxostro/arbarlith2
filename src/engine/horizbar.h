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

#ifndef HORIZBAR_H
#define HORIZBAR_H

#include "widgetmanager.h"
#include "windowwidget.h"

namespace Engine {

/** Progress bar */
class HorizBar : public Widget
{
public:
	/**
	Constructor
	@param x The position of the widget
	@param y The position of the widget
	@param capacity The value at which the bar will be completed
	@param startingPosition Starting value of the bar
	@param strForeground This image is uncovered as the bar progresses
	@param strBackground This image is the background of the bar as it moves
	@param leftAlign Indicates that the bar will progress from left to right
	*/
	HorizBar(float x,
		     float y,
			 float capacity,
			 float startingPosition,
			 const string &strForeground,
			 const string &strBackground,
			 bool leftAlign);

	/**
	Gets the position of the bar
	@return position
	*/
	inline float getPos(void) const
	{
		return m_Position;
	}

	/**
	Gets the capacity of the bar
	@return capacity
	*/
	inline float getCapacity(void) const
	{
		return m_Capacity;
	}

protected:
	/** Updates the widget */
	virtual void update(float);

private:
	/** This image is uncovered as the bar progresses */
	WindowWidget *m_ForeImage;

	/** This image is the background of the bar as it moves */
	WindowWidget *m_BackImage;

	/** The value that dtermines the size of the bar */
	float m_Position;

	/** The value at which the bar will be completed */
	float m_Capacity;

	/** the initial width of the bar */
	float initialWidth;

	/** indicates that the bar will progress from left to right */
	bool isLeftAligned;
};

}; // namespace

#endif // !defined(HORIZBAR_H)
