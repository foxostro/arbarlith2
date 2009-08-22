/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006,2007,2009 Game Creation Society
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
#include "ToolBarForEditorTools.h"
#include "ListElementWidget.h"
#include "ToggleWidget.h"


namespace Engine {



ToolBarForEditorTools::ToolBarForEditorTools(float x, float y)
:SelectionList(x, y)
{
	ListElementWidget *pencilTool = new ToggleWidget(new bool(true), "data/sprites/list/editor/pencil_on.png",	"data/sprites/list/editor/pencil_on_over.png",	"data/sprites/list/editor/pencil_off.png",	"data/sprites/list/editor/pencil_off_over.png");
	pencilTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_TILE_PENCIL_TOOL);
	addSelectableElement(pencilTool);

	ListElementWidget *blockTool = new ToggleWidget(new bool(true), "data/sprites/list/editor/block_on.png",	"data/sprites/list/editor/block_on_over.png",	"data/sprites/list/editor/block_off.png",	"data/sprites/list/editor/block_off_over.png");
	blockTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_TILE_BLOCK_TOOL);
	addSelectableElement(blockTool);

	ListElementWidget *destroyTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/destroy_on.png",	"data/sprites/list/editor/destroy_on_over.png",	"data/sprites/list/editor/destroy_off.png",	"data/sprites/list/editor/destroy_off_over.png");
	destroyTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_DESTROY_TOOL);
	addSelectableElement(destroyTool);

	ListElementWidget *createTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/create_on.png",	"data/sprites/list/editor/create_on_over.png",	"data/sprites/list/editor/create_off.png",	"data/sprites/list/editor/create_off_over.png");
	createTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_CREATE_TOOL);
	addSelectableElement(createTool);

	ListElementWidget *rotateTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/rotate_on_y.png",	"data/sprites/list/editor/rotate_on_over_y.png",	"data/sprites/list/editor/rotate_off_y.png",	"data/sprites/list/editor/rotate_off_over_y.png");
	rotateTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_TOOL);
	addSelectableElement(rotateTool);

	ListElementWidget *rotateXTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/rotate_on_x.png",	"data/sprites/list/editor/rotate_on_over_x.png",	"data/sprites/list/editor/rotate_off_x.png",	"data/sprites/list/editor/rotate_off_over_x.png");
	rotateXTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_X_TOOL);
	addSelectableElement(rotateXTool);

	ListElementWidget *rotateZTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/rotate_on_z.png",	"data/sprites/list/editor/rotate_on_over_z.png",	"data/sprites/list/editor/rotate_off_z.png",	"data/sprites/list/editor/rotate_off_over_z.png");
	rotateZTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_Z_TOOL);
	addSelectableElement(rotateZTool);

	ListElementWidget *moveTool = new ToggleWidget(new bool(false), "data/sprites/list/editor/move_on.png",		"data/sprites/list/editor/move_on_over.png",	"data/sprites/list/editor/move_off.png",	"data/sprites/list/editor/move_off_over.png");
	moveTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_MOVE_TOOL);
	addSelectableElement(moveTool);

	ListElementWidget *selectTool = new ToggleWidget(new bool(true), "data/sprites/list/editor/select_on.png",	"data/sprites/list/editor/select_on_over.png",	"data/sprites/list/editor/select_off.png",	"data/sprites/list/editor/select_off_over.png");
	selectTool->widgetName = STRINGIFY(ToolBarForEditorTools::EDITOR_SELECT_TOOL);
	addSelectableElement(selectTool);

	selected = selectTool;
	currentTool = ToolBarForEditorTools::EDITOR_SELECT_TOOL;
	m_bVisible = false;
}

ToolBarForEditorTools::~ToolBarForEditorTools(void)
{}

void ToolBarForEditorTools::ProcessTree(float deltaTime)
{
	std::list<  ListElementWidget*  >::iterator iter;

	// Determine the selected tool
	chooseSelectedTool();

	// Un-Highlight all tools
	for(iter = selectableElements.begin();
		iter!=selectableElements.end();
		++iter)
	{
		ToggleWidget *widget = dynamic_cast<ToggleWidget*>(*iter);

		if(widget)
		{
			widget->setValue(false);
		}
	}

	// Make sure the selected tool is highlighted
	if(selected)
	{
		ToggleWidget *widget = dynamic_cast<ToggleWidget*>(selected);

		if(widget)
		{
			widget->setValue(true);
		}
	}

	// Process the super class
	SelectionList::ProcessTree(deltaTime);
}

ToolBarForEditorTools::EDITOR_TOOLS ToolBarForEditorTools::chooseSelectedTool(void)
{
	ListElementWidget *selected = getSelected();

	if(selected)
	{
		if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_DESTROY_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_DESTROY_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_CREATE_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_CREATE_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_ROTATE_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_X_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_ROTATE_X_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_ROTATE_Z_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_ROTATE_Z_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_MOVE_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_MOVE_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_SELECT_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_SELECT_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_DESTROY_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_DESTROY_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_TILE_BLOCK_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_TILE_BLOCK_TOOL;
		}
		else if(selected->widgetName == STRINGIFY(ToolBarForEditorTools::EDITOR_TILE_PENCIL_TOOL))
		{
			currentTool = ToolBarForEditorTools::EDITOR_TILE_PENCIL_TOOL;
		}
	}

	return currentTool;
}

} // namespace Engine
