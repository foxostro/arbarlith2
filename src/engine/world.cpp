/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009 Game Creation Society
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
#include "profile.h"
#include "searchfile.h"

#include "WaitScreen.h"
#include "player.h"
#include "frustum.h"
#include "EditorToolBar.h"
#include "Dimmer.h"
#include "EffectSig.h"
#include "EffectManager.h"

#include "world.h"

extern bool _PLAYER_ONE_HAS_NO_JOYSTICK_;	// Set to 1 in order to force player 1 to the keyboard, player 2 to joystick 1, etc

namespace Engine {

vec3 projectVector(const vec3 &obj); // opengl.cpp

extern GLenum lightNames[7];

GLenum textureStages[32] =
{
	GL_TEXTURE0_ARB,
	GL_TEXTURE1_ARB,
	GL_TEXTURE2_ARB,
	GL_TEXTURE3_ARB,
	GL_TEXTURE4_ARB,
	GL_TEXTURE5_ARB,
	GL_TEXTURE6_ARB,
	GL_TEXTURE7_ARB,
	GL_TEXTURE8_ARB,
	GL_TEXTURE9_ARB,
	GL_TEXTURE10_ARB,
	GL_TEXTURE11_ARB,
	GL_TEXTURE12_ARB,
	GL_TEXTURE13_ARB,
	GL_TEXTURE14_ARB,
	GL_TEXTURE15_ARB,
	GL_TEXTURE16_ARB,
	GL_TEXTURE17_ARB,
	GL_TEXTURE18_ARB,
	GL_TEXTURE19_ARB,
	GL_TEXTURE20_ARB,
	GL_TEXTURE21_ARB,
	GL_TEXTURE22_ARB,
	GL_TEXTURE23_ARB,
	GL_TEXTURE24_ARB,
	GL_TEXTURE25_ARB,
	GL_TEXTURE26_ARB,
	GL_TEXTURE27_ARB,
	GL_TEXTURE28_ARB,
	GL_TEXTURE29_ARB,
	GL_TEXTURE30_ARB,
	GL_TEXTURE31_ARB
};

World::World(void)
{
	clear();
	router.setZone(this);
}

World::~World()
{
	destroy();
}

void World::loadFromFile(const string &fileName)
{
	PropertyBag bag;
	bag.loadFromFile(fileName);
	load(bag);
}

void World::clear()
{
	particles.clear();
	nextParticleHandle = 5000; // Start counting thew handles at 5000

	objects.clear();
	lightManager.clear();
	shadowManager.clear();
	worldMap.clear();

	name = "nill";
	clockTicks=0.0f;
	NumOfPlayers = 0;

	for(size_t i=0; i<MAX_PLAYERS; ++i) player[i]=0;
}

void World::destroy(void)
{
	TRACE("Destroying World...");

	worldMap.destroy();
	TRACE("...destroyed worldMap...");

	lightManager.destroy();
	TRACE("...destroyed lightManager...");

	shadowManager.destroy();
	TRACE("...destroyed shadowManager...");

	objects.destroy();
	TRACE("...destroyed objects...");

	clear();


	TRACE("...finished Destroying World");
}

void World::release(void)
{
	worldMap.release();
	lightManager.release();
	shadowManager.release();
}

void World::reaquire(void)
{
	worldMap.reaquire();
	lightManager.reaquire();
	shadowManager.reaquire();
}

PropertyBag World::save(void) const
{
	TRACE("Saving game world...");

	PropertyBag bag;

	bag.add("objects", getObjects().save());
	bag.add("player", getPlayer(0).save());
	bag.add("map", worldMap.save(name));
	bag.add("name", name);
	bag.add("fog", fog.save());
	bag.add("ambientLight", getLightManager().ambientLight);
	bag.add("music", music.save());

	TRACE("...finished");

	return bag;
}

void World::load(const PropertyBag &bag)
{
	TRACE("Loading game world...");
	g_WaitScreen.Render();

	// Destroy any old instance of this realm
	destroy();
	TRACE("...destroyed existing World data first...");

	name = bag.getString("name");

	// Load the music set
	music.load( bag.getBag("music") );
	music.update();

	// Load the fog
	fog.load( bag.getBag("fog") );

	// Load the ambient lighting intensity for the World
	Light::BRIGHTNESS = 1.0f;
	Dimmer::alphaBlur = 0.0f;
	getLightManager().ambientLight = bag.getFloat("ambientLight");

	// Load the map
	worldMap.create(bag.getBag("map"));

	// Initialize the lighting system
	lightManager.create();
	shadowManager.create();
	shadowManager.setZone(this);

	// Load the objects bags
	getObjects().load(bag.getBag("objects"), this);

	// Load the players
	reloadPlayers( bag.getBag("player") );

	TRACE("...finished (Loading \"" + getName() + ")");
}

void World::draw(void) const
{
	PROFILE

	CHECK_GL_ERROR();

	if(g_Application.getState() == GAME_STATE_EDITOR)
	{
		drawEditorScene();
	}
	else
	{
		switch(g_Application.graphicsMode)
		{
		case Application::SHADOWS_AND_LIGHTING_ENABLED:		drawScene();			break;
		case Application::LIGHTING_ENABLED:			drawShadowlessScene(true);	break;
		case Application::SHADOWS_AND_LIGHTING_DISABLED:	drawShadowlessScene(false);	break;
		};
	}
}

void World::drawEditorScene(void) const
{
CHECK_GL_ERROR();

	g_Camera.setCamera();

	// draw the scene geometry
	effect_Begin(effect_TEXTURE_REPLACE);
		worldMap.draw();
		objects.draw(g_Camera.getFrustum());
	effect_End();

CHECK_GL_ERROR();
}

void World::drawShadowlessScene(bool useLights) const
{
CHECK_GL_ERROR();

	g_Camera.setCamera();

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white*lightManager.ambientLight*Light::BRIGHTNESS); // scene ambient light
	lightManager.bindAll();
	fog.activate();

	// draw the scene geometry
	effect_Begin(useLights ? effect_TEXTURE_LIT : effect_TEXTURE_REPLACE);
		worldMap.draw();
		objects.draw(g_Camera.getFrustum());
	effect_End();

	// particles last
	drawParticles();

CHECK_GL_ERROR();
}

void World::drawScene(void) const
{
CHECK_GL_ERROR();

	/*
	Algorithm summary:
	1) Draw areas that might be in shadow into the stencil buffer
	2) In areas that are definitely not affected by shadows, draw with full lighting
	3) In areas that might be affected by shadows:
		4) Draw with ambient light
		5) Draw with full lighting, but fail fragments that are in shadow
			* Alphatest
			* Shadowmap projection to achieve the failing alpha values
	*/

	// set the ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white*lightManager.ambientLight*Light::BRIGHTNESS);



	// set the camera
	g_Camera.setCamera();


	// Draw areas that might be affected shadows into the stencil buffer
	effect_Begin(effect_RED);
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_LIGHTING);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			for(unsigned int i=0; i<shadowManager.getMaxShadows(); ++i)
			{
				const Shadow &s = shadowManager.getShadow(i);
				if(s.isInUse())
				{
					s.getFrustum().beginClipping();
					drawShadowReceivers();
				}
			}
		glPopAttrib();
	effect_End();
	glClear(GL_DEPTH_BUFFER_BIT);




	// draw the scene with shadows
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		// for the following steps, we need to enable the stencil test but refrain from modifying the stencil itself
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFFFF); // only where shadows are garaunteed to not be cast

				// draw areas of the scene unaffected by shadows
				effect_Begin(effect_TEXTURE_LIT);
					lightManager.bindAll();
					drawShadowReceivers();
				effect_End();

		glStencilFunc(GL_EQUAL, 1, 0xFFFF); // only where shadows might be cast

				// ambient pass of the scene
				if(g_Application.displayDebugData)
				{
					effect_Begin(effect_RED);
				}
				else
				{
					effect_Begin(effect_TEXTURE_LIT);
					lightManager.disableAll(); // ambient light only
				}
					drawShadowReceivers();
				effect_End();

				// draw the shadowed portions of the map
				effect_Begin(effect_CLASS_RECEIVE_SHADOWS);

					lightManager.bindAll(); // full lighting

					// bind all shadows that are in use
					unsigned int tex=1;
					for(unsigned int i=0; i<shadowManager.getMaxShadows(); ++i)
					{
						const Shadow &s = shadowManager.getShadow(i);
						if(s.isInUse())
						{
							s.bind(getObjects(), tex++);
						}
					}

					drawShadowReceivers();

				effect_End();
	glPopAttrib();

	if(g_Application.displayDebugData)
	{
		shadowManager.getShadow(0).testDepthTexture();
		shadowManager.getShadow(0).drawFrustum();
	}

	// last
	drawParticles();

CHECK_GL_ERROR();
}

void World::drawShadowReceivers(void) const
{
	worldMap.draw();
	objects.draw(g_Camera.getFrustum());
}

void World::drawParticles(void) const
{
	// Render particles in the realm
	if(g_Application.useParticleEffects && particles.size()>0)
	{
		effect_Begin(effect_PARTICLE_FX);

		for(map<size_t, ParticleSystem*>::const_iterator iter=particles.begin();
		    iter!=particles.end();
		    ++iter)
		{
			ParticleSystem *system = iter->second;

			ASSERT(system!=0, "system was NULL");

			if(!system->isDead())
			{
				system->draw();
			}
		}

		effect_End();
	}
}

void World::updateParticles(float deltaTime)
{
	if(g_Application.useParticleEffects && particles.size()>0)
	{
		vector< map<size_t, ParticleSystem*>::iterator > toErase;

		for(map<size_t, ParticleSystem*>::iterator i = particles.begin();
		    i != particles.end();
		    ++i)
		{
			ParticleSystem *s = i->second;

			ASSERT(s != 0, "system was NULL");

			if(!s || (s != 0 && s->isDead()))
			{
				delete s;
				toErase.push_back(i);
			}
			else
			{
				s->update(deltaTime);
			}
		}

		for(vector< map<size_t, ParticleSystem*>::iterator >::const_iterator i
					= toErase.begin();
		    i != toErase.end();
		    ++i)
		{
			particles.erase(*i);
		}
	}
}

void World::update(float deltaTime)
{
	PROFILE

	clockTicks += double(deltaTime); // Update the game clock

	objects.update(deltaTime, this);

	router.update(deltaTime);

	lightManager.update(deltaTime);

	updateParticles(deltaTime);

	updateShadows(deltaTime);

	recalculateAveragePlayerPosition();
	updateCamera();
}

void World::updateShadows(float deltaTime)
{
	if(g_Application.graphicsMode == Application::SHADOWS_AND_LIGHTING_ENABLED)
	{
		PROFILE
		shadowManager.update(getObjects(), deltaTime);
	}
}

size_t World::SpawnPfx(const string &fileName, const vec3 &position)
{
	size_t handle = nextParticleHandle++;

	ParticleSystem *s = new ParticleSystem(fileName);

	s->setPosition(position);

	particles.insert(make_pair(handle, s));

	// Return the particle system's ID
	return handle;
}

const Player& World::getPlayer(size_t playerNum) const
{
	const Player *player = getPlayerPtr(playerNum);

	ASSERT(player!=0, "player[" + itoa((int)playerNum) + "] was null");

	return(*player);
}

Player& World::getPlayer(size_t playerNum)
{
	ASSERT(playerNum < MAX_PLAYERS, "invalid player number");

	Player *player = getPlayerPtr(playerNum);

	ASSERT(player!=0, "player[" + itoa((int)playerNum) + "] was null");

	return(*player);
}

const Player* World::getPlayerPtr(size_t playerNum) const
{
	ASSERT(playerNum < MAX_PLAYERS, "invalid player number");
	return player[playerNum];
}

Player* World::getPlayerPtr(size_t playerNum)
{
	ASSERT(playerNum < MAX_PLAYERS, "invalid player number");
	return player[playerNum];
}

void World::reloadPlayers(const PropertyBag &playerBag)
{
	TRACE("Reloading all the players...");


	string file, type;

	if(playerBag.getSym(file))
	{
		// The type is defined in an external file
		PropertyBag external;
		external.loadFromFile(file);
		external.getSym(type);
	}
	else
	{
		// No exernal data file
		playerBag.getSym(type);
	}

	const int numOfJoysticks = SDL_NumJoysticks();

    if(_PLAYER_ONE_HAS_NO_JOYSTICK_)
    {
	    TRACE("_PLAYER_ONE_HAS_NO_JOYSTICK_ is defined, so player #1 will always be on the keyboard");
	    NumOfPlayers = (numOfJoysticks==0) ? 1 : min(numOfJoysticks + 1, MAX_PLAYERS);
    }
    else
    {
	    TRACE("_PLAYER_ONE_HAS_NO_JOYSTICK_ is NOT defined, so player #1 will always be on the keyboard and joystick #1");
	    NumOfPlayers = (numOfJoysticks==0) ? 1 : min(numOfJoysticks, MAX_PLAYERS);
    }

	TRACE("Expecting " + itoa((int)NumOfPlayers) + " players");

	// Create the players
	for(int i = 0; (size_t)i < NumOfPlayers; ++i)
	{
		player[i] = dynamic_cast<Player*>(getObjects().createPtr(type, this));
		ASSERT(player[i]!=0, "Failed to construct player #" + itoa(i));

		player[i]->load(playerBag);
		player[i]->setPlayerNumber(i);

		if(NumOfPlayers>1)
		{
			const float angle = 2.0f * ((float)i/NumOfPlayers) * (float)M_PI;
			const vec3 offset = vec3(cosf(angle), 0, sinf(angle)) * 1.2f;
			player[i]->Place(player[i]->getPos() + offset);
		}

		TRACE("Loaded player #" + itoa(i));
	}

	TRACE("...finished (Reloading the players)");
}

vec3 World::findAveragePlayerPosition(void) const
{
	vec3 averagePlayerPosition;
	const size_t numOfPlayers = getNumOfPlayers();

	ASSERT(numOfPlayers!=0, "Number of players is zero, and it shouldn't be.");
	ASSERT(numOfPlayers < MAX_PLAYERS, "Number of players (" + itoa((int)numOfPlayers) + ") is too large!  The maximum is " + itoa((int)MAX_PLAYERS));

	for(size_t i=0; i<numOfPlayers; ++i)
	{
		const Player &player = getPlayer(i);
		averagePlayerPosition = averagePlayerPosition + player.getPos();
	}
	averagePlayerPosition = averagePlayerPosition * (1.0f/numOfPlayers);

	return averagePlayerPosition;
}

void World::updateCamera(void)
{
	const float minCameraDistance = 6.0f;
	float cameraDistance = minCameraDistance;
	const vec3 averagePlayerPosition = getAveragePlayerPosition();
	const size_t numOfPlayers = getNumOfPlayers();

	if(numOfPlayers>1)
	{
		float maxPlayerDistance=0;

		// Find the position of the player that is farthest away from the average position
		for(size_t i=0; i<numOfPlayers; ++i)
		{
			const Player &player = getPlayer(i);
			maxPlayerDistance = max(maxPlayerDistance, player.getDistanceFromAveragePosition());
		}

		cameraDistance = max(minCameraDistance, maxPlayerDistance + minCameraDistance);
	}

	g_Camera.lookAt(averagePlayerPosition + vec3(cameraDistance,cameraDistance,cameraDistance), averagePlayerPosition, vec3(0,1,0));
}

bool World::isParticleSystemValid(size_t handle) const
{
	return particles.find(handle) != particles.end();
}

ParticleSystem& World::getParticleSystem(size_t handle)
{
	ASSERT(isParticleSystemValid(handle),
		   "Paricle handle is invalid: " + itoa((int)handle));

	ParticleSystem *s = particles.find(handle)->second;

	ASSERT(s != 0, "particles[handle] is null!");

	return *s;
}

const ParticleSystem& World::getParticleSystem(size_t handle) const
{
	ASSERT(isParticleSystemValid(handle),
		   "Paricle handle is invalid: " + itoa((int)handle));

	const ParticleSystem *s = particles.find(handle)->second;

	ASSERT(s != 0, "particles[handle] is null!");

	return *s;
}

} // namespace Engine
