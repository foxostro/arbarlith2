/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _EDITOR_TOOL_BAR_H_
#define _EDITOR_TOOL_BAR_H_

#include "widget.h"
#include "ActorSet.h"

#include "FileSelectionList.h"
#include "Map.h"
#include "ToolBarForEditorTools.h"
#include "CycleTextureSelectorState.h"
#include "StringSelectionList.h"

namespace Engine {

/**
The editor tool bar will be displayed while the game is in Editor Mode.
It displays the available tools, options, and Editor Mode specific data.
*/
class EditorToolBar : public Widget
{
private:
	/** World currently bound to the editor */
	World *world;

	/** the current type for a new tile */
	TILE_TYPE tileEditor_type;

	/** the current properties for a new tile */
	TILE_PROPERTIES tileEditor_properties;

	/** the current texture image for a new tile's wall */
	string tileEditor_wallTextureFile;

	/** the current texture image for a new tile's floor */
	string tileEditor_floorTextureFile;

	/** the current height for a new tile */
	float tileEditor_height;

	/** when true, indicates we should save the map on the next tick */
	bool shouldSave;

	/** when true, indicates we should load the map on the next tick */
	bool shouldLoad;

	/** when true, indicates we should create a new map on the next tick */
	bool shouldNew;

	/** Indicates that snap-to-grid is enabled or not */
	bool snapToGrid;

	/** Keeps clicks from bouncing a lot */
	bool leftClickDebounce;

	/** The file name of the XML object selected for creation */
	string nextObject;

	/** One Actor may be selected in the object editor mode */
	Actor *selected;

	/** This tool bar is for selecting between the various editor tools */
	ToolBarForEditorTools *toolBarTools;

	/** This holds the misc contros like save, load, and the snap toggle */
	ListPaneWidget *toolBarMisc;

	/** This holds the tile properties widgets */
	ListPaneWidget *toolBarTilePropreties;

	/** This holds the secondary toolbar for World specific data */
	ListPaneWidget *toolBarZone;

	/** This holds the options for new objects */
	FileSelectionList *objectPalette;

	/** This holds the options for tile wall textures */
	FileSelectionList *texturePalette_Wall;

	/** This holds the options for tile floor textures */
	FileSelectionList *texturePalette_Floor;

	/** This holds the options for tile types */
	StringSelectionList<TILE_TYPE> *tileTypeSelector;

	/** This holds the options for tile height */
	StringSelectionList<float> *tileHeightSelector;

	/** Records the mouse position */
	LabelWidget *mousePosLabel;

	/** Tools that use a drag effect can use this to store the intersectoion of the mouse and the terrain when the mouse entered the drag state. */
	vec3 mouseDownPos;

	/** Mouse position at the last update */
	vec3 mousePos;

	/** Indicates that a drag state has been entered. */
	bool drag;

	/** State of the texture selector controls */
	TEXTURE_SELECTOR_STATE textureSelectorState;

	/** Properties menu for selected actors */
	ListPaneWidget *actorProperties;

private:
	/**
	Reads the SelectionList palette and decides which object has been selected to be created
	@return The file name of the XML object selected for creation
	*/
	string chooseNextObject(void);

	/**
	Reads the SelectionList palette and decides which texture has been selected for the wall
	@return The file name of the texture image
	*/
	string chooseTileWallTexture(void);

	/**
	Reads the SelectionList palette and decides which texture has been selected for the floor
	@return The file name of the texture image
	*/
	string chooseTileFloorTexture(void);

	/**
	Selects an actor and shows the actor pane
	@param actor the selected actor
	*/
	void showActorPane(Actor * actor);

	/** Createa a new map */
	void createNewMap(void);

	/** Creates the pane of tile height options */
	void createTileHeightSelector(void);

	/** Creates the pane of tile type options */
	void createTileTypeSelector(void);

	/** Creates the list of floor texture options */
	void createFloorTextureSelector(void);

	/** Creates the list of floor texture options */
	void createWallTextureSelector(void);

	/** Creates the list of options for new objects */
	void createObjectPalette(void);

protected:
	/**
	Update the tool bar widget every tick.  Inherited from Widget
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/**
	Render the tool bar widget.  Inherited from Widget
	@param deltaTime Milliseconds since the last update
	*/
	void draw(void) const;

	TILE_TYPE getTileType(void) const;

	/** Called each tick when in editor mode and the left mouse button is down */
	void onLeftMouseDown(void);

	/**
	Find the intersection between a mouse ray and the XZ plane at the specified elevation.
	@return The point on the ground plane that was picked
	*/
	vec3 getGroundPickPos(float ground) const;

public:
	/** Constructor */
	EditorToolBar(void)
	{
		clear();
	}

	/** Destructor */
	~EditorToolBar(void)
	{
		destroy();
	}

	/** Reset to a just-constructed state */
	void clear(void);

	/** Cleanly destroy and clear the tool bar */
	void destroy(void);

	/**
	Initialize the tool bar
	@param world Game world to which to attach the tool bar
	*/
	void create(World *world);

	/** Draw parts of the tool bar in world space */
	void drawInWorldSpace(void) const;

	/**
	Gets the currently selected object template
	@return The currently selected object template
	*/
	const string& getNextObject(void) const
	{
		return nextObject;
	}

	/**
	Gets the ID of the selected object
	@return The ID of the selected object, or INVALID_ID
	*/
	OBJECT_ID getSelectedId(void) const;

	/** Hides the actor pane */
	void hideActorPane(void);

	/**
	Gets the world currently bound to the editor
	@return world
	*/
	inline World& getWorld(void)
	{
		ASSERT(world!=0, "world was null");
		return *world;
	}

	/**
	Gets the world currently bound to the editor
	@return world
	*/
	inline const World& getWorld(void) const
	{
		ASSERT(world!=0, "world was null");
		return *world;
	}

	/**
	Sets the world currently bound to the editor
	@param world the world to bind the editor to
	*/
	void setWorld(World *world);
};

} // namespace Engine

#endif
