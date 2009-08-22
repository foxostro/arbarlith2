/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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

#include "primitivedatatypes.h"

#include "EffectManager.h"
#include "Effect.h"
#include "Effect_TextureReplace_FFP.h"
#include "Effect_Gouraud_FFP.h"
#include "Effect_Particle_FFP.h"
#include "Effect_GUI_FFP.h"
#include "Effect_Red_FFP.h"
#include "Effect_Project_Shadows.h"
#include "Effect_Receive_Shadows.h"
#include "Effect_Z_Only.h"


namespace Engine {

/** Instantiate the global effect manager singleton */
EffectManager g_EffectManager;

bool effect_Begin(effect_sig signature)
{
	g_EffectManager.bind(signature);
	g_EffectManager.getEffect().begin();
	return true;
}

void effect_End(void)
{
	g_EffectManager.getEffect().end();
	g_EffectManager.bind(0);
}

EffectManager::EffectManager(void)
{
	clear();
}

EffectManager::~EffectManager(void)
{
	destroy();
}

void EffectManager::clear(void)
{
	currentEffect=0;
	currentSig=0;
}

void EffectManager::create(void)
{
	destroy();

	// Allocate all effects
	TRACE("Allocating all Effects");
	LinkTable temp;
	temp.insert(make_pair(effect_CLASS_Z_ONLY,						new Effect_Z_Only));
	temp.insert(make_pair(effect_CLASS_PROJECT_SHADOWS,				new Effect_Project_Shadows));
	temp.insert(make_pair(effect_CLASS_RECEIVE_SHADOWS,				new Effect_Receive_Shadows));
	temp.insert(make_pair(effect_CLASS_GOURAUD_FFP,					new Effect_Gouraud_FFP));
	temp.insert(make_pair(effect_CLASS_TEXTURE_REPLACE_FFP,			new Effect_TextureReplace_FFP));
	temp.insert(make_pair(effect_CLASS_PARTICLE_FX,					new Effect_Particle_FFP));
	temp.insert(make_pair(effect_CLASS_GUI,							new Effect_GUI_FFP));
	temp.insert(make_pair(effect_CLASS_RED,							new Effect_Red_FFP));

	// Iterate through effects and eliminate effects that are not supported
	TRACE("Determining supported Effects");
	for(LinkTable::iterator iter = temp.begin(); iter!=temp.end(); ++iter)
	{
		Effect *effect = (iter->second);
		ASSERT(effect != 0, "null Effect");

		// Do we have enough texture units to use the effect?
		if(effect->getRequiredTextureUnits() > g_MultitextureUnits)
		{
			// Discard the effect and continue
			TRACE(effect->getName() + ": Effect requires too many texture stages, discarding...");
			delete effect;
			continue;
		}
		// Do we have the extension support?
		else if(!isSupported(effect->getRequiredExtensions()))
		{
			// Discard the effect and continue
			TRACE(effect->getName() + ": Effect requires unsupported or disabled extensions, discarding...");
			//delete effect;
			continue;
		}
		// Effect is supported
		else
		{
			rawEffects.insert(make_pair(iter->first, iter->second));
		}
	}

	// Initialize all effect classes
	TRACE("Initializing the supported Effects");
	for(LinkTable::iterator iter = rawEffects.begin(); iter!=rawEffects.end(); ++iter)
	{
		(iter->second)->create();
	}

	// Then, insert those effects that are supported into the link table
	for(effect_sig i = 1; i<effect_CLASS_MAX; ++i)
	{
		Effect *effect = queryRaw(i);
		ASSERT(effect!=0, "EffectManager::create  ->  Effect was NULL");

		linkTable.insert(make_pair(i, effect));
	}

// Determine the best fit effects for the templated effect signatures

	// These are the effects we will work with to link together
	effect_sig effects[] =
	{
		effect_DEFAULT,
		effect_TEXTURE_REPLACE,
		effect_NO_TEXTURE_LIT,
		effect_TEXTURE_LIT,
		effect_GUI,
		effect_PARTICLE_FX,
		effect_TRANSPARENT,
		effect_RED
	};

	// For each effect signature:
	TRACE("Linking Effects to effect signature templates");
	for(size_t i=0; i<sizeof(effects); ++i)
	{
		priority_queue< pair<int,Effect*>, vector< pair<int,Effect*> >, less< pair<int,Effect*> > > options;

		effect_sig sig = effects[i];

		// For each effect class, determine the priority for this signature
		for(LinkTable::iterator iter = rawEffects.begin(); iter!=rawEffects.end(); ++iter)
		{
			options.push( make_pair( (iter->second)->howGood(sig), iter->second ) );
		}

		// Select the highest priority for this signature
		Effect *best = options.top().second;

		// Link to the best effect that was found for this signature
		linkTable.insert(  make_pair(sig, best)  );
	}

	// select no initial Effect
	currentEffect = 0;


	TRACE("Completed");
}

void EffectManager::destroy(void)
{
	TRACE("Destroying existing Effects");
	clear();
}

bool EffectManager::bind(effect_sig signature)
{
	if(signature==0)
	{
		currentEffect = 0;
	}
	else
	{
		// First, make sure there isn't already an active effect
		ASSERT(currentEffect==0, "there is already an active effect!");

		// What effect best matches the requested signature?
		Effect *requested = query(signature);
		ASSERT(requested != 0, "Effect was NULL, linker should have taken care of that!");

		currentEffect = requested;

		currentSig = signature;
	}

	// Indicate that we did change the Effect
	return true;
}

Effect& EffectManager::getEffect(void)
{
	ASSERT(currentEffect!=0, "There is currently no bound effect!");
	return(*currentEffect);
}

Effect* EffectManager::queryRaw(effect_sig signature)
{
	ASSERT(!rawEffects.empty(), "EffectManager::queryRaw  ->  rawEffects was empty");

	LinkTable::iterator iter = rawEffects.find(signature);

	LinkTable::iterator _end = rawEffects.end();

	if(iter != _end)
	{
		return(iter->second);
	}
	else
	{
		// Some default effect
		return(rawEffects.begin()->second);
	}
}

Effect* EffectManager::query(effect_sig signature)
{
	LinkTable::iterator iter = linkTable.find(signature);

	if(iter != linkTable.end())
	{
		return(iter->second);
	}
	else
	{
		return 0;
	}
}

bool EffectManager::isSupported(const string &str)
{
    tokenizer<> extensions(str);

	for(tokenizer<>::const_iterator iter = extensions.begin(); iter != extensions.end(); ++iter)
	{
		if(!Engine::OpenGL::CheckExtension(iter->c_str()))
			return false;
	}

	return true;
}

} // namespace Engine
