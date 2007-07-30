/*
Author: Andrew Fox
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

#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "engine/world.h"
#include "engine/creature.h"
#include "Spell.h"

namespace Arbarlith2 {

/** Monster creature */
class Monster : public Creature
{
public:
	GEN_RTTI(Monster, "class Arbarlith2::Monster")

public:
	/**
	Constructs the Monster
	@param ID The desired unique ID of the object
	*/
	Monster(OBJECT_ID ID);

	/** Gets the name of the proper attack animation */
	const _tstring getAttackAnim(void) const;

	/** Destroys the player instance */
	virtual void destroy(void);

	/** Clear out everything to defaults */
	virtual void clear(void);

	/**
	Updates the creature
	@param deltaTime The milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/** Gets the name of the proper idle animation */
	virtual const _tstring getSpellCastAnim(void) const;

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	/**
	Gets the Monster's preferred spell.  Returns null when the Monster cannot cast spells
	@return the Monster's preferred spell, or null
	*/
	inline Arbarlith2::Spell* getSpellPtr(void) const
	{
		return spell;
	}

	/** Called in the event that the actor slid against a wall */
	virtual void onSlidOnWall(void);

protected:
	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(PropertyBag &xml, PropertyBag &dataFile) const;

	/** The spell that this monster will use */
	Arbarlith2::Spell *spell;

	/** Name of the preferred spell */
	int preferredSpell;
};

} //namespace

#endif
