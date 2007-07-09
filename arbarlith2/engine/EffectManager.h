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

#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

#include <map>
using std::map;

#include "Effect.h"
#include "EffectSig.h"
#include "Singleton.h"


namespace Engine {


/**
This manages the global state of the Effect system.  It allows an Effect
to be selected for use by the application and manages the prioritization
and linking of Effects to ensure that the most appropriate Effects
classes are bound to each Effect ID for the given hardware platform.
*/
class EffectManager : public Singleton<EffectManager>
{
private:
	typedef map<effect_sig, Effect*> LinkTable;

	/** This table links to the raw effect classes*/
	LinkTable rawEffects;

	/** This table links the previously created effects to the effect signature they have been chosen for */
	LinkTable linkTable;

	/** This points to the Effect that is currently selected*/
	Effect *currentEffect;

	/**
	Returns a pointer to the queried effect
	@param signature The effect signature of the requested effect class
	@return a pointer to the Effect, or NULL
	*/
	Effect* queryRaw(effect_sig signature);

	/**
	Determine if a sequence of OpenGL extensions in the given _tstring are supported.
	@param The extension _tstring to test
	@return If any extension is unsupported, then return false.
	*/
	bool isSupported(const _tstring &extensions);

	/** signature of the currently bound effect */
	effect_sig currentSig;

public:
	/** Default constructor */
	EffectManager(void);

	/** Destructor */
	~EffectManager(void);

	/** Clears out and constructs the EffectManager data*/
	void clear(void);

	/** Creates the Effect linkage and sets a default Effect*/
	void create(void);

	/** Cleanly destroys EffectManager data */
	void destroy(void);

	/**
	Binds an effect signature to select a new current Effect object
	@param signature The Effect signature of the new Effect.  This
	signature is used to identify the effect that was previously
	bound to that signature when the EffectManager linkahe was
	created.  If no effects satisfy the signature, then the default
	effect is returned.
	@return true if the underlying Effect was modified, false otherwise
	*/
	bool bind(effect_sig signature);

	/**
	Returns a reference to the currently bound Effect.
	@return the currently bound effect
	*/
	Effect& getEffect(void);

	/**
	Returns the sig of the current effect
	@return the signature of currently bound effect
	*/
	effect_sig getEffectSig(void) const
	{
		return currentSig;
	}

	/**
	Returns a pointer to the queried effect
	@param signature The effect signature of the requested effect
	@return a pointer to the Effect, or NULL
	*/
	Effect* query(effect_sig signature);
};

extern EffectManager g_EffectManager;

} // namespace Engine


#endif
