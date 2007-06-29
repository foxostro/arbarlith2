/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#include "stdafx.h" // Master Header
#include "gl.h"

#include "Zone.h"
#include "Player.h"
#include "SearchFile.h"
#include "waitscreen.h"
#include "frustum.h"
#include "priority_queue.h"
#include "EditorToolBar.h"
#include "Dimmer.h"
#include "profile.h"

#include "EffectSig.h"
#include "EffectManager.h"


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



Zone::Zone(void)
: router(objects)
{
	clear();
}

Zone::Zone(const _tstring &configFileName)
: router(objects)
{
	clear();
	LoadXml(configFileName);
}

Zone::~Zone()
{
	destroy();
	xmlCache.Clear();
}

//////////////////////////////////////////////////////////////////////
// Member functions        
//////////////////////////////////////////////////////////////////////

void Zone::clear()
{
	name = _T("Pudonk");

	loaded = false;

	particles.clear();
	objects.clear();
	lightManager.clear();
	shadowManager.clear();
	map.clear();
}

void Zone::destroy(void)
{
	map.destroy();
	lightManager.destroy();
	shadowManager.destroy();
	objects.destroy();

	clear();
}

void Zone::release(void)
{
	map.release();
	lightManager.release();
	shadowManager.release();
}

void Zone::reaquire(void)
{
	map.reaquire();
	lightManager.reaquire();
	shadowManager.reaquire();
}

bool Zone::SaveXml(CPropBag &Bag)
{
	if(isLoaded() == false)
	{
		// If the realm was never loaded, then save the XML cache
		Bag = xmlCache;
	}
	else
	{
		// Save dynamic objects
		Bag.Add(_T("objects"), getObjects().save());

		// Save the map
		Bag.Add(_T("map"), map.save(name));

		// Add in misc items
		Bag.Add(_T("name"), name);

		// Save the fog settings
		Bag.Add(_T("fog"), fog.save());

		// Load the ambient lighting intensity for the Zone
		Bag.Add(_T("ambientLight"), getLightManager().ambientLight);

		// Save the music engine
		CPropBag musicEngine;
		music.save(musicEngine);
		Bag.Add(_T("music"), musicEngine);
	}

	// return with success
	return true;
}

bool Zone::LoadXml(CPropBag &Bag)
{
	// Do the waitscreen every so often
	TRACE(_T("Loading another realm"));
	g_WaitScreen.Render();

	_tstring strDataFile;

	// Get the data file, if one is specified
	if(Bag.GetNumInstances(_T("file")) > 0)
	{
		Bag.Get(_T("file"), strDataFile);

		// wait screen
		TRACE(_tstring(_T("External data file: ")) + strDataFile);
		g_WaitScreen.Render();

		// Merge the data
		Bag.LoadMerge(strDataFile);

		// wait screen
		TRACE(_T("External data file loaded"));
		g_WaitScreen.Render();
	}

	// Load data
	LoadData(Bag);

	/*
	Invalidate the XML cache. Bag either was a reference to  it, and we don't need 
	it anymore, or another XML source was optioned, and we don't need it anymore.
	*/
	xmlCache.Clear();

	// The realm has been loaded successfully :)
	loaded = true;

	// Return success
	return true;
}

bool Zone::LoadData(CPropBag &Bag)
{
	// wait screen
	TRACE(_T("Loading realm data"));
	g_WaitScreen.Render();

	_tstring rtti;

	// Destroy any old instance of this realm
	destroy();
	TRACE(_T("Destroyed old realm data"));
	g_WaitScreen.Render();	

	// Load the realm's name
	if(!Bag.getSym(name)==false)
	{
		ERR(_T("No name tag in realm definition."));
		g_WaitScreen.Render();
	}

// Load the music set
	CPropBag musicBag;
	if(Bag.Get(_T("music"), musicBag))
	{
		TRACE(getName() + _T(": Loading music data"));
		g_WaitScreen.Render();

		music.load(musicBag);
		music.update();
	}

	// Load the fog
	CPropBag fogBag;
	if(Bag.Get(_T("fog"), fogBag))
	{
		TRACE(getName() + _T(": Loading fog data"));
		g_WaitScreen.Render();
		fog.load(fogBag);
	}

	// Load the ambient lighting intensity for the Zone
	Bag.Get(_T("ambientLight"), getLightManager().ambientLight);
	Light::BRIGHTNESS = 1.0f; // reset
	Dimmer::alphaBlur = 0.0f;
	
// Load the map
	CPropBag mapBag;
	if(Bag.Get(_T("map"), mapBag))
	{
		map.create(mapBag);
	}

// Initialize the lighting system
	lightManager.create();
	shadowManager.create();
	shadowManager.setZone(this);
	
// Load the objects bags
	CPropBag ObjectsBag;

	if(Bag.Get(_T("objects"), ObjectsBag)==false)
	{
		ERR(name + _T(": No objects tag in realm definition."));
		g_WaitScreen.Render();
	}

	// load
	getObjects().load(ObjectsBag, this);

	TRACE(getName() + _T(": All Objects Loaded"));
	g_WaitScreen.Render();




	// return with success
	TRACE(getName() + _T(": Load Successful"));
	g_WaitScreen.Render();
	return true;
}

void Zone::draw(void) const
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

void Zone::drawEditorScene(void) const
{
CHECK_GL_ERROR();

	g_Camera.setCamera();

	// draw the scene geometry
	effect_Begin(effect_TEXTURE_REPLACE);
		map.draw();
		objects.draw(g_Camera.getFrustum());
	effect_End();

CHECK_GL_ERROR();
}

void Zone::drawShadowlessScene(bool useLights) const
{
CHECK_GL_ERROR();

	g_Camera.setCamera();

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white*lightManager.ambientLight*Light::BRIGHTNESS); // scene ambient light
	lightManager.bindAll();
	fog.activate();

	// draw the scene geometry
	effect_Begin(useLights ? effect_TEXTURE_LIT : effect_TEXTURE_REPLACE);
		map.draw();
		objects.draw(g_Camera.getFrustum());
	effect_End();

	// particles last
	drawParticles();

CHECK_GL_ERROR();
}

void Zone::drawScene(void) const
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

void Zone::drawShadowReceivers(void) const
{
	map.draw();
	objects.draw(g_Camera.getFrustum());
}

void Zone::drawParticles(void) const
{
	// Render particles in the realm
	if(g_Application.useParticleEffects && particles.size()>0)
	{
		effect_Begin(effect_PARTICLE_FX);

		for(PARTICLE_SYSTEMS::const_iterator iter=particles.begin(); iter!=particles.end(); ++iter)
		{
			Engine::ParticleSystem *system = (*iter);

			ASSERT(system!=0, _T("system was NULL"));

			if(!system->IsDead())
			{
				system->Render();
			}
		}

		effect_End();
	}
}

void Zone::updateParticles(float deltaTime)
{
	if(g_Application.useParticleEffects && particles.size()>0)
	{
		PARTICLE_SYSTEMS::iterator i = particles.begin();

		while(i != particles.end())
		{
			ParticleSystem *s = (*i);
			if(s==0 || (s!=0 && s->IsDead()))
			{
				delete s;
				i = particles.erase(i);
			}
			else
			{
				s->Update(deltaTime);
				++i;
			}
		}
	}
}

void Zone::update(float deltaTime)
{
	PROFILE

	objects.update(deltaTime, this);

	router.update(deltaTime);

	lightManager.update(deltaTime);

	updateParticles(deltaTime);

	updateShadows(deltaTime);

	g_World.updateCamera();
}

void Zone::updateShadows(float deltaTime)
{
	if(g_Application.graphicsMode == Application::SHADOWS_AND_LIGHTING_ENABLED)
	{
		PROFILE
		shadowManager.update(getObjects(), deltaTime);
	}
}

size_t Zone::SpawnPfx(_tstring strFile, const vec3 &position)
{
	ASSERT(!strFile.empty(), _T("Bad parameter strFile specifies no filename"));

	ParticleSystem *s = new ParticleSystem(strFile);
	s->setPosition(position);

	particles.push_back(s);

	// Return the particle system's ID
	return(particles.size()-1);
}


}; // namespace
