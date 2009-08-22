/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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

#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "world.h"
#include "Trigger.h"
#include "TriggerPrompt.h"

namespace Engine {

/** Abstract class for a Trigger that is activated through player input */
class Switch : public Trigger
{
public:
	GEN_RTTI(Switch, "class Engine::Switch")

public:
	/**
	Constructs the Switch
	@param ID The desired unique ID of the object
	*/
	Switch(OBJECT_ID ID);

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
	Gets the action label string
	@return The action label
	*/
	const string& getActionLabel(void) const
	{
		return actionLabel;
	}

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Activate the switch
	@param a The actor that uses the switch
	*/
	void activate(Actor *a);

protected:
	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	virtual bool pollConditions(void) const;

	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/**
	Called when the switch is triggered and actually used
	@param a The actor that uses the switch
	*/
	virtual void onUse(Actor *a);

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** Text that describes the Player's action on the switch "Use", "Activate", "Ring the bell" */
	string actionLabel;

	/** Milliseconds until the option to use the switch expires */
	float fadeTimer;

	/** The time to give till the option fades */
	float time;

	/** Handle to the message we set up on the prompt */
	TriggerPrompt::HANDLE promptHandle;
};

}; // namespace

#endif
