/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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

#ifndef _STRING_SELECTION_LIST_H_
#define _STRING_SELECTION_LIST_H_

#include "SelectionList.h"
#include "ListElementLabel.h"

namespace Engine {

/** Selection list of values */
template<class T>
class StringSelectionList : public SelectionList
{
private:
	/** This is a list element widget that pairs a string label with a value */
	class ELEMENT : public ListElementLabel
	{
	private:
		/** value to associate with the element */
		const T myValue;

	public:
		/**
		Constructs the widget
		@param label A text label to superimpose over the widget
		@param value Value to associate with the element
		*/
		ELEMENT(const string &label, const T &value)
		: ListElementLabel(label, "data/sprites/list/listwidget_depressed.png",
		                          "data/sprites/list/listwidget_depressed_over.png",
		                          "data/sprites/list/listwidget.png",
		                          "data/sprites/list/listwidget_hover.png"),
		  myValue(value)
		{}

		/**
		Gets the value associated with the label
		@return label
		*/
		const T& getValue(void)
		{
			return myValue;
		}
	};

public:
	/**
	Constructs the list widget
	@param x The X-Coordinate of the list widget
	@param y The Y-Coordinate of the list widget
	*/
	StringSelectionList(float x, float y)
	:SelectionList(x, y)
	{}

	/**
	Gets the value from the currently selected element
    @return value
	*/
	const T& getValue(void)
	{
		return dynamic_cast<ELEMENT&>(*getSelected()).getValue();
	}

	/**
	Adds an element to the list
	@param label Element's label
	@param value Element's value
	*/
	void addElement(const string &label, const T &value)
	{
		addSelectableElement(new ELEMENT(label, value));
	}
};

}; //namespace

#endif
