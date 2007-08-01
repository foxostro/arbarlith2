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

#include "stdafx.h"
#include "engine/gl.h"
#include "engine/LinearInterpolator.h"
#include "engine/GameStateSpellMenu.h"
#include "Spell.h"

namespace Arbarlith2 {

Spell::Spell(void)
{
	clear();
}

Spell::~Spell(void)
{
	destroy();
}

void Spell::clear(void)
{
	ownerID = 0;
	zone = 0;

	matActive.clear();
	matInactive.clear();

	castTime = 400.0f;
	coolDownTime = 800.0f;

	state = READY;
	timer = 0;

	available = false; // must be learned first

	descriptionText = "ERROR: no description available for this spell";
}

void Spell::setZone(World *zone, OBJECT_ID ownerID)
{
	ASSERT(zone!=0, "zone is null");
	ASSERT(zone->getObjects().isMember(ownerID), "owner is not present in the active zone");

	this->zone = zone;
	this->ownerID = ownerID;
}

void Spell::loadFromFile(const string &fileName, Engine::World *world, Engine::OBJECT_ID ownerID)
{
	PropertyBag xml;
	xml.loadFromFile(fileName);
	load(xml, world, ownerID);
}

void Spell::load(PropertyBag &xml, Engine::World *world, Engine::OBJECT_ID ownerID)
{
	string activeIcon;
	string inactiveIcon;

	destroy();

	xml.getSym(descriptionText);
	xml.getSym(castTime);
	xml.getSym(coolDownTime);
	xml.getSym(activeIcon);
	xml.getSym(inactiveIcon);

	loadIcon(activeIcon, inactiveIcon);

	setZone(world, ownerID);
}

void Spell::destroy(void)
{
	matActive.destroy();
	matInactive.destroy();

	clear();
}

void Spell::loadIcon(const string &active, const string &inactive)
{
	matActive.loadTexture(active, 0);
	matInactive.loadTexture(inactive, 0);
}

void Spell::create(PropertyBag &xml)
{
	destroy();

	xml.getSym(castTime);
	xml.getSym(coolDownTime);
}

void Spell::drawIcon(bool active, const vec3 &center, float size) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_CULL_FACE);

		glEnable(GL_TEXTURE_2D);
		(active ? matActive : matInactive).bind();

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);

			const mat4 &m = g_Camera.getOrientation();
			const vec3 &right = m.getAxisX().getNormal();
			const vec3 &up = m.getAxisY().getNormal();

			const vec3 a = -right * (size * 0.5f);
			const vec3 b =  right * (size * 0.5f);
			const vec3 c =  right * (size * 0.5f) + up * size;
			const vec3 d = -right * (size * 0.5f) + up * size;

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(a.x, a.y, a.z);
				glTexCoord2f(1.0f, 0.0f);	glVertex3f(b.x, b.y, b.z);
				glTexCoord2f(1.0f, 1.0f);	glVertex3f(c.x, c.y, c.z);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f(d.x, d.y, d.z);
			glEnd();

		glPopMatrix();
	glPopAttrib();
}

void Spell::drawIcon2D(bool active, float x, float y, float size) const
{
	glEnable(GL_TEXTURE_2D);
	(active ? matActive : matInactive).bind();

	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0.0f, y + size, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0.0f, y + 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + size, y + 0.0f, 0.0f);
	glEnd();

	if(state!=READY)
	{
		drawSpell_X(x, y, size);
	}
	else if(!available)
	{
		drawSpell_Lock(x, y, size);
	}

	CHECK_GL_ERROR();
}

void Spell::drawSpell_X(float x, float y, float size) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(3);
	glColor4fv(red);

	// Draw an Red X over the spell icon
	glBegin(GL_LINES);
		glVertex3f(x + size, y + size, 0.0f);
		glVertex3f(x + 0.0f, y + 0.0f, 0.0f);
		glVertex3f(x + 0.0f, y + size, 0.0f);
		glVertex3f(x + size, y + 0.0f, 0.0f);
	glEnd();

	glPopAttrib();
}

void Spell::drawSpell_Lock(float x, float y, float size) const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	GameStateSpellMenu::GetSingleton().padlock.bind();

	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + size, y + size, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + 0.0f, y + size, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + 0.0f, y + 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + size, y + 0.0f, 0.0f);
	glEnd();

	glPopAttrib();
}

void Spell::update(float deltaTime)
{
	timer -= deltaTime;
	if(timer <= 0.0f)
	{
		timer = 0.0f;

		switch(state)
		{
		case READY: break; // no op

		case CASTING:
			timer = coolDownTime;
			state = COOLING;
			castSpell();
			break;

		case COOLING:
			state = READY;
			break;
		};
	}
}

void Spell::draw(const vec3 &center) const
{
	if(timer < 1000.0f)
	{
		const float MAX_Y = 0.3f;

		switch(state)
		{
		case COOLING:
			drawIcon(true, center + vec3(0, MAX_Y, 0), 0.6f);
			break;

		case CASTING:
			drawIcon(true, center + vec3(0, MAX_Y, 0) * (1 - (timer / castTime)), 0.6f);
			break;

		case READY:
			break; // Do nothing
		};
	}
}

bool Spell::beginCast(void)
{
	if(isReady())
	{
		timer = castTime;
		state = CASTING;
		return true;
	}

	return false;
}

void Spell::cancelCast(void)
{
	if(state == CASTING)
	{
		timer = coolDownTime;
		state = COOLING;
	}
}

void Spell::castSpell(void)
{}

string Spell::toString(void) const
{
	float seconds = timer / 1000.0f;
	string timeString = ftoa(seconds, 1);

	string stateString = "<indefinite>";
	switch(state)
	{
	case READY: stateString = "READY"; break;
	case CASTING: stateString = "CASTING"; break;
	case COOLING: stateString = "COOLING"; break;
	}

	return "Spell state: " + stateString + "  ; " + timeString;
}

} // namespace Engine
