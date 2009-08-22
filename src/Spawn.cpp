/*
Author: Andrew Fox
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

#include "stdafx.h"
#include "engine/ActorSet.h"
#include "engine/world.h"
#include "engine/random.h"
#include "engine/WaitScreen.h"
#include "engine/ListElementTweaker.h"
#include "Spawn.h"

namespace Engine {
vec3 GetRandomVector(float length);
};

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(Spawn, "class Arbarlith2::Spawn")

Spawn::Spawn(OBJECT_ID ID)
:Listener(ID)
{}

void Spawn::clear(void)
{
	Listener::clear();

	triggeredOnce=false;
	maxMonsters = 3;
	minMonsters = 1;
	separationDistance = 2;
	monsterDataFile = "nill";
}

void Spawn::onTrigger(void)
{
	if(triggeredOnce) return;

	int numMonsters = (minMonsters==maxMonsters) ? minMonsters : IRAND_RANGE(minMonsters, maxMonsters);

	ActorSet &s = getZone().getObjects();

	if(numMonsters==1)
	{
		s.spawn(monsterDataFile, getPos());
	}
	else
	{
		float radius = (separationDistance*numMonsters) / (2*(float)M_PI);

		for(int i=0; i<numMonsters; ++i)
		{
			float angle = 2.0f*i*(float)M_PI/numMonsters;

			vec3 spawnPoint = vec3(cosf(angle), 0, sinf(angle))*radius + getPos();
			spawnPoint.y = 0.0f; // flatten
			s.spawn(monsterDataFile, spawnPoint);
		}
	}

	triggeredOnce = true;
}

void Spawn::load(const PropertyBag &xml)
{
	Listener::load(xml);

	xml.getSym(monsterDataFile);
	xml.getSym(minMonsters);
	xml.getSym(maxMonsters);
	xml.getSym(separationDistance);
}

bool Spawn::saveTidy(PropertyBag &xml, PropertyBag &editorData) const
{
	saveTag(xml, editorData, "monsterDataFile",     monsterDataFile);
	saveTag(xml, editorData, "minMonsters",         minMonsters);
	saveTag(xml, editorData, "maxMonsters",         maxMonsters);
	saveTag(xml, editorData, "separationDistance",  separationDistance);

	return Listener::saveTidy(xml, editorData);
}

} // namespace Engine
