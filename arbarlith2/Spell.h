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

#ifndef _SPELL_H_
#define _SPELL_H_

#include "engine/vec4.h"
#include "engine/PropBag.h"
#include "engine/Material.h"
#include "engine/Zone.h"

namespace Arbarlith2 {

/** Magical spell that a creature can cast */
class Spell
{
public:
	/** Constructor */
	Spell(void);

	/** Destructor */
	virtual ~Spell(void);

	/** Resets data */
	virtual void clear(void);

	/** cleanly releases resources and resets */
	virtual void destroy(void);

	/**
	Loads a new spell icon
	@param active Filename of the icon when the spell is active / selected
	@param inactive Filename of the icon when the spell is inactive / not-selected
	*/
	virtual void loadIcon(const _tstring &active, const _tstring &inactive);

	/**
	Loads spell data
	@param xml XML source for the Spell
	*/
	virtual void create(Engine::CPropBag &xml);

	/** draws the spell icon orthogonal to the screen in worldspace */
	void drawIcon(bool active, const Engine::vec3 &pos, float size) const;

	/** draws the spell icon in screen space */
	void drawIcon2D(bool active, float x, float y, float size) const;

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	virtual void update(float deltaTime);

	/**
	Changes the zone in which the spell applies
	@param zone The zone in which the spell applies
	@param ownerID Handle to the owner in the new zone
	*/
	void setZone(Engine::Zone *zone, Engine::OBJECT_ID ownerID);

	/**
	Draws spell effects
	@param center position of the spell caster
	*/
	virtual void draw(const Engine::vec3 &center) const;

	/**
	Casts the spell
	@return true if the spell will be cast\
	*/
	virtual bool beginCast(void);

	/** Cancels any current spell cast action */
	virtual void cancelCast(void);

	/** Indicates that the spell is ready to be cast */
	inline bool isReady(void) const
	{
		return state == READY && available;
	}

	/** Indicates that the spell is in the cating process */
	inline bool isCasting(void) const
	{
		return state == CASTING;
	}

	/**
	Gets the text to describe the spell on the spell selection menu
	@return one line of text
	*/
	inline const _tstring& getDescriptionText(void) const
	{
		return descriptionText;
	}

	/**
	Loads the spell data from file
	@param fileName XML source
	@param zone The zone in which the spell applies
	@param ownerID Spell caster owner
	*/
	void loadFromFile(const _tstring &fileName, Engine::Zone *zone, Engine::OBJECT_ID ownerID);

	/**
	Returns diagnostic data about the spell in a string of text
	@return a string of text
	*/
	virtual _tstring toString(void) const;

public:
	/** Spell cast time */
	float castTime;

	/** Spell cooldown time */
	float coolDownTime;

	/** Indicates whether the spell is available to place in the owner's spell book */
	bool available;

protected:
	/**
	Loads the spell data from file
	@param xml XML source
	@param zone The zone in which the spell applies
	@param ownerID Spell caster owner
	*/
	virtual void load(Engine::CPropBag &xml, Engine::Zone *zone, Engine::OBJECT_ID ownerID);

	/** Actually casts the spell */
	virtual void castSpell(void);

	Engine::Material matActive;
	Engine::Material matInactive;

	float timer;

	/** possible spell state */
	enum SPELL_STATE //Will cause problems unless each player is given their own instances of spells
	{
		READY,
		CASTING,
		COOLING
	};

	/** the current spell state */
	SPELL_STATE state;

	/** Spell Owner */
	Engine::OBJECT_ID ownerID;

	/** Zone in which the spell applies */
	Engine::Zone *zone;

private:
	/** Text to describe the spell on the spell selection menu */
	_tstring descriptionText;

	/** Draws a red X over the spell icon */
	void drawSpell_X(float x, float y, float size) const;

	/** Draws a lock over the spell icon */
	void drawSpell_Lock(float x, float y, float size) const;
};

} // namespace Arbarlith2

#endif
