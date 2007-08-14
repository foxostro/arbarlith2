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
#include "3dmath.h"
#include "SDLwindow.h"
#include "Application.h"

#include "player.h"
#include "world.h"
#include "Map.h"

#include "EditorToolBar.h"
#include "ListElementTweaker.h"
#include "ListElementLabel.h"
#include "ToggleWidget.h"
#include "ToggleWidgetText.h"

namespace Engine {



vec3 UnProject(float depth); // stdafx.cpp

void EditorToolBar::clear(void)
{
	world = 0;
	tileEditor_type = TILE_BLOCK;
	tileEditor_properties = 0;
	tileEditor_floorTextureFile = "data/tiles/floor/floor.jpg";
	tileEditor_wallTextureFile = "data/tiles/floor/wall.JPG";
	tileEditor_height = 0.0f;
	shouldSave = false;
	shouldLoad = false;
	shouldNew = false;
	snapToGrid = false;
	leftClickDebounce = false;
	nextObject = "nill";
	selected = 0;
	toolBarTools = 0;
	toolBarMisc = 0;
	toolBarTilePropreties = 0;
	toolBarZone = 0;
	objectPalette = 0;
	texturePalette_Wall = 0;
	texturePalette_Floor = 0;
	tileTypeSelector = 0;
	tileHeightSelector = 0;
	mousePosLabel = 0;
	mouseDownPos = vec3(0, 0, 0);
	drag = false;
	textureSelectorState = TEXTURE_SELECTOR_HIDE;
	actorProperties = 0;

	TRACE("EditorToolBar reset");
}

void EditorToolBar::destroy(void)
{
	Widget::destroy();

	clear();
}

void EditorToolBar::create(World *_world)
{
// Get rid of everything old
	destroy();

// Attach to the game world
	setWorld(_world);

// Create the World properties panel
	toolBarZone = new ListPaneWidget(0, 10);
	toolBarZone->m_bVisible = false;
	AddChild(toolBarZone);

// Populate the World properties panel
	toolBarZone->addElement(new ListElementTweaker<float>( "Ambient Light [0,1]", &(world->getLightManager().ambientLight)  ));
	toolBarZone->addElement(new ListElementTweakerString(  "World Name",          &(world->name)  ));

// Create the Tools panel
	toolBarTools = new ToolBarForEditorTools(0, 160);
	toolBarTools->m_bVisible = false;
	AddChild(toolBarTools);

// Create the misc toolbar
	toolBarMisc = new ListPaneWidget(64, 160);
	toolBarMisc->m_bVisible = false;
	AddChild(toolBarMisc);

// Populate the misc toolbar
	toolBarMisc->addElement(new ToggleWidget(&snapToGrid,
	                                         "data/sprites/list/editor/snap_on.png",
	                                         "data/sprites/list/editor/snap_off.png"));

	toolBarMisc->addElement(new ToggleWidget(&shouldSave,
	                                         "data/sprites/list/editor/save_down.png",
	                                         "data/sprites/list/editor/save_up.png"));

	toolBarMisc->addElement(new ToggleWidget(&shouldLoad,
	                                         "data/sprites/list/editor/load_down.png",
	                                         "data/sprites/list/editor/load_up.png"));

	toolBarMisc->addElement(new ToggleWidget(&shouldNew,
	                                         "data/sprites/list/editor/new_on.png",
	                                         "data/sprites/list/editor/new_on_over.png",
	                                         "data/sprites/list/editor/new_off.png",
	                                         "data/sprites/list/editor/new_off_over.png"));

	// Create some controls for the map editor
	createObjectPalette();
	createWallTextureSelector();
	createFloorTextureSelector();
	createTileTypeSelector();
	createTileHeightSelector();

// Create the tile properties page
	toolBarTilePropreties = new ListPaneWidget(1024-341, 235);
	toolBarTilePropreties->m_bVisible = false;
	AddChild(toolBarTilePropreties);

	toolBarTilePropreties->addElement(new CycleTextureSelectorState(&textureSelectorState));
	toolBarTilePropreties->addElement(new ToggleWidgetText						("Select Type",		&(tileTypeSelector->m_bVisible)));
	//toolBarTilePropreties->addElement(new ToggleWidgetText					("Select Height",	&(tileHeightSelector->m_bVisible)));
	toolBarTilePropreties->addElement(new ListElementTweaker<TILE_PROPERTIES>	("Tile Properties",	&tileEditor_properties));
	toolBarTilePropreties->addElement(new ListElementTweaker<float>				("Tile Height",		&tileEditor_height));

// Create the mouse cursor label
	mousePosLabel = new LabelWidget("label here", vec2(700,700));
	AddChild(mousePosLabel);

// Create the actor properties pane
	actorProperties	= new ListPaneWidget(400, 400);
	AddChild(actorProperties);
	hideActorPane(); // initially hidden
}

void EditorToolBar::createObjectPalette(void)
{
	objectPalette = new FileSelectionList("data/objects/", ".xml", 80+64, 100);
	objectPalette->m_bVisible = false;
	AddChild(objectPalette);
}

void EditorToolBar::createWallTextureSelector(void)
{
	texturePalette_Wall = new FileSelectionList("data/tiles/wall/", ".jpg", 320, 100);
	texturePalette_Wall->m_bVisible = false;
	AddChild(texturePalette_Wall);
}

void EditorToolBar::createFloorTextureSelector(void)
{
	texturePalette_Floor = new FileSelectionList("data/tiles/floor/", ".jpg", 320, 100);
	texturePalette_Floor->m_bVisible = false;
	AddChild(texturePalette_Floor);
}

void EditorToolBar::createTileTypeSelector(void)
{
	tileTypeSelector = new StringSelectionList<TILE_TYPE>(1024-341, 10);
	tileTypeSelector->m_bVisible = false;
	tileTypeSelector->addElement("Empty", TILE_EMPTY);
	tileTypeSelector->addElement("Block", TILE_BLOCK);
	AddChild(tileTypeSelector);
}

void EditorToolBar::createTileHeightSelector(void)
{
	tileHeightSelector = new StringSelectionList<float>(1024-341, 10);
	tileHeightSelector->m_bVisible = false;
	tileHeightSelector->addElement("-5.0", -5.0f);
	tileHeightSelector->addElement("-1.0", -1.0f);
	tileHeightSelector->addElement("0.0", 0.0f);
	tileHeightSelector->addElement("1.0", 1.0f);
	tileHeightSelector->addElement("2.4", 2.4f);
	tileHeightSelector->addElement("0.5", 0.5f);
	tileHeightSelector->addElement("5.0", 5.0f);
	AddChild(tileHeightSelector);
}

void EditorToolBar::draw(void) const
{}

void EditorToolBar::drawInWorldSpace(void) const
{
	vec3 up = vec3(0,1,0);
	vec3 a = mousePos + up*4.0f;
	vec3 b = mousePos - up*4.0f;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(3);
		glColor4fv(red);

		glBegin(GL_LINES);
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
		glEnd();

	glPopAttrib();
}

TILE_TYPE EditorToolBar::getTileType(void) const
{
	ASSERT(tileTypeSelector!=0, "tileTypeSelector was null");
	return tileTypeSelector->getValue();
}

void EditorToolBar::update(float deltaTime)
{
	ASSERT(world!=0,                 "world was null!  Call setWorld first!");
	ASSERT(toolBarTools!=0,          "toolBarTools was null");
	ASSERT(toolBarMisc!=0,           "toolBarMisc was null");
	ASSERT(toolBarZone!=0,           "toolBarZone was null");
	ASSERT(mousePosLabel!=0,         "mousePosLabel was null");
	ASSERT(objectPalette!=0,         "objectPalette was null");
	ASSERT(tileTypeSelector!=0,      "tileTypeSelector was null");
	ASSERT(texturePalette_Wall!=0,   "texturePalette_Wall was null");
	ASSERT(texturePalette_Floor!=0,  "texturePalette_Floor was null");
	ASSERT(tileHeightSelector!=0,    "tileHeightSelector was null");




	if(g_Application.getState() == GAME_STATE_EDITOR)
	{
		toolBarTools->m_bVisible = true;
		toolBarMisc->m_bVisible = true;
		toolBarZone->m_bVisible = true;
		mousePosLabel->m_bVisible = true;



		// Hide the object palette (overridden by create tool)
		objectPalette->m_bVisible = false;

		// Hide the tile properties panel (overridden in map editor mode)
		toolBarTilePropreties->m_bVisible = false;


		// Get the point under the mouse cursor
		mousePos = getGroundPickPos(0.0f);

		// update the mouse cursor to show this position
		mousePosLabel->setLabel(string("Mouse: (") + ftoa(mousePos.x) +
								string(", ") + ftoa(mousePos.y) +
								string(", ") + ftoa(mousePos.z) +
								string(")"));

        if(g_Keys.isKeyDown(KEY_TEST))
        {
            world->getMap().removeAllMaterials();
        }


		// Update the tool bar widgets
		switch(toolBarTools->getTool())
		{
		case ToolBarForEditorTools::EDITOR_CREATE_TOOL:
			// Show the object palette
			objectPalette->m_bVisible = true;

			// Decide the selected object
			{
				string o = chooseNextObject();
				if(!o.empty())
				{
					nextObject = o;
				}
			}
			break;

		case ToolBarForEditorTools::EDITOR_TILE_PENCIL_TOOL:
			toolBarTilePropreties->m_bVisible = true;

			// Update tile properties from the selectors
			tileEditor_wallTextureFile = chooseTileWallTexture();
			tileEditor_floorTextureFile = chooseTileFloorTexture();
			tileEditor_type = getTileType();

			break;

		case ToolBarForEditorTools::EDITOR_TILE_BLOCK_TOOL:
			toolBarTilePropreties->m_bVisible = true;

			// Update tile properties from the selectors
			tileEditor_wallTextureFile = chooseTileWallTexture();
			tileEditor_floorTextureFile = chooseTileFloorTexture();
			tileEditor_type = getTileType();

			// Has a drag stopped?
			if(!g_Input.MouseLeft)
			{
				// drag exitted
				if(drag)
				{
					// get a reference to the map
					Map &map = world->getMap();

					// Get the position on the ground plane that the mouse was over
					vec3 groundPos = getGroundPickPos(0.0f);

					// Fill a block of tiles
					if(map.onATile(groundPos.x, groundPos.z) && map.onATile(mouseDownPos.x, mouseDownPos.z))
					{
						map.fillBlock(groundPos.x, groundPos.z,
									  mouseDownPos.x, mouseDownPos.z,
							          tileEditor_type,
									  tileEditor_properties,
									  tileEditor_floorTextureFile,
									  tileEditor_wallTextureFile,
							          tileEditor_height);

						// Rebuild the map display list
						map.reaquire();

						g_SoundSystem.play("data/sound/activate.wav");
					}
				}

				drag = false;
			}

			break;

		case ToolBarForEditorTools::EDITOR_DESTROY_TOOL:
			// (This tool doesn't require updating every tick)
			break;

		case ToolBarForEditorTools::EDITOR_ROTATE_TOOL:
			// (This tool doesn't require updating every tick)
			break;

		case ToolBarForEditorTools::EDITOR_ROTATE_X_TOOL:
			// (This tool doesn't require updating every tick)
			break;

		case ToolBarForEditorTools::EDITOR_ROTATE_Z_TOOL:
			// (This tool doesn't require updating every tick)
			break;

		case ToolBarForEditorTools::EDITOR_MOVE_TOOL:
			// (This tool doesn't require updating every tick)
			break;

		case ToolBarForEditorTools::EDITOR_SELECT_TOOL:
			// (This tool doesn't require updating every tick)
			break;
		};




		// The tile selectors cannot be opened if the tile properties dialog is not
		if(toolBarTilePropreties->m_bVisible)
		{
			switch(textureSelectorState)
			{
			case TEXTURE_SELECTOR_HIDE:
				texturePalette_Wall->m_bVisible = false;
				texturePalette_Floor->m_bVisible = false;
				break;

			case TEXTURE_SELECTOR_FLOOR:
				texturePalette_Wall->m_bVisible = false;
				texturePalette_Floor->m_bVisible = true;
				break;

			case TEXTURE_SELECTOR_WALL:
				texturePalette_Wall->m_bVisible = true;
				texturePalette_Floor->m_bVisible = false;
				break;
			};
		}
		else
		{
			tileTypeSelector->m_bVisible = false;
			tileHeightSelector->m_bVisible = false;
			textureSelectorState = TEXTURE_SELECTOR_HIDE;
			texturePalette_Wall->m_bVisible = false;
			texturePalette_Floor->m_bVisible = false;
		}




		// In editor mode, only update the selected object
		if(selected) selected->updateForEditor(deltaTime);




		// Call the event handler for left-click
		if(g_Input.MouseLeft)
		{
			onLeftMouseDown();
		}
		else
		{
			leftClickDebounce=false;
		}

		// Save / Load the game
		if(shouldSave)
		{
			world->saveToFile();
		}
		else if(shouldLoad)
		{
			world->loadFromFile();
		}
		else if(shouldNew)
		{
			createNewMap();
		}
	}
	else
	{// None of these should be visible in game mode

		toolBarTools->m_bVisible = false;
		toolBarMisc->m_bVisible = false;
		mousePosLabel->m_bVisible = false;

		objectPalette->m_bVisible = false;
		toolBarZone->m_bVisible = false;

		// Hide tile properties dialogs
		toolBarTilePropreties->m_bVisible = false;
		tileTypeSelector->m_bVisible = false;
		tileHeightSelector->m_bVisible = false;
		texturePalette_Wall->m_bVisible = false;
		texturePalette_Floor->m_bVisible = false;
		tileHeightSelector->m_bVisible = false;
	}

	// reset each tick
	shouldSave = false;
	shouldLoad = false;
	shouldNew = false;
}

void EditorToolBar::createNewMap(void)
{
	ASSERT(world!=0, "world was null!  Call setWorld first!");

	ActorSet &objects = world->getObjects();
	Map &map = world->getMap();

	map.makeNewMap();

	// Delete all objects
	objects.destroy();

	// Add a player object
	PropertyBag newGame;
	newGame.loadFromFile("data/zone/World1.xml");
	world->reloadPlayers(newGame);
}

string EditorToolBar::chooseTileWallTexture(void)
{
	ASSERT(texturePalette_Wall!=0, "EditorToolBar::chooseTileWallTexture  ->  texturePalette_Wall was null");
	return pathAppend("data/tiles/wall", texturePalette_Wall->getFilename());
}

string EditorToolBar::chooseTileFloorTexture(void)
{
	ASSERT(texturePalette_Floor!=0, "EditorToolBar::chooseTileFloorTexture  ->  texturePalette_Floor was null");
	return pathAppend("data/tiles/floor", texturePalette_Floor->getFilename());
}

string EditorToolBar::chooseNextObject(void)
{
	ASSERT(objectPalette!=0, "EditorToolBar::chooseNextObject  ->  objectPalette was null");
	return objectPalette->getFilename();
}

vec3 EditorToolBar::getGroundPickPos(float elevation) const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	g_Camera.setCamera();




	vec3 groundPos;
	groundPos.zero();

	// Get the point on the ground plane that the mouse is hovering over.

	// Get a ray for the mouse
	vec3 mouseRay[] =
	{
		UnProject(0.0f),
		UnProject(1.0f),
	};

	// Get the location that the mouse ray intersect the ground plane at.
	// Check the mouse ray against the ground plane (XZ plane)
	float originDistance=0.0f;
	vec3 normal;

	// A polygon on the intersection plane
	vec3 a(0,elevation,0);
	vec3 b(0,elevation,1);
	vec3 c(1,elevation,0);
	vec3 vPoly[3] = {a, b, c};

	if(IntersectedPlane(vPoly, mouseRay, normal, originDistance) == true)
	{
		// Get the intersection point
		groundPos = IntersectionPoint(normal, mouseRay, (double)originDistance);

		// Snap to grid
		if(snapToGrid)
		{
			float snap = 0.25f;

			float snappedX = (snap) * floorf(groundPos.x / snap);
			float snappedZ = (snap) * floorf(groundPos.z / snap);

			groundPos.x = snappedX;
			groundPos.z = snappedZ;
		}
	}




	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();



	return groundPos;
}

void EditorToolBar::onLeftMouseDown()
{
	ASSERT(world!=0, "world was null!  Call setWorld first!");

	if(g_GUI.mouseOverSomeWidget) return;

	// Get a position on the ground beneath the cursor
	float e = selected ? selected->getPos().y : 0.0f;
	const vec3 groundPos = getGroundPickPos(e);

	// Grab the pool of objects we are working from
	ActorSet &objects = world->getObjects();

	// Get the id of the object under the mouse cursor
	OBJECT_ID id = objects.getClosest<Actor>(groundPos, 2.0f);

	// Process the action depending on the current tool
	switch(toolBarTools->getTool())
	{
	case ToolBarForEditorTools::EDITOR_SELECT_TOOL:
		if(objects.isMember(id))
		{
			Actor * p = &objects.get(id);
			showActorPane(p);
		}
		else
		{
			hideActorPane();
		}
		break;

	case ToolBarForEditorTools::EDITOR_MOVE_TOOL:
		{
			if(selected)
			{
				// Hold the right mouse button to slide objects along the y-axis
				if(g_Input.MouseRight)
				{
					vec3 delta = groundPos - selected->getPos();
					delta.y=0;
					float dist = delta.getMagnitude()*0.3f;

					// Place the object on this spot
					selected->Place(vec3(selected->getPos().x, dist, selected->getPos().z));
				}
				else
				{
					selected->Place(groundPos);
				}
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_ROTATE_TOOL:
		{
			if(selected)
			{
				const vec3 delta = vec3(selected->getPos().x-groundPos.x, 0, selected->getPos().z-groundPos.z);
				const vec3 zAxis = delta.getNormal();
				const vec3 yAxis = vec3(0,1,0);
				const vec3 xAxis = yAxis.cross(zAxis).getNormal();

				mat4 orientation = selected->getOrientation();
				orientation.setAxisZ(zAxis);
				orientation.setAxisY(yAxis);
				orientation.setAxisX(xAxis);
				orientation.setPos(vec3(0,0,0));
				selected->setOrientation(orientation);
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_ROTATE_X_TOOL:
		{
			if(selected)
			{
				vec3 delta = groundPos - selected->getPos();
				float angle = atan2f(delta.x, delta.y) * 0.1f;

				mat4 rot;
				rot.rotateX(angle);

		                mat4 orientation = selected->getOrientation();

				orientation *= rot;

				selected->setOrientation(orientation);
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_ROTATE_Z_TOOL:
		{
			if(selected)
			{
				vec3 delta = groundPos - selected->getPos();
				float angle = atan2f(delta.x, delta.y) * 0.1f;

				mat4 rot;
				rot.rotateZ(angle);

		                mat4 orientation = selected->getOrientation();

				orientation *= rot;

				selected->setOrientation(orientation);
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_CREATE_TOOL:
		if(!leftClickDebounce)
		{
			leftClickDebounce=true;

			// Create objects from a palette of available types
			string nextObject = getNextObject();

            // Gets the objects for the object palette
			string editorDataFile = pathAppend("data/objects/", nextObject);

			PropertyBag ThisObjBag;
            ThisObjBag.loadFromFile(editorDataFile);

			string rtti = ThisObjBag.getString("type");

			// Create the object inside the game world
			OBJECT_ID id = objects.create(rtti, world);
			if(id != INVALID_ID)
			{
				Actor &object = objects.get(id);
				object.load(ThisObjBag);
				object.editorDataFile = editorDataFile;

				// Do not affect object y with the move command
				vec3 groundPos = getGroundPickPos(0.0f);

				// Place the object on this spot
				object.Place(groundPos);

				// Select the new object
				selected = &object;
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_DESTROY_TOOL:
		if(objects.isMember(id))
		{
			objects.removeObjectNow(id);
			hideActorPane();
		}
		break;

	case ToolBarForEditorTools::EDITOR_TILE_PENCIL_TOOL:
		{
			// get a reference to the map
			Map &map =  world->getMap();

			// Get the position on the ground plane that was clicked
			vec3 groundPos = getGroundPickPos(0.0f);

			if(map.onATile(groundPos.x, groundPos.z))
			{
				// get the tile that was picked
				Tile &tile = map.getTile(groundPos.x, groundPos.z);

#if 1
                tile.create(map.tileX(groundPos.x),
							map.tileZ(groundPos.z),
							tileEditor_type,
							tileEditor_properties,
							tileEditor_height,
							tileEditor_floorTextureFile,
							tileEditor_wallTextureFile,
							map);
#else
                tile.setMaterials(tileEditor_wallTextureFile,
                                  tileEditor_floorTextureFile,
                                  map);
#endif

				// Rebuild the map display list
				map.reaquire();

				g_SoundSystem.play("data/sound/click.wav");
			}
		}
		break;

	case ToolBarForEditorTools::EDITOR_TILE_BLOCK_TOOL:
		{
			// Drag entered
			if(!drag)
			{
				drag = true;

				g_SoundSystem.play("data/sound/click.wav");

				mouseDownPos = getGroundPickPos(0.0f);
			}
		}
		break;
	};
}

OBJECT_ID EditorToolBar::getSelectedId(void) const
{
	return (selected==0) ? INVALID_ID : selected->m_ID;
}

void EditorToolBar::showActorPane(Actor *actor)
{
	// Set the tool bar position
	actorProperties->setRelativePos(500, 300);

	// Make the toolbar visible
	actorProperties->m_bVisible = true;

	// Destroy the contents of the tool bar
	actorProperties->destroy();

	// Have the selected actor recreate the pane
	(selected=actor)->createToolBar(actorProperties);
}

void EditorToolBar::hideActorPane(void)
{
	selected=0; // make sure no actor is selected
	actorProperties->m_bVisible = false;
}

void EditorToolBar::setWorld(World *world)
{
	ASSERT(world!=0, "world was null");
	this->world = world;
	TRACE("Set world called");
}

} // namespace Engine
