/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright � 2003-2007 Game Creation Society
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

#ifndef _WIDGETMANAGER_H_
#define _WIDGETMANAGER_H_

#include "Singleton.h"
#include "Widget.h"
#include "MessageBoxManager.h"

namespace Engine {

/** Manages the whole hierarchy of widgets in the GUI */
class WidgetManager : public Singleton<WidgetManager>
{
public:
	/** Constructor */
	WidgetManager(void);

	/** draws all widgets */
	void draw(void);

	/**
	updates all widgets
	@param deltaTime the milliseconds since the last update
	*/
	void update(float deltaTime);

	/**
	Adds a widget to the scene at the root
	@param widget Widget to add to the scene
	*/
	void addWidget(Widget *widget);

	/**
	Gets the message box manager
	@return Message Box Manager
	*/
	inline MessageBoxManager& getMsgBox(void)
	{
		return *m_MsgBox;
	}

	/** indicates that the mouse is over a widget */
	bool mouseOverSomeWidget;

	/** another widget has already assumed ownership of the present mouse click event */
	bool clickAlreadyProcessedThisTick;

private:
	/** the message box manager */
	MessageBoxManager *m_MsgBox;

	/** the root widget */
	Widget m_RootWidget;
};

} // namespace Engine

#define g_GUI (::Engine::WidgetManager::GetSingleton())

#endif
