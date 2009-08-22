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

#include "stdafx.h"
#include "ToggleWidgetText.h"

namespace Engine {

ToggleWidgetText::ToggleWidgetText(const string& label, bool *value)
: myValue(value),
  myLabel(label),
  statusLbl(0)
{
	ASSERT(myValue!=0, "value was NULL");

	// Create the status label
	AddChild(  statusLbl = new LabelWidget(myLabel, vec2(12.0f,1.0f))  );
	statusLbl->dumb = true;

	// Set up the initial status
	if(getValue())
	{
		OnTurnsOn();
	}
	else
	{
		OnTurnsOff();
	}
}

void ToggleWidgetText::onMouseClick(void)
{
	ASSERT(myValue!=0, "myValue was NULL");

	(*myValue) = !getValue();

	// Run the appropriate event handler
	if(getValue())	OnTurnsOn();
	else			OnTurnsOff();

	ListElementWidget::onMouseClick();
}

void ToggleWidgetText::OnTurnsOn(void)
{
	statusLbl->setLabel(myLabel + ": true");
}

void ToggleWidgetText::OnTurnsOff(void)
{
	statusLbl->setLabel(myLabel + ": false");
}

void ToggleWidgetText::onMouseEnter(void)
{
	statusLbl->m_Color = yellow;
}

void ToggleWidgetText::onMouseExit(void)
{
	statusLbl->m_Color = white;
}

void ToggleWidgetText::update(float deltaTime)
{
	ListElementWidget::update(deltaTime);
}

}; // namespace
