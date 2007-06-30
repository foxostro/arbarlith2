/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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

#ifndef WINDOW_WIDGET_H
#define WINDOW_WIDGET_H

#include "Widget.h"
#include "Material.h"

namespace Engine { 

class WindowWidget : public Widget  
{
public:
	/**
	Constructor
	@param x relative x
	@param y relative y
	@param imageFile The image file to use for an unaffected widget
	*/
	WindowWidget(float x, float y, const _tstring &imageFile);
	
	/**
	Constructor
	@param x relative x
	@param y relative y
	@param imageFile The image file to use for an unaffected widget
	@param depressedFile The image file to use for a depressed widget
	*/
	WindowWidget(float x, float y, const _tstring &imageFile, const _tstring &depressedFile);
	
	/**
	Constructor
	@param x relative x
	@param y relative y
	@param imageFile The image file to use for an unaffected widget
	@param depressedFile The image file to use for a depressed widget
	@param hoverFile The image file to use for a widget with mouse hover
	*/
	WindowWidget(float x, float y, const _tstring &imageFile, const _tstring &depressedFile, const _tstring &hoverFile);

	/** Destructor */
	virtual ~WindowWidget(void);

	/** Indicate that the widget materials should be stretched to fit the widget dimensions */
	bool stretchToFit;

protected:
	/** draws the widget */
	virtual void draw(void) const;

	/**
	gets the most appropriate material for the widget at the moment
	@return The material that is currrently most appropriate
	*/
	virtual const Material& getMaterial(void) const;

private:
	/** material when the mouse hovers over the widget */
	Material matHover;

	/** material when the mouse is depressed over the widget */
	Material matDepressed;

	/** material when the mouse is not affecting the widget */
	Material matRegular;

	/** Unstrectched dimensions of the widget */
	vec2 unstretchedDim;
};

}; //namespace

#endif

