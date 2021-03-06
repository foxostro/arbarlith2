/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _MENU_WORLD_SELECTION_H_
#define _MENU_WORLD_SELECTION_H_

#include "Menu.h"


namespace Engine {


/** Does the game's world selection screen */
class MenuWorldSelection : public Menu
{
public:
	/** Constructor */
	MenuWorldSelection(void);

	/** Destructor */
	~MenuWorldSelection(void);

	/** Create and allocate resources for the menus */
	void create(void);

protected:
	/** Populates the menu elements */
	virtual void populateElements(void);

	/**
	Activates an element of the menu
	@param selectedIndex The selected element to activate
	*/
	virtual void activateElement(int selectedIndex);
private:

	/**
	Enters a game world, if allowable
	@param worldNum The world that we are to enter
	*/
	void enterWorld(int worldNum);

	/**
	Creates a menu element for the given world
	@param worldNum The world that the element refers to.  Starts counting at 1 with world #1, not zero
	@return Menu element
	*/
	Menu::Element getLabel(int worldNum);

	/** Enumerates the menu elements */
	enum MENU_WORLD_SELECTION
	{
	MENU_WORLD_SELECTION_WORLD1=0,
	MENU_WORLD_SELECTION_WORLD2,
	MENU_WORLD_SELECTION_WORLD3,
	MENU_WORLD_SELECTION_RETURN,
	MENU_WORLD_SELECTION_MAX
	};
};

} // namespace Engine

#endif
