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

#ifndef _SELECTION_LIST_H_
#define _SELECTION_LIST_H_

#include "ListPaneWidget.h"


namespace Engine {



/**
This is a ListPaneWidget that can notify the client that
a specific item has been selected.
*/
class SelectionList : public ListPaneWidget
{
protected:
	/** All selectable elements of the list */
	std::list<ListElementWidget*> selectableElements;

	/** The selected element */
	ListElementWidget *selected;

public:
	/**
	Constructs the list widget
	@param x The X-Coordinate of the list widget
	@param y The Y-Coordinate of the list widget
	*/
	SelectionList(float x, float y);

	/**
	Gets the selected item.  This is the last item that was clicked or the first item in the list if the list has not been clicked yet.
	@return element or null
	*/
	ListElementWidget* getSelected(void)
	{
		return selected;
	}

	/**
	Allows the user to add selectable elements to the widget
	*/
	void addSelectableElement(ListElementWidget *widget);

	/**
	One of our children was clicked
	@param child Points to the child widget
	*/
	virtual void childWasClicked(ListElementWidget *child);
};

} // namespace Engine

#endif
