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

#include "stdafx.h"
#include "gl.h"
#include "SDLwindow.h"
#include "ActorSet.h"
#include "player.h"
#include "world.h"
#include "ShadowManager.h"

namespace Engine {

void ShadowManager::clear(void)
{
	zone=0;

	periodicTimer = 0;

	shadows.clear();
}

void ShadowManager::destroy(void)
{
	release();
	clear();
}

void ShadowManager::create(void)
{
	destroy();

	for(size_t i=0; i<getMaxShadows(); ++i)
	{
		Shadow *s = new Shadow();
		s->create();
		shadows.push_back(s);
	}
}

void ShadowManager::release(void)
{
	// TODO: Properly release shadows

	/*for(size_t i=0; i<getMaxShadows(); ++i)
	{
		Shadow *shadow = shadows[i];
		shadow->destroy();
		delete shadow;
	}*/
	shadows.clear();
}

void ShadowManager::reaquire(void)
{
	// TODO: Properly reacquire shadows

	shadows.clear();

	for(size_t i=0; i<getMaxShadows(); ++i)
	{
		Shadow *s = new Shadow();
		s->create();
		shadows.push_back(s);
	}

	/*for(size_t i=0; i<getMaxShadows(); ++i)
		shadows[i]->reaquire();*/
}

void ShadowManager::setZone(World *zone)
{
	this->zone = zone;
}

void ShadowManager::update(const ActorSet &zoneActors, float deltaTime)
{
	CHECK_GL_ERROR();

	periodicTimer -= deltaTime;
	if(periodicTimer < 0)
	{
		periodicTimer = 500.0f + FRAND_RANGE(0.0f, 1000.0f); // stagger
		reassignShadows();
	}

	for(size_t i=0; i<getMaxShadows(); ++i)
	{
		shadows[i]->update(zoneActors, deltaTime);
	}

	glViewport(0, 0, SDLWindow::GetSingleton().GetWidth(), SDLWindow::GetSingleton().GetHeight());

	CHECK_GL_ERROR();
}

void ShadowManager::reassignShadows(void)
{
	ASSERT(zone!=0, "zone was null!  Make sure to call setZone before using the shadow manager.");

	// get the set of active lights
	vector<const Light*> lights = zone->getLightManager().getActiveSet();

	// remove lights that do not cause shadows
	vector<const Light*>::iterator iter = lights.begin();
	while(iter != lights.end())
	{
		const Light *light = *iter;

		if(light->causesShadows)
			++iter;
		else
			iter = lights.erase(iter);
	}

	if(lights.empty())
	{
		// disable all shadows
		for(size_t i=0; i<getMaxShadows(); ++i)
			shadows[i]->setLight(0);
	}
	else
	{
		// get the most active light
		const Light * light = lights[0];

		// If the most active light has changed, then we need to reflect that and force a shadow update
		for(size_t i=0; i<getMaxShadows(); ++i)
			shadows[i]->setLight(light);

		// Assign shadow[0] to the player
		Player &player = zone->getPlayer(0);
		shadows[0]->setActor(player.m_ID);

#if 1
		// Get the objects
		ActorSet s = zone->getObjects().getClosestSeveral<Actor>(light->getPosition(), MAX_SHADOWS, 20);

		// assign shadows to all shadow casters
		size_t shadowIdx=1;
		for(ActorSet::iterator iter = s.begin(); iter!=s.end() && shadowIdx<getMaxShadows(); ++iter)
		{
			Actor *p = iter->second;
			if(p->doesCastShadows() && p->m_ID!=player.m_ID)
			{
				shadows[shadowIdx]->setActor(p->m_ID);
				shadowIdx++;
			}
		}
#else
		size_t shadowIdx=1;
#endif

		// disable any remaining shadows
		while(shadowIdx<getMaxShadows())
		{
			shadows[shadowIdx]->setActor(0);
			++shadowIdx;
		}
	}
}

size_t ShadowManager::getMaxShadows(void) const
{
	return max(g_MultitextureUnits-2, 0);
}

const Shadow& ShadowManager::getShadow(size_t idx) const
{
	ASSERT(idx < getMaxShadows(), "idx is invalid: " + itoa((int)idx));
	ASSERT(shadows[idx]!=0, "shadows[idx] is null");
	return(*shadows[idx]);
}

}; // namespace
