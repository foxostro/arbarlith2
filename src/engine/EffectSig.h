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

#ifndef _EFFECT_SIGNATURES_H_
#define _EFFECT_SIGNATURES_H_


namespace Engine {



/** Defaine a type to contain an effect signature value */
typedef size_t effect_sig;

const effect_sig effect_DEFAULT				= 100;	/** Selects the effect that was chosen as the default effect by the EffectManager's linker */
const effect_sig effect_TEXTURE_REPLACE		= 500;	/** Selects a simple texture replacement effect */
const effect_sig effect_NO_TEXTURE_LIT		= 600;	/** Selects an effect to light with no texture */
const effect_sig effect_TEXTURE_LIT			= 700;	/** Selects an effect to light with one texture */
const effect_sig effect_PARTICLE_FX			= 910;	/** Selects an effect to render a transparent particle */
const effect_sig effect_GUI					= 1100;	/** Selects an effect to render in Ortho2D with textured billboards */
const effect_sig effect_TRANSPARENT			= 1200;	/** Selects an effect to render a transparent model */
const effect_sig effect_RED					= 1400; /** Selects an effect to render a model so that its red, lit, and untextured */

/*
It is also possible to specify that a specific Effect class is to be used, rather than
a template of an effect that may have been prioritized accordingto hardware support.
These effect signatures are from the set of signature from 1 to 499 and are reserved for
specific effect classes.

Makes sure to update EffectManager::create when editting this!
*/
const effect_sig effect_CLASS_GOURAUD_FFP					= 1;
const effect_sig effect_CLASS_TEXTURE_REPLACE_FFP			= 2;
const effect_sig effect_CLASS_PARTICLE_FX					= 3;
const effect_sig effect_CLASS_GUI							= 4;
const effect_sig effect_CLASS_RED							= 5;
const effect_sig effect_CLASS_PROJECT_SHADOWS				= 6;
const effect_sig effect_CLASS_RECEIVE_SHADOWS				= 7;
const effect_sig effect_CLASS_Z_ONLY						= 8;
const effect_sig effect_CLASS_MAX							= 9;

/**
Binds an effect to use when rendering.  It is an error to begin
an effect while one has not yet been ended.
@param signature The effect signature of the effect to select
*/
bool effect_Begin(effect_sig signature);

/** Stops using the currently bound Effect.  It is an error to end an effect while one is not bound. */
void effect_End(void);

} // namespace Engine

#endif
