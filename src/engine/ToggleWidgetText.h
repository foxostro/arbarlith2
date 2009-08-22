/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _TOGGLE_TEXT_WIDGET_H_
#define _TOGGLE_TEXT_WIDGET_H_

#include "tstring.h"
#include "labelwidget.h"
#include "ListElementWidget.h"

namespace Engine {

/** Toggles a variable to true or false when it is clicked. The text used for the label depends on the value of the variable. */
class ToggleWidgetText : public ListElementWidget
{
private:
	/** The watched value*/
	bool *myValue;

	/** Text description of the value */
	string myLabel;

	/** The label that displays the status of the value */
	LabelWidget *statusLbl;

protected:
	/**
	Updates the widget
	@param deltaTime Milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

public:
	/**
	Constructs the widget
	@param label Text description of the value.  Goes into the creation of the label text for the widget.
	@param value The value that the toggle widget will toggle on and off
	*/
	ToggleWidgetText(const string& label, bool *value);

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void);

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void);

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void);

	/** Called in the event that the value is true, then becomes false */
	virtual void OnTurnsOn(void);

	/** Called in the event that the value is false, then becomes true */
	virtual void OnTurnsOff(void);

	/**
	Gets the boolean state of the watched value
	@return true if the value is true, false if the value is false
	*/
	inline bool getValue(void) const
	{
		ASSERT(myValue!=0, "myValue was NULL");
		return(*myValue);
	}

	/**
	Sets the value in the toggle
	@param The new value for the toggle
	*/
	inline void setValue(bool value)
	{
		ASSERT(myValue!=0, "myValue was NULL");
		(*myValue) = value;
	}
};

} // namespace Engine

#endif
