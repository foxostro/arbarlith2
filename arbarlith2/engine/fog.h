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

#ifndef FOG_H
#define FOG_H

#include "propbag.h"


namespace Engine {


/** OpenGL fog */
class Fog
{
private:
	/** Fog Density */
	float density;

	/** Near distance of fog */
	float fnear;

	/** Far distance of fog */
	float ffar;

	/** Fog Color: red component */
	float red;

	/** Fog Color: green component */
	float green;

	/** Fog Color: blue component */
	float blue;

	/** should fogging be used? */
	bool shouldBeUsed;

public:
	/**
	Default constructor
	*/
	Fog(void);

	/**
	Saves the fog to XML
	@return XML reprentation of the fog
	*/
	CPropBag save(void) const;

	/**
	Loads the fog settings from XML
	@param xml XML source
	@return true if successful, false otherwise
	*/
	bool load(CPropBag &xml);

	/** Activate the fog settings */
	void activate(void) const;

	/** Deactivate fog */
	void deactivate(void) const;
};

} // namespace Engine

#endif
