/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2007 Game Creation Society
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

#ifndef _CYCLE_TEXTURE_SELECTOR_STATE_H_
#define _CYCLE_TEXTURE_SELECTOR_STATE_H_

#include "tstring.h"
#include "labelwidget.h"
#include "ListElementWidget.h"

namespace Engine {

enum TEXTURE_SELECTOR_STATE
{
	TEXTURE_SELECTOR_HIDE,
	TEXTURE_SELECTOR_FLOOR,
	TEXTURE_SELECTOR_WALL
};

/** Cycles through the possible texture selector states */
class CycleTextureSelectorState : public ListElementWidget
{
private:
	/** Act upon this state value */
	TEXTURE_SELECTOR_STATE *myValue;

	/** Text description of the value */
	_tstring myLabel;

	/** The label that displays the status of the value */
	LabelWidget *statusLbl;

	/**
	@param state The state of the texture selector
	@return Label to represent the state of the selector
	*/
	static _tstring getLabel(TEXTURE_SELECTOR_STATE state);

public:
	/**
	Constructs the widget
	@param value The value that the toggle widget will be cycled through possible states
	*/
	CycleTextureSelectorState(TEXTURE_SELECTOR_STATE *value);

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void);

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void);

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void);

	/**
	Gets the state of the watched value
	@return texture selector state
	*/
	inline TEXTURE_SELECTOR_STATE getState(void) const
	{
		ASSERT(myValue!=0, _T("myValue was NULL"));
		return(*myValue);
	}

	/**
	Sets the value
	@param value The new value
	*/
	inline void setState(TEXTURE_SELECTOR_STATE value)
	{
		ASSERT(myValue!=0, _T("myValue was NULL"));
		(*myValue) = value;
	}
};

} // namespace Engine

#endif
