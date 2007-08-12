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

#include "stdafx.h"
#include "gl.h"
#include "SDLwindow.h"
#include "widget.h"

namespace Engine {

Widget::Widget(void)
: m_bVisible(true),
  dumb(false),
  widgetName("This widget has not been assigned a name."),
  dead(false),
  depressed(false),
  within(false),
  hover(false),
  mouseDown(false),
  parent(0)
{}

Widget::~Widget(void)
{
	destroy();
}

void Widget::AddChild(Widget *widget)
{
	ASSERT(widget!=0, "widget was NULL");
	widget->parent = this;
	m_Children.push_back(widget);
}

void Widget::destroy(void)
{
	while(!m_Children.empty())
	{
		delete(*m_Children.begin());
		m_Children.erase(m_Children.begin());
	}
}

void Widget::RenderTree(const vec2 &base) const
{
	absPos = base + relPos; // Update origins

	if(isVisible())
	{
		glPushMatrix();
		glTranslatef(relPos.x, relPos.y, 0.0f);

		// draw ourselves
		draw();

		// draw our children
		for_each(m_Children.begin(), m_Children.end(), bind(&Widget::RenderTree, _1, absPos));

		glPopMatrix();
	}
}

void Widget::ProcessTree(float deltaTime)
{
	if(isVisible())
	{
		pruneDeadChildren(); // can we run this periodically?

		if(m_Children.size()>0)
		{
			// Process Children in reverse order of rendering
			list<Widget*> reversedList;
			transform(m_Children.begin(), m_Children.end(), back_inserter(reversedList), _1);
			for_each(reversedList.begin(), reversedList.end(), bind(&Widget::ProcessTree, _1, deltaTime));
		}

		update(deltaTime);
	}
}

void Widget::handleEventsTree(void)
{
	if(isVisible())
	{
		if(m_Children.size()>0)
		{
			// Process Children in reverse order of rendering
			list<Widget*> reversedList;
			transform(m_Children.begin(), m_Children.end(), back_inserter(reversedList), _1);
			for_each(reversedList.begin(), reversedList.end(), bind(&Widget::handleEventsTree, _1));
		}

		handleEvents();
	}
}

void Widget::pruneDeadChildren(void)
{
	// free and remove all dead widgets now
	list<Widget*>::iterator iter = m_Children.begin();
	while(iter != m_Children.end())
	{
		if((*iter)->dead)
		{
			delete(*iter);
			iter = m_Children.erase(iter);
		}
		else
			iter++;
	}
}

void Widget::update(float)
{
	handleEvents();
}

float Widget::getMouseX(void) const
{
	return ((float)g_Input.MouseX / g_Window.GetWidth() * 1024.f) - getBaseX();
}

float Widget::getMouseY(void) const
{
	return ((float)(g_Window.GetHeight() - g_Input.MouseY) / g_Window.GetHeight() * 768.f) - getBaseY();
}

bool Widget::isWithin(void) const
{
	float x = getMouseX();
	float y = getMouseY();

	return(x > 0 && x < getWidth()) && (y > 0 && y < getHeight());
}

void Widget::handleEvents(void)
{
	bool lastWithin = hover;
	bool lastMouseDown = mouseDown;

	mouseDown = g_Input.MouseLeft;
	within = isWithin();

	// Mouse Enter
	if(within && !lastWithin)
	{
		hover = true;
		onMouseEnter();
	}

	// Mouse Exit
	if(!within && lastWithin)
	{
		hover = false;
		onMouseExit();
	}

	g_GUI.mouseOverSomeWidget |= hover;

	// Mouse Up
	if(!mouseDown && lastMouseDown)
	{
		if(hover)
		{
			onMouseUp();

			// Click
			if(depressed && !g_GUI.clickAlreadyProcessedThisTick)
			{
				g_GUI.clickAlreadyProcessedThisTick = true; // top-most widgets are updated first anyway
				g_SoundSystem.play("data/sound/click.wav");
				onMouseClick();
			}
		}

		depressed = false;
	}

	// Mouse Down
	if(hover && mouseDown)
	{
		depressed = true;

		if(!lastMouseDown)
		{
			onMouseDown();
		}
	}
}

} // namespace Engine
