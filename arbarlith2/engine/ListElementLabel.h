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

#ifndef _LIST_ELEMENT_LABEL_H_
#define _LIST_ELEMENT_LABEL_H_

#include "ToggleWidget.h"
#include "labelwidget.h"


namespace Engine {



/** This is a list element widget that also displays a text label */
class ListElementLabel : public ToggleWidget
{
private:
	/** A text label to put on the widget */
	LabelWidget *myLabel;

public:
	/** Controls the inactive/active visua state of the widget */
	bool isActive;

	/**
	Constructs the widget
	@param label A text label to superimpose over top the widget
	@param imageFileTRUE The image used for the element when the value is true
	@param imageFileFALSE The image used for the element when the value is false
	*/
	ListElementLabel(const string &label, const string& imageFileTRUE, const string& imageFileFALSE)
	: ToggleWidget(&isActive, imageFileTRUE, imageFileTRUE, imageFileFALSE, imageFileFALSE),
	  myLabel(0),
	  isActive(false)
	{
		myLabel = new LabelWidget(label, vec2(12,2));
		myLabel->dumb = true;
		AddChild(myLabel);
	}

	/**
	Constructs the widget
	@param label A text label to superimpose over top the widget
	@param imageFileTRUE The image used for the element when the value is true
	@param imageFileFALSE The image used for the element when the value is false
	@param imageFileMouseOverTRUE The image used for the element on mouse over when the value is true
	@param imageFileMouseOverFALSE The image used for the element on mouse over when the value is false
	*/
	ListElementLabel(const string &label, const string& imageFileTRUE, const string& imageFileMouseOverTRUE, const string& imageFileFALSE, const string& imageFileMouseOverFALSE)
	: ToggleWidget(&isActive, imageFileTRUE, imageFileMouseOverTRUE, imageFileFALSE, imageFileMouseOverFALSE),
	  myLabel(0),
	  isActive(false)
	{
		myLabel = new LabelWidget(label, vec2(12,2));
		AddChild(myLabel);
	}

	/**
	Gets the label
	@return label
	*/
	inline string getLabel(void) const
	{
		return myLabel->getLabel();
	}
};

} // namespace Engine

#endif
