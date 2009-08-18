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

#ifndef _LISTPANEWIDGET_H_
#define _LISTPANEWIDGET_H_

#include "widgetmanager.h"
#include "ListElementWidget.h"


namespace Engine {



/**
A pane for a list widget on the GUI.
Contains the individual items on the pane.
*/
class ListPaneWidget : public Widget
{
protected:
	bool askForUpdate;

	vector<ListElementWidget*> listElements;

	/**
	Get the Y-Coordinate of the element in the list that will be added next
	@return Y-Coordinate
	*/
	float getTop(void) const;

	/** Reconstructs the list */
	virtual void update(void);

public:
	/**
	Constructs the list widget
	@param x The X-Coordinate of the list widget
	@param y The Y-Coordinate of the list widget
	*/
	ListPaneWidget(float x, float y);

	/** Destructor */
	virtual ~ListPaneWidget(void);

	/**	Destroys the list */
	virtual void destroy(void);

	/** Request that the list be updated on the next tick */
	void requestUpdate(void);

	/**
	Adds an element to the pane.  The element must have been allocated
	dynamically as the ListPaneWidget will assume responsibility for memory
	managing it from the time it is added.
	@param element The element to add to the pane.
	*/
	virtual void addElement(ListElementWidget *element);
};

} // namespace Engine

#endif
