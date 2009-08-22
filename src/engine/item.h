/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009 Game Creation Society
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

#ifndef _ITEM_H_
#define _ITEM_H_

#include "Switch.h"


namespace Engine { 



/** Abstract class for a game object that can be picked up by the player */
class Item : public Switch
{
public:
	/**
	Constructor
	@param ID The unique ID of the object
	*/
	Item(const OBJECT_ID ID);

	/**
	Called when the player equips this Item
	@param player A reference to the player that owns us
	*/
	virtual void OnEquip(Player &player);
	
	/**
	Called when the player un-equips this Item
	@param player A reference to the player that owns us
	*/
	virtual void OnUnequip(Player &player);

	/**
	Indicates that the item is an instant pickup
	@return true if the item is an instant pickup
	*/
	inline bool isInstantPickup(void) const
	{
		return false;
	}

protected:
	/**
	Called when the switch is triggered and actually used
	@param a The actor that uses the switch
	*/
	virtual void onUse(Actor *a);
};


}; // namespace


#endif
