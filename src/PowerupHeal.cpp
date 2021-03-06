/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009,2010 Game Creation Society
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
#include "engine/world.h"
#include "engine/CallbackTask.h"
#include "engine/GameStateRun.h"
#include "engine/ListPaneWidget.h"
#include "MyPlayer.h"
#include "PowerupHeal.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(PowerupHeal, "class Arbarlith2::PowerupHeal")

PowerupHeal::PowerupHeal(OBJECT_ID ID)
:Powerup(ID)
{}

void PowerupHeal::clear(void)
{
	Powerup::clear();

	healValue = 30;
	healTime = 2000.0f;
}

void PowerupHeal::onPickup(MyPlayer &player)
{
	player.heal(healValue); // TODO: healTime does nothing
}

void PowerupHeal::load(const PropertyBag &xml)
{
	Powerup::load(xml);

	xml.get("healValue", healValue);
	xml.get("healTime", healTime);
}

bool PowerupHeal::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, "healValue", healValue);
	saveTag(xml, dataFile, "healTime", healTime);

	return Powerup::saveTidy(xml, dataFile);
}

void PowerupHeal::createToolBar(ListPaneWidget *pane)
{
	Powerup::createToolBar(pane);

	// Add to the tool bar
	pane->addElement(new ListElementTweaker<float>	("healTime",	&healTime));
	pane->addElement(new ListElementTweaker<int>	("healValue",	&healValue));
}

} // namespace
