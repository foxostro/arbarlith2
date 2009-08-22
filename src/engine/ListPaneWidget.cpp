/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2005-2007,2009 Game Creation Society
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
#include "world.h"
#include "ListElementWidget.h"
#include "ListPaneWidget.h"

namespace Engine {

float ListPaneWidget::getTop(void) const
{
	// Update the position of the top of the border
	if(listElements.empty() == false)
	{
		ListElementWidget *element = listElements[listElements.size()-1];

		ASSERT(element!=0, "the last element of the list was NULL");

		return element->getRelativeY() + element->getHeight();
	}
	else
	{
		return 0;
	}
}

void ListPaneWidget::update(void)
{
	if(askForUpdate)
	{
		update();
		askForUpdate=false;
	}
}

ListPaneWidget::ListPaneWidget(float x, float y)
:askForUpdate(false)
{
	m_bVisible = false;
	setRelativePos(vec2(x,y));
}

ListPaneWidget::~ListPaneWidget(void)
{
	destroy();
}

void ListPaneWidget::addElement(ListElementWidget *element)
{
	if(element)
	{
		element->setRelativePos(0.0f,getTop());

		AddChild(element);
		listElements.push_back(element);

		requestUpdate();
	}
}

void ListPaneWidget::destroy(void)
{
	Widget::destroy();
	listElements.clear();
}

void ListPaneWidget::requestUpdate(void)
{
	askForUpdate = true;
}

}; // namespace
