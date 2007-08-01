/*
Author: Andrew Fox
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

#include "engine/gl.h"
#include "engine/LinearInterpolator.h"
#include "engine/CallbackTask.h"

#include "Spell.h"
#include "SpellFireBall.h"
#include "SpellIncinerate.h"
#include "SpellHeal.h"
#include "Monster.h"

#include "MyPlayer.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(MyPlayer, "class Arbarlith2::MyPlayer")

MyPlayer::MyPlayer(OBJECT_ID ID)
:Engine::Player(ID)
{}

void MyPlayer::destroy(void)
{
	vector<Spell*>::iterator iter=spellList.begin();
	while(!spellList.empty())
	{
		delete(*iter);
		iter = spellList.erase(iter);
	}

	Player::destroy();
}

void MyPlayer::clear(void)
{
	Player::clear();

	spellList.clear();
	activeIdx=0;
	spellCastDebounce = false;

	KEY_PLAYER_CAST_SPELL = INVALID_ACTION_CODE;
}

void MyPlayer::deleteControllerActions(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber < 3, "playerNumber is invalid: " + Engine::itoa(playerNumber));

	Player::deleteControllerActions(playerNumber);

	g_Keys.deleteAction(KEY_PLAYER_CAST_SPELL);

	KEY_PLAYER_CAST_SPELL = INVALID_ACTION_CODE;
}

void MyPlayer::setupControllerActions(int playerNumber)
{
	Player::setupControllerActions(playerNumber);

	string num = Engine::itoa(playerNumber+1);
	KEY_PLAYER_CAST_SPELL = g_Keys.createAction("Player" + num + "-Cast");
}

void MyPlayer::setupControllerBindings(int playerNumber)
{
	Player::setupControllerBindings(playerNumber);

#if _PLAYER_ONE_HAS_NO_JOYSTICK_
	if(playerNumber>0) // player 2 is on joystick 1, and so forth
	{
		string num = Engine::itoa(playerNumber);
		g_Keys.addBinding(KEY_PLAYER_CAST_SPELL, "JOY" + num + "_BUTT_3");
	}
#else
	string num = Engine::itoa(playerNumber+1);
	g_Keys.addBinding(KEY_PLAYER_CAST_SPELL, "JOY" + num + "_BUTT_3");
#endif

	if(playerNumber==0) // add keyboard support for player #1
	{
		g_Keys.addBinding(KEY_PLAYER_CAST_SPELL, "L_Shift");
	}
}

void MyPlayer::load(const PropertyBag &xml)
{
	Player::load(xml);

	spellList.push_back( new SpellFireBall   (&getZone(), m_ID, "data/spells/fireball.xml")    ); // 0 - Fireball
	spellList.push_back( new SpellFireBall   (&getZone(), m_ID, "data/spells/arctic-wind.xml") ); // 1 - Arctic Wind
	spellList.push_back( new SpellIncinerate (&getZone(), m_ID, "data/spells/incinerate.xml")  ); // 2 - Incinerate
	spellList.push_back( new SpellFireBall   (&getZone(), m_ID, "data/spells/chill.xml")       ); // 3 - Chill
	spellList.push_back( new SpellHeal       (&getZone(), m_ID, "data/spells/heal.xml")        ); // 4 - Heal
	spellList.push_back( new SpellFireBall   (&getZone(), m_ID, "data/spells/ice-blast.xml")   ); // 5 - Ice Blast

	this->activeIdx = 5;
}

void MyPlayer::update(float deltaTime)
{
	if(zombie) return;

	// Has the player tried to cast a spell?
	if(isAlive() && getCanCast(getActiveSpell()) && g_Keys.isKeyDown(KEY_PLAYER_CAST_SPELL))
	{
		if(!spellCastDebounce)
		{
			spellCastDebounce = true;

			if(getSpell().beginCast())
			{
				// scale the animation speed for the duration of the actual spell cast
				size_t animHandle = getAnimationHandle(getSpellCastAnim());
				float delay = getAnimationLength(animHandle);
				ChangeAnimation(animHandle, delay / getSpell().castTime);
			}
		}
	}
	else
	{
		spellCastDebounce = false;
	}

	// Process spells
	for(size_t i = 0; i < spellList.size(); ++i)
	{
		Spell *spell = spellList[i];

		ASSERT(spell!=0, "Spell was NULL while processing spells");

		spell->update(deltaTime);
	}

	if(g_Keys.isKeyDown(KEY_TEST))
	{
		getZone().getPlayer(0).kill();
	}

	Player::update(deltaTime);
}

bool MyPlayer::doUseAction(void)
{
	// Give the super class a chance to see if there is anything to do
	if(Player::doUseAction())
	{
		return true;
	}

	ActorSet closestMonsters = getZone().getObjects().typeFilter<Monster>();
	// ActorSet closestMonsters = getZone().getObjects().typeFilter<Creature>().exclude(m_ID); // allows you to attack friendlies

	for(ActorSet::iterator i = closestMonsters.begin(); i != closestMonsters.end(); ++i)
	{
		Creature &o = dynamic_cast<Creature&>(*i->second);

		float distance = getDistance(this, o);
		float maxAttackDistance = (getCylinderRadius() + o.getCylinderRadius()) * 1.1f;

		if(!o.zombie && o.isAlive() && distance<maxAttackDistance)
		{
			// Once the ATTACK animation has completed, perform the action
			AnimationController *mdl = getModel();
			if(mdl==0)
			{
				// Immediately attack the monster
				startAttackAction(i->first);
			}
			else
			{
				size_t animHandle = getAnimationHandle(getAttackAnim());
				float delay = getAnimationLength(animHandle);
				ChangeAnimation(animHandle);

				boost::function<void (void)> fn = boost::bind(&MyPlayer::startAttackAction, this, i->first);
				Task *task = Engine::makeCallbackTask(delay*.075f, fn);
				g_Application.addTask(task);
			}

			// Signal that SOME action was taken
			return true;
		}
	}

	// No monsters around?  Well then, let's just swing randomly
	ChangeAnimation(getAttackAnim());
	g_SoundSystem.play("data/sound/player/p_sword_miss.wav");
	return true;
}

bool MyPlayer::canMove(void) const
{
	ASSERT(activeIdx >= 0 && (size_t)activeIdx < spellList.size(), "Spell index was out of bounds");

	bool movable = Player::canMove();
	bool stillCasting = spellList[activeIdx]->isCasting();

	return movable && !stillCasting;
}

void MyPlayer::drawObject(void) const
{
	Player::drawObject();

	// Draw the HP and charge bar unless the Creature class is going to do that
	if(isAlive() && !g_Application.displayDebugData)
	{
		drawHealthBar();
		drawChargeBar();
	}

	if(state == GHOST)
	{
		drawStateCountdown();
	}

	// Draw spell effects
	for(vector<Spell*>::const_iterator iter = spellList.begin(); iter != spellList.end(); ++iter)
	{
		(*iter)->draw(getPos() + vec3(0, this->m_desiredHeight, 0));
	}
}

const string MyPlayer::getSpellCastAnim(void) const
{
	return string("cast") + getItemName();
}

void MyPlayer::DoCollisionResponse(void)
{
	if(hasMoved && getDistanceFromAveragePosition() > 6)
	{
		RevertToValidatedPos();
	}
	else
	{
		Player::DoCollisionResponse();
	}
}

void MyPlayer::OnChangePlayerNumber(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber <= 3, "playerNumber is invalid: " + Engine::itoa(playerNumber));

	Player::OnChangePlayerNumber(playerNumber);

	string playerModel[] =
	{
		"data/hero/model.md3xml",
		"data/hero-red/model.md3xml",
		"data/hero-yellow/model.md3xml",
		"data/hero-blue/model.md3xml",

	};

	const string &fileName = playerModel[playerNumber];

	LoadModel(fileName);
}

void MyPlayer::drawObjectDebugData(void) const
{
	const vec3 base = getPos() + vec3(0, getHeight(), 0);

	ASSERT(activeIdx >= 0 && (size_t)activeIdx < spellList.size(), "Spell index was out of bounds");
	const Spell &spell = *spellList[activeIdx];

	glPushMatrix();
		glTranslatef(base.x, base.y + 0.25f, base.z);
		g_Application.fontSmall.write(spell.toString(), white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(getName(), white, FONT_SIZE_NORMAL, true);

		glTranslatef(0.0f, 0.2f, 0.0f);
		g_Application.fontSmall.write(getTypeString(), white, FONT_SIZE_NORMAL, true);
	glPopMatrix();
}

void MyPlayer::drawStateCountdown(void) const
{
	const vec3 base = getPos() + vec3(0, getHeight(), 0);

	if(stateTimer >= 0.0f)
	{
		glPushMatrix();
			glTranslatef(base.x, base.y + 0.25f, base.z);
			g_Application.fontLarge.write(itoa( (int)ceilf(stateTimer / 1000.0f) ), green, FONT_SIZE_HUGE, true);
		glPopMatrix();
	}
}

} // namespace Arbarlith2
