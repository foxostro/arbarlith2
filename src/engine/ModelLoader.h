/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2007 Game Creation Society
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

#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

#include "tstring.h"
#include "AnimationController.h"

namespace Engine {

/** Generic model loader */
class ModelLoader
{
private:
	/** Stores previously loaded MD3 models */
	static map< string, AnimationController* > cache;

protected:
	/**
	Inserts the model into the cache
	@param fileName file name to identify the model
	@param controller animated model to cache
	*/
	static void insertInCache(const string &fileName, AnimationController *controller);

	/**
	Gets the model from the cache
	@param fileName file name to identify the model
	@return animated model retrieved from the cache
	*/
	static AnimationController* getFromCache(const string &fileName);

	/**
	Loads a model from file
	@param fileName The file name of the model
	*/
	virtual AnimationController* loadFromFile(const string &fileName) const = 0;

public:
	/** Destructor */
	virtual ~ModelLoader(void) {}

	/**
	Loads a model from file or the cache
	@param fileName The file name of the model
	*/
	AnimationController* load(const string &fileName);
};

} // namespace Engine

#endif
