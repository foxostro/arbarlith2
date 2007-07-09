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

#ifndef _IMAGE_H_
#define _IMAGE_H_


namespace Engine {


/**
Wrapper class for the DevIL image loading library
*/
class Image
{
private:
	/** DevIL image name */
	unsigned int imageName;

	/** File name */
	_tstring fileName;

public:
	/**
	Default contructor
	*/
	Image(void);

	/**
	Loads an image on construction
	@param fileName The image to load
	*/
	Image(const _tstring &fileName);

	/**
	Destructor
	*/
	~Image(void);

	/**
	Loads an image
	@param fileName The image to load
	@return true if the image was loaded, false otherwise
	*/
	bool load(const _tstring &fileName);

	/**
	Get the width of the image
	@return width
	*/
	int getWidth(void) const;

	/**
	Get the height of the image
	@return height
	*/
	int getHeight(void) const;

	/**
	Get the depth of the image
	@return depth
	*/
	int getDepth(void) const;

	/**
	Gets the DevIL image name of the image
	@return DevIL image name
	*/
	unsigned int getImageName(void) const
	{
		return imageName;
	}

	/**
	Gets the file name of the image
	@return file name
	*/
	_tstring getFileName(void) const
	{
		return fileName;
	}

	/**
	Gets the image buffer
	@return image buffer
	*/
	unsigned char *getImage(void) const;
};

} //namespace Engine

#endif
