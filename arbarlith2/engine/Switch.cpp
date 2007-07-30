/*
Copyright © 2006-2007 Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
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
	actionLabel = _T("flip the switch."); // default prompt

	Trigger::clear();
}

void Switch::load(const PropertyBag &xml)
{
	Trigger::load(xml);

	xml.getSym(actionLabel);
	xml.getSym(time);
}

bool Switch::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, _T("actionLabel"), actionLabel);
	saveTag(xml, dataFile, _T("time"), time);

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

			_tstring useKeyName = g_Keys.getKeyName(player.KEY_PLAYER_USE)[0];
			_tstring message = _T("Press ") + useKeyName + _T(" to ") + actionLabel;

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
