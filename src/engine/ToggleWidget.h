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

#ifndef _TOGGLE_WIDGET_H_
#define _TOGGLE_WIDGET_H_

#include "ListElementWidget.h"

namespace Engine {

/** Toggles a variable to true or false when it is clicked. The image used for the widget depends on the value of the variable. */
class ToggleWidget : public ListElementWidget
{
private:
	/** The watched value*/
	bool *myValue;

	/** The image used for the element when the value is true */
	Material matTRUE;

	/** The image used for the element on mouse over when the value is true */
	Material matMouseOverTRUE;

	/** The image used for the element when the value is false */
	Material matFALSE;

	/** The image used for the element on mouse over when the value is false */
	Material matMouseOverFALSE;

protected:
	/**
	gets the most appropriate material for the widget at the moment
	@return The material that is currrently most appropriate
	*/
	virtual const Material& getMaterial(void) const;

public:
	/**
	Constructs the widget
	@param value The value that the toggle widget will toggle on and off
	@param imageFileTRUE The image used for the element when the value is true
	@param imageFileFALSE The image used for the element when the value is false
	*/
	ToggleWidget(bool *value, const string& imageFileTRUE, const string& imageFileFALSE);

	/**
	Constructs the widget
	@param value The value that the toggle widget will toggle on and off
	@param imageFileTRUE The image used for the element when the value is true
	@param imageFileFALSE The image used for the element when the value is false
	@param imageFileMouseOvereTRUE The image used for the element on mouse over when the value is true
	@param imageFileMouseOvereFALSE The image used for the element on mouse over when the value is false
	*/
	ToggleWidget(bool *value, const string& imageFileTRUE, const string& imageFileMouseOverTRUE, const string& imageFileFALSE, const string& imageFileMouseOverFALSE);

	/** Called on the event that the widget is clicked */
	virtual void onMouseClick(void);

	/**
	Gets the boolean state of the watched value
	@return true if the value is true, false if the value is false
	*/
	inline bool getValue(void) const
	{
		ASSERT(myValue!=0, "ToggleWidget::isOn  ->  myValue was NULL");
		return(*myValue);
	}

	/**
	Sets the value in the toggle
	@param The new value for the toggle
	*/
	inline void setValue(bool value)
	{
		ASSERT(myValue!=0, "ToggleWidget::isOn  ->  myValue was NULL");
		(*myValue) = value;
	}
};

}; // namespace

#endif
