/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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

#ifndef _DETECTOR_H_
#define _DETECTOR_H_

#include "Trigger.h"

namespace Engine {

/** Sends a signal when conditions are met */
class Detector : public Trigger
{
public:
	GEN_RTTI(Detector, "class Engine::Detector")

public:
	/**
	Constructs the Detector
	@param ID The desired unique ID of the object
	*/
	Detector(OBJECT_ID ID);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

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
	virtual void onDoesntTrigger(void);

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
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

private:
	/**
	Prevent the detector from raising the signal except at the very
	moment that the detector conditions begin to occur
	*/
	bool disallowBounce;

	/** debounce for the conditions (ignored when disallowDebounce is true) */
	bool debounce;

	/** Index of the signal to send on condition satisfaction, or -1 to indicate none */
	int signalSuccess;

	/** Index of the signal to send on condition failure, or -1 to indicate none */
	int signalFail;

	/** only allow the detector to be tripped once */
	bool onlyTripOnce;

	/** already triggered once (not saved in a save game!) */
	bool triggeredOnce;
};

} // namespace Engine

#endif
