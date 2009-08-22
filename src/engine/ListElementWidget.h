/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009 Game Creation Society
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

#ifndef _LISTELEMENTWIDGET_H_
#define _LISTELEMENTWIDGET_H_

#include "windowwidget.h"

namespace Engine {

/** Element of a list widget on the GUI */
class ListElementWidget : public WindowWidget
{
private:
	/** Image when the element is unaffected */
	static string normalImg;

	/** Image when the element is depressed */
	static string depressedImg;

	/** Image when the mouse is over the element */
	static string hoverImg;

public:
	/** Constructor */
	ListElementWidget(void);

	/**
	Constructor
	@param x relative x
	@param y relative y
	@param imageFile The image file to use for an unaffected widget
	@param depressedFile The image file to use for a depressed widget
	@param hoverFile The image file to use for a widget with mouse hover
	*/
	ListElementWidget(float x, float y, const string &imageFile, const string &depressedFile, const string &hoverFile);

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void);
};

} // namespace Engine

#endif
