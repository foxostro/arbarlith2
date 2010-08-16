/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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
#include "ListPaneWidget.h"
#include "ToggleWidgetText.h"
#include "ListElementTweaker.h"
#include "world.h"
#include "player.h"
#include "Trigger.h"


namespace Engine {



GEN_ACTOR_RTTI_CPP(Trigger, "class Engine::Trigger")



Trigger::Trigger(OBJECT_ID ID)
: Actor(ID)
{}

void Trigger::destroy(void)
{
	Actor::destroy();
}

void Trigger::clear(void)
{
	Actor::clear();
	sounds.clear();
	triggerRadius=2.0f;
}

void Trigger::update(float deltaTime)
{
	Actor::update(deltaTime);

	if(pollConditions())
		onTrigger();
	else
		onDoesntTrigger();
}

bool Trigger::playerIsInProximity(OBJECT_ID &player) const
{
	for(size_t i=0; i<getZone().getNumOfPlayers(); ++i)
	{
		const Actor &a = getZone().getPlayer(i);

		const ActorSet &s = getZone().getObjects();

		if(!zombie && !a.zombie && s.isMember(a.m_ID))
		{
			vec3 delta = getPos() - a.getPos();
			delta.y=0;

			// The minimum distance before a trigger event occurs
			float minDist = (triggerRadius + a.getCylinderRadius());

			// The distance separating the two cylinders
			float realDist = delta.getMagnitude() - minDist;

			// Has an intersection occurred?
			if(realDist < 0.0f)
			{
				player = a.m_ID;
				return true;
			}
		}
	}

	return false;
}

bool Trigger::pollConditions(void) const
{
	OBJECT_ID temp=0;
	return playerIsInProximity(temp);
}

void Trigger::onTrigger(void)
{
	g_SoundSystem.play(getTriggerSfx());
}

bool Trigger::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveList(xml, "sounds", sounds);
	saveTag(xml, dataFile, "triggerRadius", triggerRadius);

	return Actor::saveTidy(xml, dataFile);
}

void Trigger::load(const PropertyBag &xml)
{
	Actor::load(xml);

	loadList(xml, "sounds", sounds);
	if(!xml.get("triggerRadius", triggerRadius))
	{
		triggerRadius = getCylinderRadius(); // default
	}
}

string Trigger::getTriggerSfx(void) const
{
	string sfx;

	if(sounds.empty())
		sfx = "";
	else if(sounds.size()==1)
		sfx = sounds[0];
	else
		sfx = sounds[IRAND_RANGE(0, sounds.size()-1)];

	return sfx;
}

void Trigger::createToolBar(ListPaneWidget *pane)
{
	Actor::createToolBar(pane);
	pane->addElement(new ListElementTweaker<float>	("Trigger Radius",	&triggerRadius));
}

}; // namespace
