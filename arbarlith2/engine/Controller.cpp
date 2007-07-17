/*
Originally written 2004-2007 Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Modified to port to SDL and compactify the key lookup tables by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu

Rewritten as a class instead of all global functions by Andrew Fox Mar 2006

Modified to add joystick/gamepad support by Michael Collins Mar 2007
E-Mail: mailto:mdc@andrew.cmu.edu

Refactored by Andrew Fox in Apr 2007
Added support for multiple key bindings per action and actions defined at run time

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
#include "SDLwindow.h"
#include "Controller.h"

namespace Engine {

const Uint8 JOY_BUTTON = 0;
const Uint8 JOY_AXIS = 1;
const Uint8 JOY_HAT = 2;
const Uint8 JOY_BALL = 3;

ACTION_CODE Controller::nextNewActionCode = 1000;

ACTION_CODE KEY_MENU_UP				= INVALID_ACTION_CODE;
ACTION_CODE KEY_MENU_DOWN			= INVALID_ACTION_CODE;
ACTION_CODE KEY_MENU_LEFT			= INVALID_ACTION_CODE;
ACTION_CODE KEY_MENU_RIGHT			= INVALID_ACTION_CODE;
ACTION_CODE KEY_MENU				= INVALID_ACTION_CODE;
ACTION_CODE KEY_ENTER				= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_WALK_FWD		= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_WALK_REV		= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_WALK_LEFT	= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_WALK_RIGHT	= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_ROTATE_UP	= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_ROTATE_DOWN	= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_ROTATE_LEFT	= INVALID_ACTION_CODE;
ACTION_CODE KEY_EDITOR_ROTATE_RIGHT	= INVALID_ACTION_CODE;
ACTION_CODE KEY_SCREENSHOT		= INVALID_ACTION_CODE;
ACTION_CODE KEY_ENTER_EDITOR		= INVALID_ACTION_CODE;
ACTION_CODE KEY_TEST			= INVALID_ACTION_CODE;
ACTION_CODE KEY_TOGGLE_DEBUG_DATA	= INVALID_ACTION_CODE;
ACTION_CODE KEY_TOGGLE_FPS		= INVALID_ACTION_CODE;

JoyDir::JoyDir(SDL_Joystick *Joystick, Uint8 devType, int idx, Sint16 thresh)
{
	joystick = Joystick;
	Type = devType;
	DeviceIdx = idx;
	Direction = thresh;
}


Controller::Controller(void)
{
	joystick[0] = 0;
	joystick[1] = 0;
	joystick[2] = 0;
	joystick[3] = 0;

	threshold = 8192; // a good value for an XBox 360 Controller, at least

	setupControllers();

	buildKeymap();

	buildJoymap();

	buildActionNames();

	// Load key bindings from the user's application data directory
	_tstring userKeysFile = pathAppend(getAppDataDirectory(), _T("keys.xml"));

	bool exists = File::isFileOnDisk(userKeysFile);

	if(!exists || (exists && !load(userKeysFile)))
	{
		// Next time, make sure this file is ready, and at least contains defaults
		setDefaults();
		save(userKeysFile);
	}
}

Controller::~Controller(void)
{
	if(joystick[0]) SDL_JoystickClose(joystick[0]);
	if(joystick[1]) SDL_JoystickClose(joystick[1]);
	if(joystick[2]) SDL_JoystickClose(joystick[2]);
	if(joystick[3]) SDL_JoystickClose(joystick[3]);

	// TODO: We also need to free all the JoyDir pointers
}

void Controller::setupControllers(void)
{
	for(int i = 0; i < SDL_NumJoysticks() && i < 4; i++)
	{
		joystick[i] = SDL_JoystickOpen(i);

		if(joystick[i])
		{
			TRACE(_T("Joystick ") + toTString(SDL_JoystickName(i)) + _T(" Successfully opened."));
		}
	}
}

void Controller::buildKeymap(void)
{
	keymap[_T("Backspace")] = SDLK_BACKSPACE;
	keymap[_T("Tab")] = SDLK_TAB;
	keymap[_T("Clear")] = SDLK_CLEAR;
	keymap[_T("Return")] = SDLK_RETURN;
	keymap[_T("L_Shift")] = SDLK_LSHIFT;
	keymap[_T("R_Shift")] = SDLK_RSHIFT;
	keymap[_T("L_Control")] = SDLK_LCTRL;
	keymap[_T("R_Control")] = SDLK_RCTRL;
	keymap[_T("Pause")] = SDLK_BREAK;
	keymap[_T("Escape")] = SDLK_ESCAPE;
	keymap[_T("Space")] = SDLK_SPACE;
	keymap[_T("Page_Up")] = SDLK_PAGEUP;
	keymap[_T("Page_Down")] = SDLK_PAGEDOWN;
	keymap[_T("End")] = SDLK_END;
	keymap[_T("Home")] = SDLK_HOME;
	keymap[_T("Left")] = SDLK_LEFT;
	keymap[_T("Up")] = SDLK_UP;
	keymap[_T("Right")] = SDLK_RIGHT;
	keymap[_T("Down")] = SDLK_DOWN;
	keymap[_T("Insert")] = SDLK_INSERT;
	keymap[_T("Delete")] = SDLK_DELETE;
	keymap[_T("Numpad_0")] = SDLK_KP0;
	keymap[_T("Numpad_1")] = SDLK_KP1;
	keymap[_T("Numpad_2")] = SDLK_KP2;
	keymap[_T("Numpad_3")] = SDLK_KP3;
	keymap[_T("Numpad_4")] = SDLK_KP4;
	keymap[_T("Numpad_5")] = SDLK_KP5;
	keymap[_T("Numpad_6")] = SDLK_KP6;
	keymap[_T("Numpad_7")] = SDLK_KP7;
	keymap[_T("Numpad_8")] = SDLK_KP8;
	keymap[_T("Numpad_9")] = SDLK_KP9;
	keymap[_T("Numpad_Multiply")] = SDLK_KP_MULTIPLY;
	keymap[_T("Numpad_Add")] = SDLK_KP_PLUS;
	keymap[_T("Numpad_Subtract")] = SDLK_KP_MINUS;
	keymap[_T("Numpad_Dot")] = SDLK_KP_PERIOD;
	keymap[_T("Numpad_Divide")] = SDLK_KP_DIVIDE;
	keymap[_T("Numpad_Enter")] = SDLK_KP_ENTER;
	keymap[_T("F1")] = SDLK_F1;
	keymap[_T("F2")] = SDLK_F2;
	keymap[_T("F3")] = SDLK_F3;
	keymap[_T("F4")] = SDLK_F4;
	keymap[_T("F5")] = SDLK_F5;
	keymap[_T("F6")] = SDLK_F6;
	keymap[_T("F7")] = SDLK_F7;
	keymap[_T("F8")] = SDLK_F8;
	keymap[_T("F9")] = SDLK_F9;
	keymap[_T("F10")] = SDLK_F10;
	keymap[_T("F11")] = SDLK_F11;
	keymap[_T("F12")] = SDLK_F12;
	keymap[_T("F13")] = SDLK_F13;
	keymap[_T("F14")] = SDLK_F14;
	keymap[_T("F15")] = SDLK_F15;

	for(TCHAR c = _T('a'); c <= _T('z'); ++c) //alphabetic keys
	{
		SDLKey key = (SDLKey)(   (int)SDLK_a + (c-'a')   );

		TCHAR name[] = {c, 0};

		keymap[name] = key;
	}

	for(TCHAR c = _T('0'); c <= _T('9'); ++c) //numeric keys
	{
		SDLKey key = (SDLKey)(   (int)SDLK_0 + (c-_T('0'))   );

		TCHAR name[] = {c, 0};

		keymap[name] = key;
	}

	keymap[_T("Comma")] = SDLK_COMMA;
	keymap[_T("Period")] = SDLK_PERIOD;
	keymap[_T("Slash")] = SDLK_SLASH;
	keymap[_T("Semicolon")] = SDLK_SEMICOLON;
	keymap[_T("Quote")] = SDLK_QUOTE;
	keymap[_T("L_Bracket")] = SDLK_LEFTBRACKET;
	keymap[_T("R_Bracket")] = SDLK_RIGHTBRACKET;
	keymap[_T("Backslash")] = SDLK_BACKSLASH;
	keymap[_T("Underscore")] = SDLK_UNDERSCORE;
	keymap[_T("Equals")] = SDLK_EQUALS;
}

void Controller::buildJoymap(void)
{
	for(int i=0; i<4; ++i)
	{
		if(joystick[i])
		{
			const _tstring num = itoa(i+1);

			joymap[_T("JOY") + num + _T("_BUTT_0")] = new JoyDir(joystick[i], JOY_BUTTON, 0, 1);
			joymap[_T("JOY") + num + _T("_BUTT_1")] = new JoyDir(joystick[i], JOY_BUTTON, 1, 1);
			joymap[_T("JOY") + num + _T("_BUTT_2")] = new JoyDir(joystick[i], JOY_BUTTON, 2, 1);
			joymap[_T("JOY") + num + _T("_BUTT_3")] = new JoyDir(joystick[i], JOY_BUTTON, 3, 1);
			joymap[_T("JOY") + num + _T("_BUTT_4")] = new JoyDir(joystick[i], JOY_BUTTON, 4, 1);
			joymap[_T("JOY") + num + _T("_BUTT_5")] = new JoyDir(joystick[i], JOY_BUTTON, 5, 1);
			joymap[_T("JOY") + num + _T("_BUTT_6")] = new JoyDir(joystick[i], JOY_BUTTON, 6, 1);
			joymap[_T("JOY") + num + _T("_BUTT_7")] = new JoyDir(joystick[i], JOY_BUTTON, 7, 1);
			joymap[_T("JOY") + num + _T("_BUTT_8")] = new JoyDir(joystick[i], JOY_BUTTON, 8, 1);
			joymap[_T("JOY") + num + _T("_BUTT_9")] = new JoyDir(joystick[i], JOY_BUTTON, 9, 1);
			joymap[_T("JOY") + num + _T("_BUTT_10")] = new JoyDir(joystick[i], JOY_BUTTON, 10, 1);
			joymap[_T("JOY") + num + _T("_BUTT_11")] = new JoyDir(joystick[i], JOY_BUTTON, 11, 1);
			joymap[_T("JOY") + num + _T("_BUTT_12")] = new JoyDir(joystick[i], JOY_BUTTON, 12, 1);
			joymap[_T("JOY") + num + _T("_BUTT_13")] = new JoyDir(joystick[i], JOY_BUTTON, 13, 1);
			joymap[_T("JOY") + num + _T("_BUTT_14")] = new JoyDir(joystick[i], JOY_BUTTON, 14, 1);
			joymap[_T("JOY") + num + _T("_BUTT_15")] = new JoyDir(joystick[i], JOY_BUTTON, 15, 1);

			joymap[_T("JOY") + num + _T("_AXIS_X-")] = new JoyDir(joystick[i], JOY_AXIS, 0, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_X+")] = new JoyDir(joystick[i], JOY_AXIS, 0, threshold);
			joymap[_T("JOY") + num + _T("_AXIS_Y-")] = new JoyDir(joystick[i], JOY_AXIS, 1, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_Y+")] = new JoyDir(joystick[i], JOY_AXIS, 1, threshold);
			joymap[_T("JOY") + num + _T("_AXIS_U-")] = new JoyDir(joystick[i], JOY_AXIS, 2, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_U+")] = new JoyDir(joystick[i], JOY_AXIS, 2, threshold);
			joymap[_T("JOY") + num + _T("_AXIS_V-")] = new JoyDir(joystick[i], JOY_AXIS, 3, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_V+")] = new JoyDir(joystick[i], JOY_AXIS, 3, threshold);
			joymap[_T("JOY") + num + _T("_AXIS_W-")] = new JoyDir(joystick[i], JOY_AXIS, 4, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_W+")] = new JoyDir(joystick[i], JOY_AXIS, 4, threshold);
			joymap[_T("JOY") + num + _T("_AXIS_Z-")] = new JoyDir(joystick[i], JOY_AXIS, 5, -1*threshold);
			joymap[_T("JOY") + num + _T("_AXIS_Z+")] = new JoyDir(joystick[i], JOY_AXIS, 5, threshold);

			joymap[_T("JOY") + num + _T("_HAT_DOWN")]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_DOWN);
			joymap[_T("JOY") + num + _T("_HAT_LEFT")]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_LEFT);
			joymap[_T("JOY") + num + _T("_HAT_UP")]		= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_UP);
			joymap[_T("JOY") + num + _T("_HAT_RIGHT")]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_RIGHT);
		}
	}
}

void Controller::buildActionNames(void)
{
	KEY_MENU_UP		= createAction(_T("Menu-Up"));
	KEY_MENU_DOWN		= createAction(_T("Menu-Down"));
	KEY_MENU_LEFT		= createAction(_T("Menu-Left"));
	KEY_MENU_RIGHT		= createAction(_T("Menu-Right"));
	KEY_MENU		= createAction(_T("Menu"));
	KEY_ENTER		= createAction(_T("Enter"));
	KEY_EDITOR_WALK_FWD	= createAction(_T("Editor-Walk-Fwd"));
	KEY_EDITOR_WALK_REV	= createAction(_T("Editor-Walk-Rev"));
	KEY_EDITOR_WALK_LEFT	= createAction(_T("Editor-Walk-Left"));
	KEY_EDITOR_WALK_RIGHT	= createAction(_T("Editor-Walk-Right"));
	KEY_EDITOR_ROTATE_UP	= createAction(_T("Editor-Rotate-Up"));
	KEY_EDITOR_ROTATE_DOWN	= createAction(_T("Editor-Rotate-Down"));
	KEY_EDITOR_ROTATE_LEFT	= createAction(_T("Editor-Rotate-Left"));
	KEY_EDITOR_ROTATE_RIGHT	= createAction(_T("Editor-Rotate-Right"));
	KEY_SCREENSHOT		= createAction(_T("Screenshot"));
	KEY_ENTER_EDITOR	= createAction(_T("Editor"));
	KEY_TEST		= createAction(_T("Test"));
	KEY_TOGGLE_DEBUG_DATA	= createAction(_T("Toggle-Debug-Data"));
	KEY_TOGGLE_FPS		= createAction(_T("Toggle-FPS"));
}

void Controller::setDefaults(void)
{
	// Keyboard
	addBinding(KEY_MENU_UP,			_T("Up"));
	addBinding(KEY_MENU_DOWN,		_T("Down"));
	addBinding(KEY_MENU_LEFT,		_T("Left"));
	addBinding(KEY_MENU_RIGHT,		_T("Right"));
	addBinding(KEY_MENU,			_T("Escape"));
	addBinding(KEY_ENTER,			_T("Return"));
	addBinding(KEY_EDITOR_WALK_FWD,	_T("w"));
	addBinding(KEY_EDITOR_WALK_REV,	_T("s"));
	addBinding(KEY_EDITOR_WALK_LEFT,	_T("a"));
	addBinding(KEY_EDITOR_WALK_RIGHT,	_T("d"));
	addBinding(KEY_EDITOR_ROTATE_UP,	_T("Up"));
	addBinding(KEY_EDITOR_ROTATE_DOWN,	_T("Down"));
	addBinding(KEY_EDITOR_ROTATE_LEFT,	_T("Left"));
	addBinding(KEY_EDITOR_ROTATE_RIGHT,	_T("Right"));
	addBinding(KEY_SCREENSHOT,		_T("F11"));
	addBinding(KEY_ENTER_EDITOR,		_T("F1"));
	addBinding(KEY_TEST,			_T("Tab"));
	addBinding(KEY_TOGGLE_DEBUG_DATA,	_T("F2"));
	addBinding(KEY_TOGGLE_FPS,		_T("F3"));

	// Joystick
	addBinding(KEY_MENU_UP,			_T("JOY1_AXIS_Y-"));
	addBinding(KEY_MENU_UP,			_T("JOY2_AXIS_Y-"));
	addBinding(KEY_MENU_UP,			_T("JOY3_AXIS_Y-"));
	addBinding(KEY_MENU_UP,			_T("JOY4_AXIS_Y-"));
	addBinding(KEY_MENU_DOWN,		_T("JOY1_AXIS_Y+"));
	addBinding(KEY_MENU_DOWN,		_T("JOY2_AXIS_Y+"));
	addBinding(KEY_MENU_DOWN,		_T("JOY3_AXIS_Y+"));
	addBinding(KEY_MENU_DOWN,		_T("JOY4_AXIS_Y+"));
	addBinding(KEY_MENU_LEFT,		_T("JOY1_AXIS_X-"));
	addBinding(KEY_MENU_LEFT,		_T("JOY2_AXIS_X-"));
	addBinding(KEY_MENU_LEFT,		_T("JOY3_AXIS_X-"));
	addBinding(KEY_MENU_LEFT,		_T("JOY4_AXIS_X-"));
	addBinding(KEY_MENU_RIGHT,		_T("JOY1_AXIS_X+"));
	addBinding(KEY_MENU_RIGHT,		_T("JOY2_AXIS_X+"));
	addBinding(KEY_MENU_RIGHT,		_T("JOY3_AXIS_X+"));
	addBinding(KEY_MENU_RIGHT,		_T("JOY4_AXIS_X+"));
	addBinding(KEY_MENU,			_T("JOY1_BUTT_7"));
	addBinding(KEY_MENU,			_T("JOY2_BUTT_7"));
	addBinding(KEY_MENU,			_T("JOY3_BUTT_7"));
	addBinding(KEY_MENU,			_T("JOY4_BUTT_7"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_0"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_1"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_2"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_3"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_4"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_5"));
	addBinding(KEY_ENTER,			_T("JOY1_BUTT_6"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_0"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_1"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_2"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_3"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_4"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_5"));
	addBinding(KEY_ENTER,			_T("JOY2_BUTT_6"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_0"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_1"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_2"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_3"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_4"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_5"));
	addBinding(KEY_ENTER,			_T("JOY3_BUTT_6"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_0"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_1"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_2"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_3"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_4"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_5"));
	addBinding(KEY_ENTER,			_T("JOY4_BUTT_6"));
	addBinding(KEY_EDITOR_WALK_FWD,	_T("JOY1_AXIS_Y-"));
	addBinding(KEY_EDITOR_WALK_REV,	_T("JOY1_AXIS_Y+"));
	addBinding(KEY_EDITOR_WALK_LEFT,	_T("JOY1_AXIS_X-"));
	addBinding(KEY_EDITOR_WALK_RIGHT,	_T("JOY1_AXIS_X+"));
	addBinding(KEY_EDITOR_ROTATE_UP,	_T("JOY1_AXIS_U+"));
	addBinding(KEY_EDITOR_ROTATE_DOWN,	_T("JOY1_AXIS_U-"));
	addBinding(KEY_EDITOR_ROTATE_LEFT,	_T("JOY1_AXIS_V-"));
	addBinding(KEY_EDITOR_ROTATE_RIGHT,	_T("JOY1_AXIS_V+"));
//	addBinding(KEY_SCREENSHOT,		_T("Up"));
//	addBinding(KEY_ENTER_EDITOR,		_T("Up"));
//	addBinding(KEY_TEST,			_T("Up"));
}

bool Controller::isKeyDown(ACTION_CODE actionCode)
{
	if(bindings.find(actionCode) != bindings.end())
	{
		multimap<ACTION_CODE, _tstring>::const_iterator i = bindings.lower_bound(actionCode);
		multimap<ACTION_CODE, _tstring>::const_iterator stop = bindings.upper_bound(actionCode);

		while(i!=stop)
		{
			const _tstring &keyName = i->second;

			if(keymap[keyName]) // is it bound to the keyboard?
			{
				SDLKey keyCode = keymap[keyName];
				if(g_Input.Keys[keyCode])
					return true;
			}

			if(joymap[keyName]) // is it bound to a joystick?
			{
				JoyDir* joyDir = joymap[keyName];
				if(hasJoyEventOccured(joyDir))
					return true;
			}

			++i;
		}
	}

	return false;
}

bool Controller::isAnyKeyDown(void)
{
	for(size_t i=0; i<sizeof(g_Input.Keys); ++i)
	{
		if(g_Input.Keys[i])
		{
			return true;
		}
	}

	for(map<_tstring, JoyDir*>::const_iterator i = joymap.begin(); i != joymap.end(); ++i)
	{
		if(hasJoyEventOccured(i->second))
		{
			return true;
		}
	}

	return false;
}

bool Controller::getKey(size_t &key, bool &shift)
{
	for(size_t i=0; i<sizeof(g_Input.Keys); ++i)
	{
		if(g_Input.Keys[i])
		{
			shift = g_Input.Keys[SDLK_LSHIFT] || g_Input.Keys[SDLK_RSHIFT];
			key = i;
			return true;
		}
	}

	return false;
}

bool Controller::hasJoyEventOccured(JoyDir* subfunct)
{
	ASSERT(subfunct!=0, _T("subfunct was null"));

	switch(subfunct->Type)
	{
	case JOY_BUTTON:
		return( SDL_JoystickGetButton(subfunct->joystick, subfunct->DeviceIdx) == 1);

	case JOY_AXIS:
		if(subfunct->Direction > 0)
			return( SDL_JoystickGetAxis(subfunct->joystick, subfunct->DeviceIdx) >= subfunct->Direction );
		else
			return( SDL_JoystickGetAxis(subfunct->joystick, subfunct->DeviceIdx) <= subfunct->Direction );

	case JOY_HAT:
		return( (SDL_JoystickGetHat(subfunct->joystick, subfunct->DeviceIdx) & (Uint8) subfunct->Direction) != 0);

	case JOY_BALL:
		return false; // Who the fuck has a ball on their stick?

	default:
		return false;
	}
}

vector<_tstring> Controller::getKeyName(ACTION_CODE actionCode)
{
	vector<_tstring> keysBound;

	if(bindings.find(actionCode) != bindings.end())
	{
		multimap<ACTION_CODE, _tstring>::const_iterator i = bindings.lower_bound(actionCode);
		multimap<ACTION_CODE, _tstring>::const_iterator stop = bindings.upper_bound(actionCode);

		while(i!=stop)
		{
			keysBound.push_back(i->second);
			++i;
		}
	}
	else
	{
		keysBound.push_back(_T("[NO KEY BOUND]"));
	}

	return keysBound;
}

bool Controller::load(const _tstring &fileName)
{
	PropertyBag Bag;
	if(!Bag.Load(fileName))
	{
		return false;
	}

	setDefaults();

	const size_t numActions = Bag.getNumInstances(_T("action"));
	for(size_t j=0; j<numActions; ++j)
	{
		PropertyBag actionBag;
		_tstring actionName;

		Bag.get(_T("action"), actionBag, j);
		actionBag.get(_T("name"), actionName);

		ACTION_CODE actionCode = createAction(actionName);

		// Load one or more keys for this action
		const size_t numBindings = Bag.getNumInstances(_T("binding"));
		for(size_t i=0; i<numBindings; ++i)
		{
			_tstring keyName;
			if(Bag.get(actionName, keyName, i))
			{
				bindings.insert(make_pair(actionCode, keyName)); // multimap allows duplicate keys
			}
		}
	}

	return true;
}

bool Controller::save(const _tstring &filename)
{
	PropertyBag Bag;

	for(map<ACTION_CODE, _tstring>::const_iterator iter = actionNames.begin(); iter != actionNames.end(); ++iter)
	{
		const ACTION_CODE &actionCode = iter->first;
		const _tstring &actionName = iter->second;

		PropertyBag actionBag;
		actionBag.add(_T("name"), actionName);

		if(bindings.find(actionCode) != bindings.end())
		{
			multimap<ACTION_CODE, _tstring>::const_iterator i = bindings.lower_bound(actionCode);
			multimap<ACTION_CODE, _tstring>::const_iterator stop = bindings.upper_bound(actionCode);

			// Save the (possibly) multiple bindings for this action
			while(i!=stop)
			{
				actionBag.add(_T("binding"), i->second);
				++i;
			}
		}

		Bag.add(_T("action"), actionBag);
	}

	Bag.saveToFile(filename);
	return true;
}

void Controller::deleteAction(ACTION_CODE action)
{
	// Erase all bindings for this action
	if(bindings.find(action) != bindings.end())
	{
		multimap<ACTION_CODE, _tstring>::iterator i = bindings.lower_bound(action);
		multimap<ACTION_CODE, _tstring>::iterator stop = bindings.upper_bound(action);

		while(i!=stop)
		{
			/*
			Is there a more elegant way to do this?
			*/

			multimap<ACTION_CODE, _tstring>::iterator nextIter = i;
			nextIter++;

			bindings.erase(i);

			i = nextIter;
		}
	}

	// Erase the action
	map<ACTION_CODE, _tstring>::iterator i = actionNames.find(action);
	if(i != actionNames.end())
	{
		actionNames.erase(i);
	}
}

ACTION_CODE Controller::createAction(const _tstring &name)
{
	for(map<ACTION_CODE, _tstring>::const_iterator iter = actionNames.begin(); iter != actionNames.end(); ++iter)
	{
		if(toLowerCase(name) == toLowerCase(iter->second))
		{
			return iter->first;
		}
	}

	ACTION_CODE code = nextNewActionCode;
	nextNewActionCode++;
	actionNames.insert(make_pair(code, name));
	return code;
}

void Controller::addBinding(ACTION_CODE action, const _tstring &binding)
{
	// Do not allow duplicate bindings
	if(bindings.find(action) != bindings.end())
	{
		multimap<ACTION_CODE, _tstring>::const_iterator i = bindings.lower_bound(action);
		multimap<ACTION_CODE, _tstring>::const_iterator stop = bindings.upper_bound(action);

		while(i!=stop)
		{
			if(i->second == binding)
				return;

			++i;
		}
	}

	// allow duplicate keys
	bindings.insert(make_pair(action, binding));
}

}; // namespace
