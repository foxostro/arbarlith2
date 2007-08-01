/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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

#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "object.h"
#include "world.h"

namespace Engine {

/** Abstract class for a game object that can be triggered by the Player */
class Trigger : public Actor
{
public:
	GEN_RTTI(Trigger, "class Engine::Trigger")

public:
	/**
	Constructs the Trigger
	@param ID The desired unique ID of the object
	*/
	Trigger(OBJECT_ID ID);

	/** Destroy and clear out the object */
	virtual void destroy(void);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Creates the editor toolbar for the Creature
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

protected:
	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	virtual bool pollConditions(void) const;

	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/** Called every frame the trigger doesn's activate */
	virtual void onDoesntTrigger(void) {}

	/**
	Determines whether a player is in proximity to the trigger
	@param player If playerIsInProximity returns true, this returns the ID of the player
	@return true if a player was in close proximity
	*/
	bool playerIsInProximity(OBJECT_ID &player) const;

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** Gets the sound effect file to play when the Trigger activates */
	string getTriggerSfx(void) const;

	/**
	Sounds to play when the Portal is entered.
	One is chosen at random
	*/
	vector<string> sounds;

	/** Radius of the trigger */
	float triggerRadius;
};

} // namespace Engine

#endif
