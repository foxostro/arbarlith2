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
#include "SDLwindow.h"
#include "BackdropTexture.h"



namespace Engine {




BackdropTexture::BackdropTexture(void)
{
	clear();
}

BackdropTexture::~BackdropTexture(void)
{}

void BackdropTexture::clear(void)
{
	texture_640x480 = 0;
	texture_800x600 = 0;
	texture_1024x768 = 0;
	texture_1280x768 = 0;
}

void BackdropTexture::destroy(void)
{
	delete texture_640x480;
	delete texture_800x600;
	delete texture_1024x768;
	delete texture_1280x768;

	clear();
}

void BackdropTexture::create(const string &imageFileName_640x480,
							 const string &imageFileName_800x600,
							 const string &imageFileName_1024x768,
							 const string &imageFileName_1280x768)
{
	destroy();

	if(false==imageFileName_640x480.empty())
	{
		texture_640x480 = new Material;
		texture_640x480->loadTexture(imageFileName_640x480, 0);
	}


	if(false==imageFileName_800x600.empty())
	{
		texture_800x600 = new Material;
		texture_800x600->loadTexture(imageFileName_800x600, 0);
	}


	if(false==imageFileName_1024x768.empty())
	{
		texture_1024x768 = new Material;
		texture_1024x768->loadTexture(imageFileName_1024x768, 0);
	}


	if(false==imageFileName_1280x768.empty())
	{
		texture_1280x768 = new Material;
		texture_1280x768->loadTexture(imageFileName_1280x768, 0);
	}
}

Material& BackdropTexture::getTexture(void)
{
	switch(g_Window.GetWidth())
	{
	case 640:
		if(texture_640x480==0)
			return(*texture_800x600);
		else
			return(*texture_640x480);

	case 1024:
		if(texture_1024x768==0)
			return(*texture_800x600);
		else
			return(*texture_1024x768);

	case 1280:
		if(texture_1280x768==0)
			return(*texture_800x600);
		else
			return(*texture_1280x768);

	case 800:
	default: return(*texture_800x600);
	};
}

}; // namespace
