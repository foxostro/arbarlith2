/*
Copyright © 2006 Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
Changed to use SDL keys instead of VK keys by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu
*/

#include "stdafx.h"
#include "SDLwindow.h"
#include "EditWidget.h"


namespace Engine { 



EditWidget *EditWidget::currentlyFocused = 0;

EditWidget::EditWidget(const _tstring &label, float x, float y, FONT_SIZE size)
:debounce(false),
shift(false),
lastKey(0),
hadFocusLastTick(false),
LabelWidget(label, x, y, white, size)
{}

void EditWidget::onAcquireFocus(void)
{
	m_Color = red;
}

void EditWidget::onLostFocus(void)
{
	m_Color = white;
}

void EditWidget::onMouseEnter(void)
{
	if(hasFocus())
	{
		m_Color = red;
	}
	else
	{
		m_Color = yellow;
	}
}

void EditWidget::onMouseExit(void)
{
	if(hasFocus())
	{
		m_Color = red;
	}
	else
	{
		m_Color = white;
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

	g_SoundSystem.play(_T("data/sound/click.wav"));
}

bool EditWidget::hasFocus(void) const
{
	return(currentlyFocused == this);
}

void EditWidget::update(float)
{
	__super::onMouseClick();

	if(hasFocus())
	{
		size_t key=0;

		// Get keyboard input
		if(g_Keys.getKey(key, shift))
		{
			if(debounce == false || lastKey!=key)
			{
				debounce = true;

				_tstring label = getLabel();

				switch(key)
				{
				case SDLK_BACKSPACE:
					label = label.substr(0, label.size()-1);
					break;
				case SDLK_SPACE:
					label = label + _T(" ");
					break;
				case SDLK_SLASH: // '/?' key
					if(shift)
					{
						label = label + _T("?");
					}
					else
					{
						label = label + _T("/");
					}
					break;
				case SDLK_COLON:
					label = label + _T(":");
					break;
				case SDLK_SEMICOLON:
					label = label + _T(";");
					break;
				case SDLK_QUESTION:
					label = label + _T("?");
					break;
				case SDLK_BACKSLASH: // '|\' key
					if(shift)
					{
						label = label + _T("|");
					}
					else
					{
						label = label + _T("\\");
					}
					break;
				case SDLK_KP_DIVIDE:
					label = label + _T("/");
					break;
				case SDLK_MINUS:
					if(shift)
					{
						label = label + _T("_");
					}
					else
					{
						label = label + _T("-");
					}
					break;
				case SDLK_KP_MINUS:
					label = label + _T("-");
					break;
				case SDLK_KP_PERIOD:
				case SDLK_PERIOD:
					label = label + _T(".");
					break;
				case SDLK_COMMA:
					label = label + _T(",");
					break;
				default:
					{
						if(key >= SDLK_a && key <= SDLK_z)
						{
							TCHAR keyName[2] =
							{
								(TCHAR)(  shift ? toupper((int)key) : tolower((int)key)  ),
								0
							};

							label = label + keyName;
						}
						else
						{
							if(key==SDLK_1) label += shift ? _T("!") : _T("1");
							if(key==SDLK_2) label += shift ? _T("@") : _T("2");
							if(key==SDLK_3) label += shift ? _T("#") : _T("3");
							if(key==SDLK_4) label += shift ? _T("$") : _T("4");
							if(key==SDLK_5) label += shift ? _T("%") : _T("5");
							if(key==SDLK_6) label += shift ? _T("^") : _T("6");
							if(key==SDLK_7) label += shift ? _T("&") : _T("7");
							if(key==SDLK_8) label += shift ? _T("*") : _T("8");
							if(key==SDLK_9) label += shift ? _T("(") : _T("9");
							if(key==SDLK_0) label += shift ? _T(")") : _T("0");
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

}; // namespace
