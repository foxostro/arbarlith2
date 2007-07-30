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

#ifndef _TILE_H_
#define _TILE_H_

#include "BoundingBox.h"
#include "material.h"

namespace Engine {

// prototype
class Map;



/** Describes the properties of the tile */
typedef int TILE_PROPERTIES;

/** Tile property bitflag: The tile is impassable (i.e. A wall is impassable) */
const TILE_PROPERTIES TILE_PROPERTY_IMPASSABLE = 128; // 2^7



/** Describes the type of the Tile */
typedef int TILE_TYPE;

/** Tile type token: The tile is a block. Depending on tile height, it can be a WALL, FLOOR, or HOLE */
const TILE_TYPE TILE_BLOCK = 0;

/** Tile is not to be rendered at all */
const TILE_TYPE TILE_EMPTY = 3;




/** Handle to identify a material in the map */
typedef int MAP_MATERIAL_ID;



/** Represents the data necessary to construct a Tile in the game map. */
class Tile
{
public:
	/**
	Construct the tile
	@param grid tile grid
	@param x Tile X
	@param z Tile Z
	*/
	Tile(void);

	/**
	Draws the Tile.
	@param x		The world space x coordinate of the Tile
	@param y		The world space y coordinate of the Tile
	*/
	void draw(float x, float z) const;

	/**
	Determines whether the Tile is passable or unpassable.
	@return true if the Tile is passable, false otherwise
	*/
	bool isPassable(void) const
	{
		return (propertiesBitmap & TILE_PROPERTY_IMPASSABLE)==0;
	}

	/**
	Modifies the passability flag on the tile
	@param passable true if the tile is to be passable
	@return true if the tile becomes passable
	*/
	bool setPassable(bool passable);

	/**
	Gets the type of the Tile
	@return The type of the Tile
	*/
	TILE_TYPE getType(void) const
	{
		return type;
	}

	/**
	Gets the tile height, specified in meters
	@return tile height, specified in meters
	*/
	float getTileHeight(void) const
	{
		return boundingBox.m_Max.y;
	}

	/**
	Gets a pointer to the tile height for the explicit purpose of allowing it to modified arbitrarily
	@return Pointer to the tile height
	*/
	float* getModifiableTileHeight(void)
	{
		return &boundingBox.m_Max.y;
	}

	/**
	Constructs the Tile from data contained in a binary file.
	@param x Tile x
	@param z Tile z
	@param file File is seek'd to the beginning of the tile data
	@param owner Map that owns the tile
	*/
	void load(int x, int z, File &file, Map &owner);

	/**
	Saves the Tile to a binary file
	@param file File is seek'd to the beginning of the tile data
	*/
	void save(File &file);

	/**
	Constructs a tile with the specified characteristics
	@param x Tile x
	@param z Tile z
	@param type Indicates the type of the Tile
	@param propertiesBitmap Flags to indicate tile properties
	@param tileHeight The height of a tile, specified in meters
	@param floorMaterialMatID Material for the ceiling
	@param wallMaterialMatID Material for the tile
	@param owner Map that owns this Tile
	*/
	void create(int x, int z, TILE_TYPE type, TILE_PROPERTIES propertiesBitmap, float tileHeight, MAP_MATERIAL_ID floorMaterialMatID, MAP_MATERIAL_ID wallMaterialMatID, Map &owner);

	/**
	Constructs a tile with the specified characteristics
	@param x Tile x
	@param z Tile z
	@param type Indicates the type of the Tile
	@param propertiesBitmap Flags to indicate tile properties
	@param tileHeight The height of a tile, specified in meters
	@param floorFileName Material for the ceiling
	@param wallFileName Material for the tile
	@param owner Map that owns this Tile
	*/
	void create(int x, int z, TILE_TYPE type, TILE_PROPERTIES propertiesBitmap, float tileHeight, const _tstring &floorFileName, const _tstring &wallFileName, Map &owner);

private:
	/** Tile type */
	TILE_TYPE type;

	/** Flags to indicate tile properties */
	TILE_PROPERTIES propertiesBitmap;

	/** Describes the material that is to be used to texture the Tile */
	MAP_MATERIAL_ID wallMaterialMatID;

	/** Describes the material to use for ceiling on the Tile */
	MAP_MATERIAL_ID floorMaterialMatID;

	/** Describes the material that is to be used to texture the Tile */
	const Material *wallMat;

	/** Describes the material to use for floor/top on the Tile */
	const Material *floorMat;

	/** Bounding box for the node */
	BoundingBox boundingBox;

	Tile *north;
	Tile *south;
	Tile *east;
	Tile *west;

	/**
	Initializes the bounding box for the node
	@param width Node width
	@param height Node height
	*/
	void setupBoundingBox(float width, float height);

	/**
	Sets up neighboring tiles
	@param owner the map that owns the tiles
	@param x Tile x
	@param z Tile z
	*/
	void setupNeighbors(Map &owner, int x, int z);

	/**
	Draws the walls of a block-type tile
	@param north	The northern tile.
	@param south	The southern tile.
	@param east		The eastern tile.
	@param west		The western tile.
	*/
	void drawWalls(const Tile *north, const Tile *south, const Tile *east, const Tile *west) const;

	/**
	Draws the floor of a block-type tile
	@param north	The northern tile.
	@param south	The southern tile.
	@param east		The eastern tile.
	@param west		The western tile.
	*/
	void drawFloor(const Tile *north, const Tile *south, const Tile *east, const Tile *west) const;

	/**
	Draws the walls of a block-type tile
	@param north	The northern tile.
	@param south	The southern tile.
	@param east		The eastern tile.
	@param west		The western tile.
	*/
	void drawBlockTypeTile(const Tile *north, const Tile *south, const Tile *east, const Tile *west) const;

	/**
	Should we draw a wall between the border to the tile here?
	@param x Tile on the border
	*/
	inline bool SHOULD_DRAW_WALL(const Tile *neighbor) const
	{
		return (neighbor==0) ||
			   (neighbor!=0 && neighbor->getType()!=TILE_BLOCK) ||
			   (neighbor!=0 && neighbor->getType()==TILE_BLOCK && fabsf(neighbor->getTileHeight()-this->getTileHeight())>FLT_EPSILON);
	}
};

} // namespace Engine

#endif
