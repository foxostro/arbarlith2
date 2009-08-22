/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "Trigger.h"

using namespace Engine;

namespace Engine {

/** Listens for a combination of signals and triggers when they are met */
class Listener : public Trigger
{
public:
	GEN_RTTI(Listener, "class Engine::Listener")

public:
	/**
	Constructs the Listener
	@param ID The desired unique ID of the object
	*/
	Listener(OBJECT_ID ID);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

protected:
	/**
	Given an expression, create the signal conditions
	@param expression defines the signal conditions
	*/
	void setupSignals(const string &expression);

	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	bool pollConditions(void) const;

	/**
	Called on the event that a message is received by the object
	@param message The message received
	*/
	void OnMessage(Message_s message);

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/**
	Creates the editor toolbar for the Creature
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

	/**
	When the toolbar is active data values may be changed at
	any moment.  This function takes the data for the Actor
	and synchronizes the Actor's state with it.  For example,
	the appropriate model is reloaded and the scale is
	recalculated.
	*/
	virtual void sync(void);

private:
	/** Signals to listen for mapped with the milliseconds since they were detected */
	map<int, float> signals;

	/** Indicates that we trigger on any subscribed signal.  Otherwise, we require all subscribed signals before triggering */
	bool anySignal;

	/** Signal expression */
	string expression;
};

} // namespace Engine

#endif
