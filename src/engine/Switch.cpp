/*
Copyright (c) 2006,2007,2009,2010 Game Creation Society
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
#include "world.h"
#include "player.h"
#include "CallbackTask.h"
#include "GameStateRun.h"
#include "Switch.h"

namespace Engine {



GEN_ACTOR_RTTI_CPP(Switch, "class Engine::Switch")



Switch::Switch(OBJECT_ID ID)
: Trigger(ID)
{}

void Switch::destroy(void)
{
	Trigger::destroy();
}

void Switch::clear(void)
{
	promptHandle = -1;
	fadeTimer = time = 1000.0f;
	actionLabel = "flip the switch."; // default prompt

	Trigger::clear();
}

void Switch::load(const PropertyBag &xml)
{
	Trigger::load(xml);

	xml.get("actionLabel", actionLabel);
	xml.get("time", time);
}

bool Switch::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, "actionLabel", actionLabel);
	saveTag(xml, dataFile, "time", time);

	return Trigger::saveTidy(xml, dataFile);
}

void Switch::onTrigger(void)
{}

void Switch::activate(Actor *a)
{
	onUse(a);
}

void Switch::onUse(Actor *)
{
	TriggerPrompt &prompt = GameStateRun::GetSingleton().getPrompt();
	prompt.remove(promptHandle);

	Trigger::onTrigger();
}

bool Switch::pollConditions(void) const
{
	return false;
}

void Switch::update(float deltaTime)
{
	TriggerPrompt &prompt = GameStateRun::GetSingleton().getPrompt();

	// Count down
	fadeTimer -= deltaTime;

	// has a player gotten close enough to gain the Switch's attention?
	OBJECT_ID user = INVALID_ID;
	if(playerIsInProximity(user))
	{
		fadeTimer = time; // reset the countdown as long as the player is in proximity

		if(promptHandle == -1)
		{
			const Player &player = dynamic_cast<const Player &>(getZone().getObjects().get(user));

			string useKeyName = g_Keys.getKeyName(player.KEY_PLAYER_USE)[0];
			string message = "Press " + useKeyName + " to " + actionLabel;

			promptHandle = prompt.add(message, fadeTimer);
		}
		else
		{
			prompt.renew(promptHandle);
		}
	}

	// If the count down has finished
	if(fadeTimer<0)
	{
		fadeTimer=0;
		prompt.remove(promptHandle);
	}

	Trigger::update(deltaTime);
}


}; // namespace
