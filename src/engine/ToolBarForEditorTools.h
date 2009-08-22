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

#ifndef _ToolBarForEditorTools_H_
#define _ToolBarForEditorTools_H_

#include "SelectionList.h"



namespace Engine {


class ToolBarForEditorTools : public SelectionList
{
public:
	/** Names of the various editor mode tools */
	enum EDITOR_TOOLS
	{
		EDITOR_DESTROY_TOOL=2,
		EDITOR_CREATE_TOOL,
		EDITOR_ROTATE_TOOL,
		EDITOR_ROTATE_X_TOOL,
		EDITOR_ROTATE_Z_TOOL,
		EDITOR_MOVE_TOOL,
		EDITOR_SELECT_TOOL,
		EDITOR_TILE_PENCIL_TOOL,
		EDITOR_TILE_BLOCK_TOOL,
	};

private:
	/** This indicates the currently selected tool on the toolbar */
	EDITOR_TOOLS currentTool;

public:
	/**
	Constructor
	@param x X
	@param y Y
	*/
	ToolBarForEditorTools(float x, float y);

	/** destructor */
	virtual ~ToolBarForEditorTools(void);

	/**
	Gets the currently selected tool
	@return The currently selected tool
	*/
	EDITOR_TOOLS getTool(void) const
	{
		return currentTool;
	}

	/**
	Determines what the selected tool should be
	@return the selected tool
	*/
	EDITOR_TOOLS chooseSelectedTool(void);

	/**
	Updates the tool bar
	@param deltaTime Time since the last update
	*/
	void ProcessTree(float deltaTime);
};


}; // namespace


#endif
