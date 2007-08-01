/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2005-2007 Game Creation Society
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
#include "fog.h"


namespace Engine {



Fog::Fog(void)
: density(1),
  fnear(1),
  ffar(60),
  red(0),
  green(0),
  blue(0),
  shouldBeUsed(true)
{}

PropertyBag Fog::save(void) const
{
	PropertyBag xml;

	// Load the data
	xml.add("near",    fnear);
	xml.add("far",     ffar);
	xml.add("red",     red);
	xml.add("green",   green);
	xml.add("blue",    blue);
	xml.add("enabled", shouldBeUsed);

	return xml;
}

bool Fog::load(const PropertyBag &xml)
{
	// Load the fog settings
	xml.get("enabled", shouldBeUsed);
	xml.get("near",    fnear);
	xml.get("far",     ffar);
	xml.get("red",     red);
	xml.get("green",   green);
	xml.get("blue",    blue);

	OpenGL::GetSingleton().SetClippingPlanes(0.01f, ffar);

	return true;
}

void Fog::activate(void) const
{
	if(shouldBeUsed)
	{
		// set the fog type
		glFogi(GL_FOG_MODE, GL_LINEAR);

		// set fog color
		float color[] = {red, green, blue, 1.0f};
		glFogfv(GL_FOG_COLOR, color);
		glClearColor(red, green, blue, 1.0f);

		// set density
		glFogf(GL_FOG_DENSITY,density);

		// set extents
		glFogf(GL_FOG_START, fnear);
		glFogf(GL_FOG_END, ffar);
		OpenGL::GetSingleton().SetClippingPlanes(0.01f, ffar);

		// set hint
		glHint(GL_FOG_HINT, GL_NICEST);

		// and switch on fog
		glEnable(GL_FOG);

	}
}

void Fog::deactivate(void) const
{
	glDisable(GL_FOG);
}


}; // namespace
