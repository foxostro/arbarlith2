/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009 Game Creation Society
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

#ifndef _FILE_SELECTION_LIST_H_
#define _FILE_SELECTION_LIST_H_

#include "SelectionList.h"


namespace Engine {



/** A SelectionList that generates its elements from a list of file names */
class FileSelectionList : public SelectionList
{
private:
    void add(const string &label);

public:
	/**
	Constructs the list widget
	@param searchDirectory Directory to search within
	@param fileExtension File extension to search for
	@param x The X-Coordinate of the list widget
	@param y The Y-Coordinate of the list widget
	*/
	FileSelectionList(const string &searchDirectory,
                      const string &fileExtension,
                      float x, float y);

	/**
	Gets the file name from the currently selected element
	@return the filename
	*/
	string getFilename(void);

protected:
	/**
	Updates the widget
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);
};

} // namespace Engine

#endif
