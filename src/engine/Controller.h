/*
Originally written 2004-2007 Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Modified to port to SDL and compactify the key lookup tables by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu

Rewritten as a class instead of all global functions by Andrew Fox Mar 2006

Modified to add joystick/gamepad support by Michael Collins Mar 2007
E-Mail: mailto:mdc@andrew.cmu.edu

Spruced up by Andrew Fox in Apr 2007
Added support for multiple key bindings per action and actions defined at run time

Copyright (c) 2007,2009 Game Creation Society
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

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <SDL/SDL.h>
#include <map>

#include "singleton.h"

namespace Engine {

/** Handle to a particular action */
typedef int ACTION_CODE;

/** Defines an invalid/nill action code */
const ACTION_CODE INVALID_ACTION_CODE = -1;

extern ACTION_CODE KEY_MENU_UP;
extern ACTION_CODE KEY_MENU_DOWN;
extern ACTION_CODE KEY_MENU_LEFT;
extern ACTION_CODE KEY_MENU_RIGHT;
extern ACTION_CODE KEY_MENU;
extern ACTION_CODE KEY_ENTER;
extern ACTION_CODE KEY_EDITOR_WALK_FWD;
extern ACTION_CODE KEY_EDITOR_WALK_REV;
extern ACTION_CODE KEY_EDITOR_WALK_LEFT;
extern ACTION_CODE KEY_EDITOR_WALK_RIGHT;
extern ACTION_CODE KEY_EDITOR_ROTATE_UP;
extern ACTION_CODE KEY_EDITOR_ROTATE_DOWN;
extern ACTION_CODE KEY_EDITOR_ROTATE_LEFT;
extern ACTION_CODE KEY_EDITOR_ROTATE_RIGHT;
extern ACTION_CODE KEY_SCREENSHOT;
extern ACTION_CODE KEY_ENTER_EDITOR;
extern ACTION_CODE KEY_TEST;
extern ACTION_CODE KEY_TOGGLE_DEBUG_DATA;
extern ACTION_CODE KEY_TOGGLE_FPS;

/** A class to parse the joystick state as a series of boolean values, like a keyboard */
class JoyDir
{
public:
	/** Index of the device, ie the hat or axis or button */
	int DeviceIdx;

	/** Type of device */
	Uint8 Type;

	/** Focused Direction for hats, signed threshold for axis */
	Sint16 Direction;

	/** The joystick in question */
	SDL_Joystick* joystick;

public:
	/**
	Initializes the joystick action
	@param joystick The joystick in question
	@param devType Device type
	@param idx Button/Axis index
	@param thresh Threshold value for action
	*/
	JoyDir(SDL_Joystick* joystick, Uint8 devType, int idx, Sint16 thresh);
};

/** Creates a binding between defined actions and user inputs */
class Controller : public Singleton<Controller>
{
private:
	/** action code -> action name */
	map<ACTION_CODE, string> actionNames;

	/** action code -> set of key names */
	multimap<ACTION_CODE, string> bindings;

	/** key name -> keyboard key */
	map<string, SDLKey> keymap;

	/** key name -> joystick/gamepad action */
	map<string, JoyDir*> joymap;

	/** Supports up to four joystick/gamepads */
	SDL_Joystick* joystick[4];

	/** Deadzone on the axii */
	Sint16 threshold;

	/** Next action code to use when creating actions */
	static ACTION_CODE nextNewActionCode;

private:
	/** Maps all key names with the SDL key code */
	void buildKeymap();

	/** maps all action codes to the action name */
	void buildActionNames();

	/**
	Sets up variables based upon SDL joystick query.
	@param joystick number, or use -1 to indicate the keyboard
	*/
	void setupControllers();

	void buildJoymap();

	/**
	Determines if a joystick / gamepad action has occurred
	@param subfunct Joystick / Gamepad action
	@return true if the joystick action has occurred
	*/
	bool hasJoyEventOccured(JoyDir *subfunct);

public:
	/** Load key bindings */
	Controller(void);

	/** Destructor */
	~Controller(void);

	/**
	Determines whether any keyboard key or any joystick button has been pressed
	@return true if a key was depressed, false otherwise
	*/
	bool isAnyKeyDown(void);

	/**
	Determines whether a key has been depressed.  If so, the value of the key
	is placed in 'key' and the function returns true.
	Completely ignores joysticks and gamepads, focusing exclusively on the keyboard
	@param key The value of the depressed key as a virtual key code, else it will remain unmodified
	@param shift If the shift key was depressed, then this is set to false
	@return true if a key was depressed, false otherwise
	*/
	bool getKey(size_t &key, bool &shift);

	/**
	Determines whether the key is down
	@param key Action code
	@return true if the corresponding key is down
	*/
	bool isKeyDown(const ACTION_CODE key);

	/**
	Gets a list of the keys bound to the action.
	If no keys are bound, returns a list containing the single string, "[NO KEY BOUND]"
	@param actionCode the given action
	@return list of the keys bound to the action
	*/
	vector<string> getKeyName(ACTION_CODE actionCode);

	/**
	Loads key bindings from file
	@param filename The file to load key binding from
	@return true if successful
	*/
	bool load(const string &filename);

	/**
	Saves key bindings to file
	@param filename The file to save key binding to
	@return true if successful
	*/
	bool save(const string &filename);

	/** Set up the default key bindings */
	void setDefaults(void);

	/**
	Deletes an action and all asociated bindings
	@param action the action code to delete
	*/
	void deleteAction(ACTION_CODE action);

	/**
	Creates a new action with no binding
	@param name the name of the action
	@return the action code of the new action or of the one with that name that already existed
	*/
	ACTION_CODE createAction(const string &name);

	/**
	Adds a binding to an action
	@param action the action to modify
	@param binding the name of the binding
	*/
	void addBinding(ACTION_CODE action, const string &binding);
};



} // namespace Engine

#define g_Keys (::Engine::Controller::GetSingleton())

#endif
