/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2003-2007 Game Creation Society
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

#include "stdafx.h" // Master Header
#include "gl.h"

#include "Player.h"
#include "World.h"
#include "Light.h"
#include "ListElementTweaker.h"
#include "Item.h"
#include "MenuGameOver.h"
#include "WaitScreen.h"
#include "CallbackInterpolator.h"
#include "Dimmer.h"
#include "Switch.h"
#include "GameStateMenu.h"


namespace Engine {





GEN_ACTOR_RTTI_CPP(Player, "class Engine::Player")



Player::Player(OBJECT_ID ID)
:Creature(ID)
{
	clear();
}

void Player::deleteController(void)
{
	if(playerNumber >= 0 && playerNumber < 3)
	{
		deleteControllerActions(playerNumber);
	}
}

void Player::deleteControllerActions(int)
{
	g_Keys.deleteAction(KEY_PLAYER_WALK_FWD);
	g_Keys.deleteAction(KEY_PLAYER_WALK_REV);
	g_Keys.deleteAction(KEY_PLAYER_WALK_LEFT);
	g_Keys.deleteAction(KEY_PLAYER_WALK_RIGHT);
	g_Keys.deleteAction(KEY_PLAYER_USE);

	KEY_PLAYER_WALK_FWD = INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_REV = INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_LEFT = INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_RIGHT = INVALID_ACTION_CODE;
	KEY_PLAYER_USE = INVALID_ACTION_CODE;
}

void Player::setupController(void)
{
	ASSERT(playerNumber >= 0 && playerNumber < 3, _T("playerNumber is invalid: ") + Engine::itoa(playerNumber));

	setupControllerActions(playerNumber);
	setupControllerBindings(playerNumber);
}

void Player::setupControllerActions(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber < 3, _T("playerNumber is invalid: ") + Engine::itoa(playerNumber));

	_tstring num = Engine::itoa(playerNumber+1);

	KEY_PLAYER_WALK_FWD	= g_Keys.createAction(_T("Player") + num + _T("-Walk-Fwd"));
	KEY_PLAYER_WALK_REV	= g_Keys.createAction(_T("Player") + num + _T("-Walk-Rev"));
	KEY_PLAYER_WALK_LEFT	= g_Keys.createAction(_T("Player") + num + _T("-Walk-Left"));
	KEY_PLAYER_WALK_RIGHT	= g_Keys.createAction(_T("Player") + num + _T("-Walk-Right"));
	KEY_PLAYER_USE		= g_Keys.createAction(_T("Player") + num + _T("-Use"));
}

void Player::setupControllerBindings(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber < 3, _T("playerNumber is invalid: ") + Engine::itoa(playerNumber));

#if _PLAYER_ONE_HAS_NO_JOYSTICK_
	if(playerNumber>0) // player 2 is on joystick 1, and so forth
	{
		_tstring num = Engine::itoa(playerNumber);
		g_Keys.addBinding(KEY_PLAYER_WALK_FWD,	_T("JOY") + num + _T("_AXIS_Y-"));
		g_Keys.addBinding(KEY_PLAYER_WALK_REV,	_T("JOY") + num + _T("_AXIS_Y+"));
		g_Keys.addBinding(KEY_PLAYER_WALK_LEFT,	_T("JOY") + num + _T("_AXIS_X-"));
		g_Keys.addBinding(KEY_PLAYER_WALK_RIGHT,	_T("JOY") + num + _T("_AXIS_X+"));
		g_Keys.addBinding(KEY_PLAYER_USE,		_T("JOY") + num + _T("_BUTT_2"));
	}
#else
	_tstring num = Engine::itoa(playerNumber+1);
	g_Keys.addBinding(KEY_PLAYER_WALK_FWD,	_T("JOY") + num + _T("_AXIS_Y-"));
	g_Keys.addBinding(KEY_PLAYER_WALK_REV,	_T("JOY") + num + _T("_AXIS_Y+"));
	g_Keys.addBinding(KEY_PLAYER_WALK_LEFT,	_T("JOY") + num + _T("_AXIS_X-"));
	g_Keys.addBinding(KEY_PLAYER_WALK_RIGHT,	_T("JOY") + num + _T("_AXIS_X+"));
	g_Keys.addBinding(KEY_PLAYER_USE,		_T("JOY") + num + _T("_BUTT_2"));
#endif

	if(playerNumber==0) // add keyboard support for player #1
	{
		g_Keys.addBinding(KEY_PLAYER_WALK_FWD,	_T("Up"));
		g_Keys.addBinding(KEY_PLAYER_WALK_REV,	_T("Down"));
		g_Keys.addBinding(KEY_PLAYER_WALK_LEFT,	_T("Left"));
		g_Keys.addBinding(KEY_PLAYER_WALK_RIGHT,	_T("Right"));
		g_Keys.addBinding(KEY_PLAYER_USE,		_T("Space"));
	}

	const _tstring &userKeysFile = pathAppend(getAppDataDirectory(), _T("keys.xml"));
	g_Keys.save(userKeysFile);
}

void Player::enterGameOverScreen(void)
{
	g_Application.changeGameState(GAME_STATE_MENU);
	GameStateMenu::GetSingleton().enterGameOverScreen();
}

void Player::OnDeath(void)
{
	Creature::OnDeath();

	if(allPlayersAreDead(getZone()))
	{
		// Begin to fade the screen out
		boost::function<void (void)> fn = boost::bind(&Player::enterGameOverScreen, this);
		g_Application.addTask(  makeCallbackInterpolator(&Dimmer::alphaBlur, 0.0f, 1.0f, 2000.0f, fn)  );
	}
}

bool Player::allPlayersAreDead(World &zone)
{
	const size_t numOfPlayers = zone.getNumOfPlayers();

	for(size_t i=0; i<numOfPlayers; ++i)
	{
		const Player &player = zone.getPlayer(i);

		if(player.state != DYING && player.state != DEAD && player.state != GHOST)
		{
			return false;
		}
	}

	return true;
}

void Player::load(const PropertyBag &xml)
{
	Creature::load(xml);

	// Load the inventory
	PropertyBag invBag;
	if(xml.get(_T("inventory"), invBag))
	{
		inventory.load(invBag, &getZone());
	}
	selectedItem = INVALID_ID;

	// Create the player glow light
	Light *light = new Light;
	light->create();
	light->constantAttenuation = 0;
	light->linearAttenuation = 0;
	light->quadraticAttenuation = 0.3f;
	light->causesShadows = false;
	playerGlow = getZone().getLightManager().addLight(light);
}

bool Player::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	xml.add(_T("inventory"), inventory.save()); // Save the inventory, the default value doesn't matter
	return Creature::saveTidy(xml, dataFile);
}

void Player::update(float deltaTime)
{
	if(!inventory.isMember(selectedItem))
		selectedItem = INVALID_ID;

	// Has the player tried to perform a USE action?
	if(isAlive() && canMove() && g_Keys.isKeyDown(KEY_PLAYER_USE))
	{
		if(useKeyDebounce == false)
		{
			useKeyDebounce = doUseAction();
		}
	}
	else
	{
		useKeyDebounce = false;
	}

	directControl();

	// update the creature
	Creature::update(deltaTime);

	zombie = false; // do not allow players to be deleted when they die

	// Instead of marking the player for deletion when he is dead, turn him into a ghost
	if(state == DEAD && stateTimer <= 0.0f)
	{
		state = GHOST;

		// 30 seconds as a ghost and then resurrection
		stateTimer = 30000.0f;

		/*
		normally set to false on death
		Ghosts can come back to life, so we can't let them get into a colliding state
		*/
		solid = true;
	}

	// Sync the glow light
	getZone().getLightManager().getLight(playerGlow).setPosition(getPos() + vec3(0, getHeight() * 1.2f, 0));
}

bool Player::doUseAction(void)
{
	ActorSet closestSwitches = getZone().getObjects().typeFilter<Switch>();

	for(ActorSet::iterator i = closestSwitches.begin(); i != closestSwitches.end(); ++i)
	{
		Switch &o = dynamic_cast<Switch&>(*i->second);

		float distance = getDistance(this, o);
		float maxUseDistance = (getCylinderRadius() + o.getCylinderRadius()) * 1.1f;

		if(!o.zombie && distance<maxUseDistance)
		{
			// Perform the USE action immediately
			o.activate(this);

			// Signal that SOME action was taken
			return true;
		}
	}

	// Signal that NO action was taken
	return false;
}

void Player::directControl(void)
{
	if(canMove())
	{
		vec3 direction;
		vec3 z = g_Camera.getOrientation().getAxisZ();
		vec3 x = g_Camera.getOrientation().getAxisX();

		bool w = g_Keys.isKeyDown(KEY_PLAYER_WALK_FWD);
		bool s = g_Keys.isKeyDown(KEY_PLAYER_WALK_REV);
		bool a = g_Keys.isKeyDown(KEY_PLAYER_WALK_LEFT);
		bool d = g_Keys.isKeyDown(KEY_PLAYER_WALK_RIGHT);

		bool moveKey = w || s || a || d;

		     if(w && a) direction =  z + x;
		else if(w && d) direction =  z - x;
		else if(s && a) direction = -z + x;
		else if(s && d) direction = -z - x;
		else if(w)      direction =  z;
		else if(s)      direction = -z;
		else if(a)      direction =  x;
		else if(d)      direction = -x;

		if(moveKey)
		{
			direction.y = 0.0f; // flatten
			walkTowards(getPos() - direction, 1.0f);
		}
	}
}

void Player::clear(void)
{
	Creature::clear();

	playerGlow=INVALID_LIGHT;
	selectedItem = 0;
	useKeyDebounce = false;
	playerNumber = -1;

	KEY_PLAYER_WALK_FWD	= INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_REV	= INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_LEFT	= INVALID_ACTION_CODE;
	KEY_PLAYER_WALK_RIGHT	= INVALID_ACTION_CODE;
	KEY_PLAYER_USE		= INVALID_ACTION_CODE;
}

void Player::setPlayerNumber(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber < 3, _T("playerNumber is invalid: ") + Engine::itoa(playerNumber));

	deleteController();

	this->playerNumber = playerNumber;

	setupController();

	OnChangePlayerNumber(playerNumber);
}

#ifdef _DEBUG
void Player::OnChangePlayerNumber(int playerNumber)
{
	ASSERT(playerNumber >= 0 && playerNumber <= 3, _T("playerNumber is invalid: ") + Engine::itoa(playerNumber));
	// Do nothing
}
#else
void Player::OnChangePlayerNumber(int) {}  // leaves no unused parameters
#endif

void Player::pickupItem(Item *item)
{
	ASSERT(item!=0, _T("item was null"));

	// Add it to the inventory
	inventory.insert(make_pair(item->m_ID, item));

	// Select it
	selectSpecificItem(item->m_ID);
}

bool Player::isInventoryItem(OBJECT_ID id) const
{
	return inventory.isMember(id);
}

bool Player::inventoryQuery(const _tstring &name, OBJECT_ID &out) const
{
	return inventory.query(name, out);
}

void Player::discardItem(OBJECT_ID id)
{
	inventory.erase(inventory.find(id));

	if(inventory.empty())
	{
		selectSpecificItem(INVALID_ID);
	}
	else
	{
		ActorSet::iterator iter = inventory.begin();
		OBJECT_ID id = iter->first;
		selectSpecificItem(id);
	}
}

void Player::cycleItemNext(void)
{
	if(!inventory.empty() && inventory.size()>1)
	{
		for(ActorSet::iterator iter=inventory.begin(); iter!=inventory.end(); ++iter)
		{
			OBJECT_ID id = static_cast<Actor*>(iter->second)->m_ID;

			if(selectedItem == id)
			{
				++iter;

				if(iter == inventory.end())
				{
					iter = inventory.begin();
				}

				selectedItem = static_cast<Actor*>(iter->second)->m_ID;

				selectSpecificItem(selectedItem);
				return;
			}
		}
	}
}

void Player::selectSpecificItem(OBJECT_ID id)
{
	if(id != INVALID_ID)
	{
		if(inventory.isMember(id))
		{
			selectedItem = id;

			// Play an item select noise
			g_SoundSystem.play(_T("data/sound/click.wav"));
		}
	}
}

const _tstring Player::getAttackAnim(void) const
{
	return _tstring(_T("attack")) + getItemName();
}

const _tstring Player::getIdleAnim(void) const
{
	return _tstring(_T("idle")) + getItemName();
}

const _tstring Player::getRunFwdAnim(void) const
{
	return _tstring(_T("run")) + getItemName();
}

const _tstring Player::getRunRevAnim(void) const
{
	return _tstring(_T("runback")) + getItemName();;
}

const _tstring Player::getRunLeftAnim(void) const
{
	return _tstring(_T("runleft")) + getItemName();
}

const _tstring Player::getRunRightAnim(void) const
{
	return _tstring(_T("runright")) + getItemName();
}

const _tstring Player::getRunLeftFwdAnim(void) const
{
	return _tstring(_T("runleftforward")) + getItemName();
}

const _tstring Player::getRunRightFwdAnim(void) const
{
	return _tstring(_T("runrightforward")) + getItemName();
}

const _tstring Player::getRunLeftRevAnim(void) const
{
	return _tstring(_T("runleftback")) + getItemName();
}

const _tstring Player::getRunRightRevAnim(void) const
{
	return _tstring(_T("runrightback")) + getItemName();
}

_tstring Player::getItemName(void) const
{
	if(!inventory.empty() && selectedItem != INVALID_ID && inventory.isMember(selectedItem))
	{
		const Actor &item = inventory.get(selectedItem);

		return _tstring(_T("_")) + item.getName();
	}

	return _tstring();
}

const _tstring Player::getPainAnim(void) const
{
	return _tstring(_T("flinch")) + getItemName();
}

const _tstring Player::getDyingAnim(void) const
{
	return _tstring(_T("dying")) + getItemName();
}

const _tstring Player::getWalkAnim(float speed) const
{
	if(speed >= topSpeed*0.9)
	{
		return _T("run");
	}
	else if(speed < minWalkingVelocity)
	{
		return getIdleAnim();
	}
	else
	{
		return _T("walk");
	}
}

float Player::getDistanceFromAveragePosition(void) const
{
	const vec3 &averagePlayerPosition = getZone().getAveragePlayerPosition();
	return vec3(position.x-averagePlayerPosition.x, 0, position.z-averagePlayerPosition.z).getMagnitude();
}

list<Actor*> Player::getCollisions(const ActorSet &s) const
{
	return Actor::getCollisions(s);
}

void Player::walkTowards(const vec3 &target, float speed)
{
	ASSERT(speed >= -1.0f, _T("Speed is too negative: ") + ftoa(speed));
	ASSERT(speed <= +1.0f, _T("Speed is too positive: ") + ftoa(speed));

	if(canMove())
	{
		velocity = vec3(target-getPos()).getNormal() * getTopSpeed() * speed;
		lookAt(target);
	}
}

}; // namespace
