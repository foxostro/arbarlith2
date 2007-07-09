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

#include "stdafx.h"
#include "MyPlayer.h"
#include "PowerUp.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(Powerup, "class Arbarlith2::Powerup")

Powerup::Powerup(OBJECT_ID ID)
:Switch(ID)
{}

void Powerup::update(float milliseconds)
{
	Switch::update(milliseconds);

	position.y = getZone().getMap().getTile(position.x, position.z).getTileHeight() + getSphereRadius();

	const float radiansPerSecond = (float)M_PI / 8.0f;
	float deltaAngle = radiansPerSecond * (milliseconds/1000.0f);

	mat4 rotationMatrix;
	rotationMatrix.rotateY(deltaAngle);

	mat4 orientation = getOrientation();
	orientation *= rotationMatrix;
	setOrientation(orientation);
}

void Powerup::onPickup(MyPlayer&)
{}

void Powerup::onUse(Actor *a)
{
	Switch::onUse(a);

	MyPlayer *player = dynamic_cast<MyPlayer*>(a);

	if(player)
	{
		onPickup(*player);
	}

	zombie=true; // disappear after use
}

}
