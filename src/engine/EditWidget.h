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

#ifndef _EDIT_WIDGET_H_
#define _EDIT_WIDGET_H_

#include "labelwidget.h"
#include "Controller.h"

namespace Engine {

/** Edittable text widget.  The EditWidget allows the user to edit a block of text. */
class EditWidget : public LabelWidget
{
private:
	/** Prevents the keys from repeating when held down */
	bool debounce;

	/** The shift key was depressed */
	bool shift;

	/** The last key pressed.  If the next key is different, then debounce is ignored. */
	size_t lastKey;

	/** Indicates that we had focus last tick */
	bool hadFocusLastTick;

	/** The edit widget that currently has keyboard focus */
	static EditWidget *currentlyFocused;

public:
	/**
	Constructs the widget
	@param label The label
	@param pos Position of the widget
	*/
	EditWidget(const string &label,
	           const vec2 &pos);

	/**
	Updates the EditWidget by grabbing user input if it has focus
	@param deltaTime The milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void);

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void);

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void);

	/**	Called in the event that focus is acquired */
	virtual void onAcquireFocus(void);

	/**	Called in the event that focus is lost */
	virtual void onLostFocus(void);

	/**
	Determines if this widget is the currently selected edit widget
	@return true if this widget has keyboard focus
	*/
	bool hasFocus(void) const;
};

} // namespace Engine

#endif
