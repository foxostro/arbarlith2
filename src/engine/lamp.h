/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _LAMP_H_
#define _LAMP_H_

#include "Switch.h"
#include "LightManager.h"


namespace Engine {


/** Device that creates a Light and can be brken or repaired */
class Lamp : public Switch
{
public:
	GEN_RTTI(Lamp, "class Engine::Lamp")

public:
	/**
	Constructor
	@param ID The unique ID of the object
	*/
	Lamp(OBJECT_ID ID);

	/** Destroy and clear out the object */
	void destroy(void);

	/** Clear out everything to defaults */
	void clear(void);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	void update(float deltaTime);

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

protected:
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

	/** Sync data in editor mode */
	void sync(void);

	/**
	Creates the editor toolbar for the Creature
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

private:
	/** Handle to the Light used by this Lamp */
	HLIGHT light;

	/** The light is activated */
	bool active;

	/**
	Constant factor of the light attenuation.
	Sync'd only in editor mode.
	*/
	float constant;

	/**
	Linear factor of the light attenuation.
	Sync'd only in editor mode.
	*/
	float linear;

	/**
	Quadratic factor of the light attenuation.
	Sync'd only in editor mode.
	*/
	float quadratic;
};

}; //namespace

#endif
