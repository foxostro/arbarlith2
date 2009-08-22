/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Changed to use SDL keys instead of VK keys by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu

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

#include "stdafx.h"
#include "SDLwindow.h"
#include "EditWidget.h"


namespace Engine {



EditWidget *EditWidget::currentlyFocused = 0;

EditWidget::EditWidget(const string &label, const vec2 &pos)
: LabelWidget(label, pos),
  debounce(false),
  shift(false),
  lastKey(0),
  hadFocusLastTick(false)
{}

void EditWidget::onAcquireFocus(void)
{
	setColor(red);
}

void EditWidget::onLostFocus(void)
{
	setColor(white);
}

void EditWidget::onMouseEnter(void)
{
	if(hasFocus())
	{
		setColor(red);
	}
	else
	{
		setColor(yellow);
	}
}

void EditWidget::onMouseExit(void)
{
	if(hasFocus())
	{
		setColor(red);
	}
	else
	{
		setColor(white);
	}
}

void EditWidget::onMouseClick(void)
{
	if(!hasFocus())
	{
		currentlyFocused = this;
		onAcquireFocus();
	}
	else
	{
		currentlyFocused = 0;
	}

	g_SoundSystem.play("data/sound/click.wav");
}

bool EditWidget::hasFocus(void) const
{
	return(currentlyFocused == this);
}

void EditWidget::update(float)
{
	LabelWidget::onMouseClick();

	if(hasFocus())
	{
		size_t key=0;

		// Get keyboard input
		if(g_Keys.getKey(key, shift))
		{
			if(debounce == false || lastKey!=key)
			{
				debounce = true;

				string label = getLabel();

				switch(key)
				{
				case SDLK_BACKSPACE:
					label = label.substr(0, label.size()-1);
					break;
				case SDLK_SPACE:
					label = label + " ";
					break;
				case SDLK_SLASH: // '/?' key
					if(shift)
					{
						label = label + "?";
					}
					else
					{
						label = label + "/";
					}
					break;
				case SDLK_COLON:
					label = label + ":";
					break;
				case SDLK_SEMICOLON:
					label = label + ";";
					break;
				case SDLK_QUESTION:
					label = label + "?";
					break;
				case SDLK_BACKSLASH: // '|\' key
					if(shift)
					{
						label = label + "|";
					}
					else
					{
						label = label + "\\";
					}
					break;
				case SDLK_KP_DIVIDE:
					label = label + "/";
					break;
				case SDLK_MINUS:
					if(shift)
					{
						label = label + "_";
					}
					else
					{
						label = label + "-";
					}
					break;
				case SDLK_KP_MINUS:
					label = label + "-";
					break;
				case SDLK_KP_PERIOD:
				case SDLK_PERIOD:
					label = label + ".";
					break;
				case SDLK_COMMA:
					label = label + ",";
					break;
				default:
					{
						if(key >= SDLK_a && key <= SDLK_z)
						{
							char keyName[2] =
							{
								(char)(  shift ? toupper((int)key) : tolower((int)key)  ),
								0
							};

							label = label + keyName;
						}
						else
						{
							if(key==SDLK_1) label += shift ? "!" : "1";
							if(key==SDLK_2) label += shift ? "@" : "2";
							if(key==SDLK_3) label += shift ? "#" : "3";
							if(key==SDLK_4) label += shift ? "$" : "4";
							if(key==SDLK_5) label += shift ? "%" : "5";
							if(key==SDLK_6) label += shift ? "^" : "6";
							if(key==SDLK_7) label += shift ? "&" : "7";
							if(key==SDLK_8) label += shift ? "*" : "8";
							if(key==SDLK_9) label += shift ? "(" : "9";
							if(key==SDLK_0) label += shift ? ")" : "0";
						}
					}
					break;
				};

				setLabel(label);
				lastKey = key;
			}
		}
		else
		{
			debounce = false;
		}
	}
	else if(hadFocusLastTick) // Have we lost focus?
	{
		onLostFocus();
	}

	// House keeping
	hadFocusLastTick = hasFocus();
}

} // namespace Engine
