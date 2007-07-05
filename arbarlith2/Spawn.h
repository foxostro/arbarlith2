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

#ifndef _SPAWN_H_
#define _SPAWN_H_

#include "engine/Listener.h"

using namespace Engine;

namespace Arbarlith2 { 

/** Spawns monsters of a specific type when triggered */
class Spawn : public Listener
{
public:
	GEN_RTTI(Spawn)

public:
	/**
	Constructs the Spawn
	@param ID The desired unique ID of the object
	*/
	Spawn(OBJECT_ID ID);

	/** Clear out everything to defaults */
	void clear(void);

	/**
	Loads Player from XML
	@param xml XML source
	@return true if the XML loaded successfully
	*/
	virtual bool LoadXml(Engine::CPropBag &xml);

protected:
	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(CPropBag &xml, CPropBag &dataFile) const;

private:
	/** Data file of the monster to spawn */
	_tstring monsterDatafile;

	/** Maximum number to spawn on regular difficulty */
	int maxMonsters;

	/** Minimum number to spawn on regular difficulty */
	int minMonsters;

	/** Meters to separate spawned monsters from eachother */
	float separationDistance;

	/** already spawned once (not saved in a save game!) */
	bool triggeredOnce;
};

}; // namespace

#endif
