/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright � 2007 Game Creation Society
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
#include "engine/World.h"
#include "engine/CallbackInterpolator.h"
#include "engine/CallbackTask.h"
#include "engine/ListPaneWidget.h"
#include "engine/ListElementTweaker.h"
#include "engine/ToggleWidgetText.h"
#include "TileGate.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(TileGate, "class Arbarlith2::TileGate")

TileGate::TileGate(OBJECT_ID ID)
:Actor(ID)
{}

void TileGate::clear(void)
{
	Actor::clear();

	locked = true;
	lockedHeight = 2.4f; // default
	unlockedHeight = 0.0f; // default
	lockTime = 1000.0f;
	unlockTime = 1000.0f;
	reactionDelay = 0.0f;
	initiallyLocked = true;
	unlockSfx=_T("nill");
	lockSfx=_T("nill");
}

void TileGate::onTrigger(void)
{
	if(!transitioningNow)
	{
		transitioningNow = true;
		if(locked)
		{
			unlockGate();
		}
		else
		{
			lockGate();
		}
	}
}

bool TileGate::LoadXml(CPropBag &xml)
{
	Actor::LoadXml(xml);

	xml.getSym(lockedHeight);
	xml.getSym(unlockedHeight);
	xml.getSym(lockTime);
	xml.getSym(unlockTime);
	xml.getSym(reactionDelay);
	xml.getSym(initiallyLocked);
	xml.getSym(unlockSfx);
	xml.getSym(lockSfx);

	getTile().setPassable(true); // the tile should not be instrinically impassable, but made impassable by its height

	if(initiallyLocked)
	{
		lockGateImmediately();
	}
	else
	{
		unlockGateImmediately();
	}

	return true;
}

bool TileGate::saveTidy(CPropBag &xml, CPropBag &editorData) const
{
	saveTag(xml, editorData, _T("lockedHeight"),	lockedHeight);
	saveTag(xml, editorData, _T("unlockedHeight"),	unlockedHeight);
	saveTag(xml, editorData, _T("lockTime"),		lockTime);
	saveTag(xml, editorData, _T("unlockTime"),	unlockTime);
	saveTag(xml, editorData, _T("reactionDelay"),	reactionDelay);
	saveTag(xml, editorData, _T("initiallyLocked"),	initiallyLocked);
	saveTag(xml, editorData, _T("unlockSfx"),		unlockSfx);
	saveTag(xml, editorData, _T("lockSfx"),		lockSfx);

	return Actor::saveTidy(xml, editorData);
}

void TileGate::unlockGate(void)
{
	if(reactionDelay<=FLT_EPSILON)
	{
		unlockGateImmediately();
	}
	else
	{
		boost::function<void (void)> fn = boost::bind(&TileGate::unlockGateImmediately, this);
		Task *task = makeCallbackTask(reactionDelay, fn);
		g_Application.addTask(task);
	}
}

void TileGate::lockGate(void)
{
	if(reactionDelay<=FLT_EPSILON)
	{
		lockGateImmediately();
	}
	else
	{
		boost::function<void (void)> fn = boost::bind(&TileGate::lockGateImmediately, this);
		Task *task = makeCallbackTask(reactionDelay, fn);
		g_Application.addTask(task);
	}
}

void TileGate::unlockGateImmediately(void)
{
	locked = false;

	Tile &tile = getTile();

	boost::function<void (void)> fn = boost::bind(&TileGate::onUnlocked, this);
	Task *task = makeCallbackInterpolator(tile.getModifiableTileHeight(), tile.getTileHeight(), unlockedHeight, unlockTime, fn);
	g_Application.addTask(task);

	if(unlockSfx!=_T("nill"))
		g_SoundSystem.play3D(unlockSfx, getPos());
}

void TileGate::lockGateImmediately(void)
{
	locked = true;

	Tile &tile = getTile();

	boost::function<void (void)> fn = boost::bind(&TileGate::onLocked, this);
	Task *task = makeCallbackInterpolator(tile.getModifiableTileHeight(), tile.getTileHeight(), lockedHeight, lockTime, fn);
	g_Application.addTask(task);

	if(lockSfx!=_T("nill"))
		g_SoundSystem.play3D(lockSfx, getPos());
}

void TileGate::onLocked(void)
{
	transitioningNow = false;
}

void TileGate::onUnlocked(void)
{
	transitioningNow = false;
}

Tile& TileGate::getTile(void)
{
	Zone &zone = getZone();
	Map &map = zone.getMap();
	return map.getTile(getPos().x, getPos().z);
}

void TileGate::createToolBar(ListPaneWidget *pane)
{
	Actor::createToolBar(pane);

	pane->addElement(new ListElementTweakerString	(_T("Unlocking Sfx"),		&unlockSfx));
	pane->addElement(new ListElementTweakerString	(_T("Locking Sfx"),		&lockSfx));
	pane->addElement(new ListElementTweaker<float>	(_T("Locked Height"),		&lockedHeight));
	pane->addElement(new ListElementTweaker<float>	(_T("Unlocked Height"),		&unlockedHeight));
	pane->addElement(new ListElementTweaker<float>	(_T("Locking Time"),		&lockTime));
	pane->addElement(new ListElementTweaker<float>	(_T("Unlocking Time"),		&unlockTime));
	pane->addElement(new ToggleWidgetText		(_T("Initially Locked"),	&initiallyLocked));
}

}; // namespace
