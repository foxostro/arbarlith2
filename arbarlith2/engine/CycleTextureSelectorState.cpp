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

#include "stdafx.h"
#include "CycleTextureSelectorState.h"

namespace Engine {

CycleTextureSelectorState::CycleTextureSelectorState(TEXTURE_SELECTOR_STATE *value)
: myValue(value),
  myLabel(_T("uninitialized")),
  statusLbl(0)
{
	// Create the status label
	AddChild(  statusLbl = new LabelWidget(myLabel, vec2(12.0f,1.0f))  );
	statusLbl->dumb = true;
	statusLbl->setLabel(getLabel(*myValue));
}

_tstring CycleTextureSelectorState::getLabel(TEXTURE_SELECTOR_STATE state)
{
	switch(state)
	{
	case TEXTURE_SELECTOR_HIDE:
		return _T("Hiding texture selector");

	case TEXTURE_SELECTOR_FLOOR:
		return _T("Selecting the floor texture");

	case TEXTURE_SELECTOR_WALL:
		return _T("Selecting the wall texture");
	}

	return _T("INVALID STATE");
}

void CycleTextureSelectorState::onMouseClick(void)
{
	ASSERT(myValue!=0, _T("myValue was NULL"));

	switch(getState())
	{
	case TEXTURE_SELECTOR_HIDE:
		(*myValue) = TEXTURE_SELECTOR_FLOOR;
		break;

	case TEXTURE_SELECTOR_FLOOR:
		(*myValue) = TEXTURE_SELECTOR_WALL;
		break;

	case TEXTURE_SELECTOR_WALL:
		(*myValue) = TEXTURE_SELECTOR_HIDE;
		break;
	}

	statusLbl->setLabel(getLabel(*myValue));

	ListElementWidget::onMouseClick();
}

void CycleTextureSelectorState::onMouseEnter(void)
{
	statusLbl->m_Color = yellow;
}

void CycleTextureSelectorState::onMouseExit(void)
{
	statusLbl->m_Color = white;
}

}; // namespace
