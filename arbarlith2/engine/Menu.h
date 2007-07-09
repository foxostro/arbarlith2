/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2005-2007 Game Creation Society
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

#ifndef _MENU_H_
#define _MENU_H_

#include "BackdropTexture.h"
#include "Application.h"

namespace Engine {

/** Does the game menu screens */
class Menu
{
public:
	/** Menu item */
	class Element
	{
	private:
		/** Position of the menu item on the menu screen */
		vec2 position;

		/** Label for the menu item */
		_tstring label;

		/** Indicates that the item is enabled */
		bool enabled;

	public:
		/**
		Constructor
		@param position Position of the menu item on the menu screen
		@param label Label for the menu item
		@param enabled Indicates that the element is enabled
		*/
		Element(const vec2 &position, const _tstring &label, bool enabled=true)
		{
			this->position = position;
			this->label = label;
			this->enabled = enabled;
		}

		/**
		Gets the position of the menu item
		@return The position of the menu item
		*/
		const vec2& getPosition(void) const
		{
			return position;
		}

		/**
		Gets the label for the menu item
		@return The label for the menu item
		*/
		const _tstring& getLabel(void) const
		{
			return label;
		}

		/**
		Determines whether the menu item is enabled
		@return true if the menu item is enabled
		*/
		bool isEnabled(void) const
		{
			return enabled;
		}

		/** Enables the menu item */
		void enable(void)
		{
			enabled = true;
		}

		/** Disables the menu item */
		void disable(void)
		{
			enabled = false;
		}
	};

	/** Constructor */
	Menu(void);

	/**
	Constructor
	@param imageFileName Image file name for the backdrop at 800x600 resolution
	*/
	Menu(const _tstring &imageFileName);

	/** Destructor */
	virtual ~Menu(void);

	/** Clear out the menu */
	virtual void clear(void);

	/** Release resources for the menu */
	virtual void destroy(void);

	/**
	Allocates all resources
	@param imageFileNameImage file name for the backdrop
	*/
	virtual void create(const _tstring &imageFileName);

	/** Draw the menus */
	virtual void draw(void);

	/** Accepts user input */
	virtual void update(void);

	/** re-populates the menu elements */
	virtual void populateElements(void);

protected:
	/**
	Activates an element of the menu
	@param The selected element to activate
	*/
	virtual void activateElement(int selectedIndex);

	/** The menu elements */
	vector<Element> elements;

	/** Debounce the enter key */
	bool debounceEnter;

private:
	/** The index of the currently selected element */
	int selectedIndex;

	/** Debounce the up key */
	bool debounceUp;

	/** Debounce the down key */
	bool debounceDown;

	/** A handle to the menu's background back drop */
	BackdropTexture menuBackdrop;

	/** The current font */
	TextWriter *font;
};

} // namespace Engine

#endif
