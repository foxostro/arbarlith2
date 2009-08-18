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
#include "file.h"
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
	string userKeysFile = pathAppend(getAppDataDirectory(), "keys.xml");

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
			TRACE(string("Joystick ") + SDL_JoystickName(i) + " Successfully opened.");
		}
	}
}

void Controller::buildKeymap(void)
{
	keymap["Backspace"] = SDLK_BACKSPACE;
	keymap["Tab"] = SDLK_TAB;
	keymap["Clear"] = SDLK_CLEAR;
	keymap["Return"] = SDLK_RETURN;
	keymap["L_Shift"] = SDLK_LSHIFT;
	keymap["R_Shift"] = SDLK_RSHIFT;
	keymap["L_Control"] = SDLK_LCTRL;
	keymap["R_Control"] = SDLK_RCTRL;
	keymap["Pause"] = SDLK_BREAK;
	keymap["Escape"] = SDLK_ESCAPE;
	keymap["Space"] = SDLK_SPACE;
	keymap["Page_Up"] = SDLK_PAGEUP;
	keymap["Page_Down"] = SDLK_PAGEDOWN;
	keymap["End"] = SDLK_END;
	keymap["Home"] = SDLK_HOME;
	keymap["Left"] = SDLK_LEFT;
	keymap["Up"] = SDLK_UP;
	keymap["Right"] = SDLK_RIGHT;
	keymap["Down"] = SDLK_DOWN;
	keymap["Insert"] = SDLK_INSERT;
	keymap["Delete"] = SDLK_DELETE;
	keymap["Numpad_0"] = SDLK_KP0;
	keymap["Numpad_1"] = SDLK_KP1;
	keymap["Numpad_2"] = SDLK_KP2;
	keymap["Numpad_3"] = SDLK_KP3;
	keymap["Numpad_4"] = SDLK_KP4;
	keymap["Numpad_5"] = SDLK_KP5;
	keymap["Numpad_6"] = SDLK_KP6;
	keymap["Numpad_7"] = SDLK_KP7;
	keymap["Numpad_8"] = SDLK_KP8;
	keymap["Numpad_9"] = SDLK_KP9;
	keymap["Numpad_Multiply"] = SDLK_KP_MULTIPLY;
	keymap["Numpad_Add"] = SDLK_KP_PLUS;
	keymap["Numpad_Subtract"] = SDLK_KP_MINUS;
	keymap["Numpad_Dot"] = SDLK_KP_PERIOD;
	keymap["Numpad_Divide"] = SDLK_KP_DIVIDE;
	keymap["Numpad_Enter"] = SDLK_KP_ENTER;
	keymap["F1"] = SDLK_F1;
	keymap["F2"] = SDLK_F2;
	keymap["F3"] = SDLK_F3;
	keymap["F4"] = SDLK_F4;
	keymap["F5"] = SDLK_F5;
	keymap["F6"] = SDLK_F6;
	keymap["F7"] = SDLK_F7;
	keymap["F8"] = SDLK_F8;
	keymap["F9"] = SDLK_F9;
	keymap["F10"] = SDLK_F10;
	keymap["F11"] = SDLK_F11;
	keymap["F12"] = SDLK_F12;
	keymap["F13"] = SDLK_F13;
	keymap["F14"] = SDLK_F14;
	keymap["F15"] = SDLK_F15;

	for(char c = 'a'; c <= 'z'; ++c) //alphabetic keys
	{
		SDLKey key = (SDLKey)(   (int)SDLK_a + (c-'a')   );

		char name[] = {c, 0};

		keymap[name] = key;
	}

	for(char c = '0'; c <= '9'; ++c) //numeric keys
	{
		SDLKey key = (SDLKey)(   (int)SDLK_0 + (c-'0')   );

		char name[] = {c, 0};

		keymap[name] = key;
	}

	keymap["Comma"] = SDLK_COMMA;
	keymap["Period"] = SDLK_PERIOD;
	keymap["Slash"] = SDLK_SLASH;
	keymap["Semicolon"] = SDLK_SEMICOLON;
	keymap["Quote"] = SDLK_QUOTE;
	keymap["L_Bracket"] = SDLK_LEFTBRACKET;
	keymap["R_Bracket"] = SDLK_RIGHTBRACKET;
	keymap["Backslash"] = SDLK_BACKSLASH;
	keymap["Underscore"] = SDLK_UNDERSCORE;
	keymap["Equals"] = SDLK_EQUALS;
}

void Controller::buildJoymap(void)
{
	for(int i=0; i<4; ++i)
	{
		if(joystick[i])
		{
			const string num = itoa(i+1);

			joymap["JOY" + num + "_BUTT_0"] = new JoyDir(joystick[i], JOY_BUTTON, 0, 1);
			joymap["JOY" + num + "_BUTT_1"] = new JoyDir(joystick[i], JOY_BUTTON, 1, 1);
			joymap["JOY" + num + "_BUTT_2"] = new JoyDir(joystick[i], JOY_BUTTON, 2, 1);
			joymap["JOY" + num + "_BUTT_3"] = new JoyDir(joystick[i], JOY_BUTTON, 3, 1);
			joymap["JOY" + num + "_BUTT_4"] = new JoyDir(joystick[i], JOY_BUTTON, 4, 1);
			joymap["JOY" + num + "_BUTT_5"] = new JoyDir(joystick[i], JOY_BUTTON, 5, 1);
			joymap["JOY" + num + "_BUTT_6"] = new JoyDir(joystick[i], JOY_BUTTON, 6, 1);
			joymap["JOY" + num + "_BUTT_7"] = new JoyDir(joystick[i], JOY_BUTTON, 7, 1);
			joymap["JOY" + num + "_BUTT_8"] = new JoyDir(joystick[i], JOY_BUTTON, 8, 1);
			joymap["JOY" + num + "_BUTT_9"] = new JoyDir(joystick[i], JOY_BUTTON, 9, 1);
			joymap["JOY" + num + "_BUTT_10"] = new JoyDir(joystick[i], JOY_BUTTON, 10, 1);
			joymap["JOY" + num + "_BUTT_11"] = new JoyDir(joystick[i], JOY_BUTTON, 11, 1);
			joymap["JOY" + num + "_BUTT_12"] = new JoyDir(joystick[i], JOY_BUTTON, 12, 1);
			joymap["JOY" + num + "_BUTT_13"] = new JoyDir(joystick[i], JOY_BUTTON, 13, 1);
			joymap["JOY" + num + "_BUTT_14"] = new JoyDir(joystick[i], JOY_BUTTON, 14, 1);
			joymap["JOY" + num + "_BUTT_15"] = new JoyDir(joystick[i], JOY_BUTTON, 15, 1);

			joymap["JOY" + num + "_AXIS_X-"] = new JoyDir(joystick[i], JOY_AXIS, 0, -1*threshold);
			joymap["JOY" + num + "_AXIS_X+"] = new JoyDir(joystick[i], JOY_AXIS, 0, threshold);
			joymap["JOY" + num + "_AXIS_Y-"] = new JoyDir(joystick[i], JOY_AXIS, 1, -1*threshold);
			joymap["JOY" + num + "_AXIS_Y+"] = new JoyDir(joystick[i], JOY_AXIS, 1, threshold);
			joymap["JOY" + num + "_AXIS_U-"] = new JoyDir(joystick[i], JOY_AXIS, 2, -1*threshold);
			joymap["JOY" + num + "_AXIS_U+"] = new JoyDir(joystick[i], JOY_AXIS, 2, threshold);
			joymap["JOY" + num + "_AXIS_V-"] = new JoyDir(joystick[i], JOY_AXIS, 3, -1*threshold);
			joymap["JOY" + num + "_AXIS_V+"] = new JoyDir(joystick[i], JOY_AXIS, 3, threshold);
			joymap["JOY" + num + "_AXIS_W-"] = new JoyDir(joystick[i], JOY_AXIS, 4, -1*threshold);
			joymap["JOY" + num + "_AXIS_W+"] = new JoyDir(joystick[i], JOY_AXIS, 4, threshold);
			joymap["JOY" + num + "_AXIS_Z-"] = new JoyDir(joystick[i], JOY_AXIS, 5, -1*threshold);
			joymap["JOY" + num + "_AXIS_Z+"] = new JoyDir(joystick[i], JOY_AXIS, 5, threshold);

			joymap["JOY" + num + "_HAT_DOWN"]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_DOWN);
			joymap["JOY" + num + "_HAT_LEFT"]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_LEFT);
			joymap["JOY" + num + "_HAT_UP"]		= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_UP);
			joymap["JOY" + num + "_HAT_RIGHT"]	= new JoyDir(joystick[i], JOY_HAT, 0, SDL_HAT_RIGHT);
		}
	}
}

void Controller::buildActionNames(void)
{
	KEY_MENU_UP		= createAction("Menu-Up");
	KEY_MENU_DOWN		= createAction("Menu-Down");
	KEY_MENU_LEFT		= createAction("Menu-Left");
	KEY_MENU_RIGHT		= createAction("Menu-Right");
	KEY_MENU		= createAction("Menu");
	KEY_ENTER		= createAction("Enter");
	KEY_EDITOR_WALK_FWD	= createAction("Editor-Walk-Fwd");
	KEY_EDITOR_WALK_REV	= createAction("Editor-Walk-Rev");
	KEY_EDITOR_WALK_LEFT	= createAction("Editor-Walk-Left");
	KEY_EDITOR_WALK_RIGHT	= createAction("Editor-Walk-Right");
	KEY_EDITOR_ROTATE_UP	= createAction("Editor-Rotate-Up");
	KEY_EDITOR_ROTATE_DOWN	= createAction("Editor-Rotate-Down");
	KEY_EDITOR_ROTATE_LEFT	= createAction("Editor-Rotate-Left");
	KEY_EDITOR_ROTATE_RIGHT	= createAction("Editor-Rotate-Right");
	KEY_SCREENSHOT		= createAction("Screenshot");
	KEY_ENTER_EDITOR	= createAction("Editor");
	KEY_TEST		= createAction("Test");
	KEY_TOGGLE_DEBUG_DATA	= createAction("Toggle-Debug-Data");
	KEY_TOGGLE_FPS		= createAction("Toggle-FPS");
}

void Controller::setDefaults(void)
{
	// Keyboard
	addBinding(KEY_MENU_UP,			    "Up");
	addBinding(KEY_MENU_DOWN,		    "Down");
	addBinding(KEY_MENU_LEFT,		    "Left");
	addBinding(KEY_MENU_RIGHT,		    "Right");
	addBinding(KEY_MENU,			    "Escape");
	addBinding(KEY_ENTER,			    "Return");
	addBinding(KEY_EDITOR_WALK_FWD,     "w");
	addBinding(KEY_EDITOR_WALK_REV,	    "s");
	addBinding(KEY_EDITOR_WALK_LEFT,	"a");
	addBinding(KEY_EDITOR_WALK_RIGHT,	"d");
	addBinding(KEY_EDITOR_ROTATE_UP,	"Up");
	addBinding(KEY_EDITOR_ROTATE_DOWN,	"Down");
	addBinding(KEY_EDITOR_ROTATE_LEFT,	"Left");
	addBinding(KEY_EDITOR_ROTATE_RIGHT,	"Right");
	addBinding(KEY_SCREENSHOT,		    "F11");
	addBinding(KEY_ENTER_EDITOR,		"F1");
	addBinding(KEY_TEST,			    "Tab");
	addBinding(KEY_TOGGLE_DEBUG_DATA,	"F2");
	addBinding(KEY_TOGGLE_FPS,		    "F3");

	// Joystick
	addBinding(KEY_MENU_UP,			"JOY1_AXIS_Y-");
	addBinding(KEY_MENU_UP,			"JOY2_AXIS_Y-");
	addBinding(KEY_MENU_UP,			"JOY3_AXIS_Y-");
	addBinding(KEY_MENU_UP,			"JOY4_AXIS_Y-");
	addBinding(KEY_MENU_DOWN,		"JOY1_AXIS_Y+");
	addBinding(KEY_MENU_DOWN,		"JOY2_AXIS_Y+");
	addBinding(KEY_MENU_DOWN,		"JOY3_AXIS_Y+");
	addBinding(KEY_MENU_DOWN,		"JOY4_AXIS_Y+");
	addBinding(KEY_MENU_LEFT,		"JOY1_AXIS_X-");
	addBinding(KEY_MENU_LEFT,		"JOY2_AXIS_X-");
	addBinding(KEY_MENU_LEFT,		"JOY3_AXIS_X-");
	addBinding(KEY_MENU_LEFT,		"JOY4_AXIS_X-");
	addBinding(KEY_MENU_RIGHT,		"JOY1_AXIS_X+");
	addBinding(KEY_MENU_RIGHT,		"JOY2_AXIS_X+");
	addBinding(KEY_MENU_RIGHT,		"JOY3_AXIS_X+");
	addBinding(KEY_MENU_RIGHT,		"JOY4_AXIS_X+");
	addBinding(KEY_MENU,			"JOY1_BUTT_7");
	addBinding(KEY_MENU,			"JOY2_BUTT_7");
	addBinding(KEY_MENU,			"JOY3_BUTT_7");
	addBinding(KEY_MENU,			"JOY4_BUTT_7");
	addBinding(KEY_ENTER,			"JOY1_BUTT_0");
	addBinding(KEY_ENTER,			"JOY1_BUTT_1");
	addBinding(KEY_ENTER,			"JOY1_BUTT_2");
	addBinding(KEY_ENTER,			"JOY1_BUTT_3");
	addBinding(KEY_ENTER,			"JOY1_BUTT_4");
	addBinding(KEY_ENTER,			"JOY1_BUTT_5");
	addBinding(KEY_ENTER,			"JOY1_BUTT_6");
	addBinding(KEY_ENTER,			"JOY2_BUTT_0");
	addBinding(KEY_ENTER,			"JOY2_BUTT_1");
	addBinding(KEY_ENTER,			"JOY2_BUTT_2");
	addBinding(KEY_ENTER,			"JOY2_BUTT_3");
	addBinding(KEY_ENTER,			"JOY2_BUTT_4");
	addBinding(KEY_ENTER,			"JOY2_BUTT_5");
	addBinding(KEY_ENTER,			"JOY2_BUTT_6");
	addBinding(KEY_ENTER,			"JOY3_BUTT_0");
	addBinding(KEY_ENTER,			"JOY3_BUTT_1");
	addBinding(KEY_ENTER,			"JOY3_BUTT_2");
	addBinding(KEY_ENTER,			"JOY3_BUTT_3");
	addBinding(KEY_ENTER,			"JOY3_BUTT_4");
	addBinding(KEY_ENTER,			"JOY3_BUTT_5");
	addBinding(KEY_ENTER,			"JOY3_BUTT_6");
	addBinding(KEY_ENTER,			"JOY4_BUTT_0");
	addBinding(KEY_ENTER,			"JOY4_BUTT_1");
	addBinding(KEY_ENTER,			"JOY4_BUTT_2");
	addBinding(KEY_ENTER,			"JOY4_BUTT_3");
	addBinding(KEY_ENTER,			"JOY4_BUTT_4");
	addBinding(KEY_ENTER,			"JOY4_BUTT_5");
	addBinding(KEY_ENTER,			"JOY4_BUTT_6");
	addBinding(KEY_EDITOR_WALK_FWD,	"JOY1_AXIS_Y-");
	addBinding(KEY_EDITOR_WALK_REV,	"JOY1_AXIS_Y+");
	addBinding(KEY_EDITOR_WALK_LEFT,	"JOY1_AXIS_X-");
	addBinding(KEY_EDITOR_WALK_RIGHT,	"JOY1_AXIS_X+");
	addBinding(KEY_EDITOR_ROTATE_UP,	"JOY1_AXIS_U+");
	addBinding(KEY_EDITOR_ROTATE_DOWN,	"JOY1_AXIS_U-");
	addBinding(KEY_EDITOR_ROTATE_LEFT,	"JOY1_AXIS_V-");
	addBinding(KEY_EDITOR_ROTATE_RIGHT,	"JOY1_AXIS_V+");
//	addBinding(KEY_SCREENSHOT,		"Up");
//	addBinding(KEY_ENTER_EDITOR,		"Up");
//	addBinding(KEY_TEST,			"Up");
}

bool Controller::isKeyDown(ACTION_CODE actionCode)
{
	if(bindings.find(actionCode) != bindings.end())
	{
		multimap<ACTION_CODE, string>::const_iterator i = bindings.lower_bound(actionCode);
		multimap<ACTION_CODE, string>::const_iterator stop = bindings.upper_bound(actionCode);

		while(i!=stop)
		{
			const string &keyName = i->second;

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

	for(map<string, JoyDir*>::const_iterator i = joymap.begin(); i != joymap.end(); ++i)
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
	ASSERT(subfunct!=0, "subfunct was null");

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

vector<string> Controller::getKeyName(ACTION_CODE actionCode)
{
	vector<string> keysBound;

	if(bindings.find(actionCode) != bindings.end())
	{
		multimap<ACTION_CODE, string>::const_iterator i = bindings.lower_bound(actionCode);
		multimap<ACTION_CODE, string>::const_iterator stop = bindings.upper_bound(actionCode);

		while(i!=stop)
		{
			keysBound.push_back(i->second);
			++i;
		}
	}
	else
	{
		keysBound.push_back("[NO KEY BOUND]");
	}

	return keysBound;
}

bool Controller::load(const string &fileName)
{
	PropertyBag Bag;
	Bag.loadFromFile(fileName);

	setDefaults();

	for(size_t j=0, numActions=Bag.getNumInstances("action"); j<numActions; ++j)
	{
		PropertyBag actionBag = Bag.getBag("action", j);
		string actionName = actionBag.getString("name");

		ACTION_CODE actionCode = createAction(actionName);

		// Load one or more keys for this action
		for(size_t i=0, numBindings=Bag.getNumInstances("binding"); i<numBindings; ++i)
		{
			bindings.insert(  make_pair(actionCode, Bag.getString(actionName, i))  );
		}
	}

	return true;
}

bool Controller::save(const string &filename)
{
	PropertyBag Bag;

	for(map<ACTION_CODE, string>::const_iterator iter = actionNames.begin(); iter != actionNames.end(); ++iter)
	{
		const ACTION_CODE &actionCode = iter->first;
		const string &actionName = iter->second;

		PropertyBag actionBag;
		actionBag.add("name", actionName);

		if(bindings.find(actionCode) != bindings.end())
		{
			multimap<ACTION_CODE, string>::const_iterator i = bindings.lower_bound(actionCode);
			multimap<ACTION_CODE, string>::const_iterator stop = bindings.upper_bound(actionCode);

			// Save the (possibly) multiple bindings for this action
			while(i!=stop)
			{
				actionBag.add("binding", i->second);
				++i;
			}
		}

		Bag.add("action", actionBag);
	}

	Bag.saveToFile(filename);
	return true;
}

void Controller::deleteAction(ACTION_CODE action)
{
	// Erase all bindings for this action
	if(bindings.find(action) != bindings.end())
	{
		multimap<ACTION_CODE, string>::iterator i = bindings.lower_bound(action);
		multimap<ACTION_CODE, string>::iterator stop = bindings.upper_bound(action);

		while(i!=stop)
		{
			/*
			Is there a more elegant way to do this?
			*/

			multimap<ACTION_CODE, string>::iterator nextIter = i;
			nextIter++;

			bindings.erase(i);

			i = nextIter;
		}
	}

	// Erase the action
	map<ACTION_CODE, string>::iterator i = actionNames.find(action);
	if(i != actionNames.end())
	{
		actionNames.erase(i);
	}
}

ACTION_CODE Controller::createAction(const string &name)
{
	for(map<ACTION_CODE, string>::const_iterator iter = actionNames.begin(); iter != actionNames.end(); ++iter)
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

void Controller::addBinding(ACTION_CODE action, const string &binding)
{
	// Do not allow duplicate bindings
	if(bindings.find(action) != bindings.end())
	{
		multimap<ACTION_CODE, string>::const_iterator i = bindings.lower_bound(action);
		multimap<ACTION_CODE, string>::const_iterator stop = bindings.upper_bound(action);

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
