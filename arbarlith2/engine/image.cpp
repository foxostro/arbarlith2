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
#include "file.h"
#include "profile.h"
#include "image.h"

/*
Include the DevIL headers
*/
#include <IL/il.h>
#include <IL/ilu.h>

namespace Engine {



Image::Image(void)
:imageName(0)
{}

Image::Image(const _tstring &fileName)
:imageName(0)
{
	load(fileName);
}

Image::~Image()
{
	// Free the image from DevIL
	ilDeleteImages(1, &imageName);
}

bool Image::load(const _tstring &_fileName)
{
	PROFILE

	const _tstring fileName = File::fixFilename(pathAppend(getWorkingDirectory(), _fileName));

	// anything to do?
	if(fileName.empty())
	{
		FAIL(_T("Cannot load image when filename is empty!"));
		return false;
	}

	// Free the old image from DevIL
	ilDeleteImages(1, &imageName);

	// Load the image with DevIL
	ilGenImages(1, &imageName);
	ilBindImage(imageName);


	char *s = toAnsiCharArray(fileName);

	if(!ilLoadImage(s))
	{
		ILenum err;
		while((err=ilGetError()) != IL_NO_ERROR)
		{
			ERR(_tstring(_T("IL Error: ")) + itoa(err));
		}

		delete[] s;

		FAIL(_tstring(_T("Failed to load image: ")) + fileName);

		return false;
	}

	if(ilGetInteger(IL_ORIGIN_MODE) == IL_ORIGIN_LOWER_LEFT)
	{
		iluFlipImage();
	}

	this->fileName = fileName;

	delete[] s;

	return true;
}

int Image::getWidth(void) const
{
	ilBindImage(imageName);
	return ilGetInteger(IL_IMAGE_WIDTH);
}

int Image::getHeight(void) const
{
	ilBindImage(imageName);
	return ilGetInteger(IL_IMAGE_HEIGHT);
}

int Image::getDepth(void) const
{
	ilBindImage(imageName);
	return ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
}

unsigned char* Image::getImage(void) const
{
	ilBindImage(imageName);
	unsigned char *data = ilGetData();
	return data;
}

} // namespace Engine
