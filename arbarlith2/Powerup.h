/*
Original Author: Michael Collins
E-Mail: mailto:mdc1@cmu.edu

Modified by Andrew Fox in Apr-2007 to be more generic towards other types of powerups
E-Mail: mailto:andrewfox@cmu.edu


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

#ifndef _POWERUP_H_
#define _POWERUP_H_

#include "engine/Switch.h"

namespace Arbarlith2 {

class MyPlayer;

/** Basic power up object */
class Powerup : public Switch
{
public:
	GEN_RTTI(Powerup, "class Arbarlith2::Powerup")

public:
	/**
	Constructor
	@param ID The unique ID to assign to the Player
	*/
	Powerup(OBJECT_ID ID);

	/** Updates the powerup */
	virtual void update(float milliseconds);

protected:
	/**
	Called whenever the player picks up the powerup
	@param player the pointer to the player who picks up
	*/
	virtual void onPickup(MyPlayer &player);

private:
	/**
	Called when the switch is triggered and actually used
	@param a The actor that uses the switch
	*/
	void onUse(Actor *a);
};

}; // namespace

#endif
