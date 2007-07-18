/*
Original Author: Andrew Fox
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

#include "stdafx.h"
#include "gl.h"
#include "priority_queue.h"
#include "Frustum.h"
#include "World.h"
#include "Player.h"
#include "LightManager.h"

namespace Engine {


GLenum lightNames[7] =
{
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
	GL_LIGHT4,
	GL_LIGHT5,
	GL_LIGHT6,
	GL_LIGHT7,
};




void LightManager::clear(void)
{
	periodicTimer = 0.0f;
	newHandle = 1000;
	lights.clear();
}

void LightManager::destroy(void)
{
	clear();
}

void LightManager::create(void)
{
	clear();

	// (Add any initialization code here)
}

HLIGHT LightManager::addLight(Light *light)
{
	ASSERT(light!=0, _T("light was null"));

	newHandle++;
	lights.insert(make_pair(newHandle, light));
	computeActiveSet();
	return newHandle;
}

void LightManager::removeLight(HLIGHT handle)
{
	if(lights.find(handle) != lights.end())
	{
		lights.erase(handle);
		computeActiveSet();
	}
}

Light& LightManager::getLight(HLIGHT handle)
{
	LIGHTS::iterator i = lights.find(handle);

	ASSERT(i!=lights.end(), _T("handle was not found"));

	return *(i->second);
}

const Light& LightManager::getLight(HLIGHT handle) const
{
	LIGHTS::const_iterator i = lights.find(handle);

	ASSERT(i!=lights.end(), _T("handle was not found"));

	return *(i->second);
}

vector<const Light*> LightManager::getActiveSet(void) const
{
	return activeSet;
}

void LightManager::update(float deltaTime)
{
	// Periodically update the active set
	periodicTimer -= deltaTime;
	if(periodicTimer < 0)
	{
		periodicTimer = 1000.0f;
		computeActiveSet();
	}
}

void LightManager::computeActiveSet(void)
{
	ASSERT(World::GetSingletonPtr()!=0, _T("World is null"));

	Player *playerPtr = World::GetSingleton().getPlayerPtr(0);

	if(playerPtr == 0)
	{
		activeSet.clear(); // reset
		return;
	}

	PriorityQueue_Less_F q;

	const Player &player = *playerPtr;
	const vec3 playerPos = player.getPos() + vec3(0, player.getSphereRadius(), 0);

	for(LIGHTS::const_iterator iter=lights.begin(); iter!=lights.end(); ++iter)
	{
		const Light *light = iter->second;

		// Only consider lights that are allowed to be enabled
		if(light->enable)
		{
			float distance = vec3(  playerPos - light->getPosition()  ).getMagnitude();
			prioritize_f p = {distance, (void*)(light)};
			q.push(p);
		}
	}

	activeSet.clear(); // reset
	while(!q.empty())
	{
		activeSet.push_back(static_cast<const Light*>(q.top().object));
		q.pop();
	}
}

void LightManager::disableAll(void) const
{
	for(size_t i=0; i<7; ++i)
		glDisable(lightNames[i]);
}

void LightManager::bindAll(void) const
{
	vector<const Light*> s = getActiveSet();

	disableAll();

	// Bind the 7 active lights
	for(size_t i=0; i<7 && i<s.size(); ++i)
	{
		s[i]->bind(lightNames[i]);
		glEnable(lightNames[i]);
	}
}

void LightManager::release(void)
{}

void LightManager::reaquire(void)
{}

}; // namespace
