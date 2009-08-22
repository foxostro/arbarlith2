/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _TELEPORTER_H_
#define _TELEPORTER_H_

#include "engine/Switch.h"

namespace Engine {

/** Device that moves the player and party to another location when activated */
class Teleporter : public Switch
{
public:
	GEN_RTTI(Teleporter, "class Engine::Teleporter")

public:
	/**
	Constructs the Teleporter
	@param handle The desired unique ID of the object
	*/
	Teleporter(OBJECT_ID handle);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Creates the editor toolbar for the Creature
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

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

private:
	/** Destination position for the player party */
	vec3 destinationPosition;
};


} // namespace Arbarlith2

#endif
