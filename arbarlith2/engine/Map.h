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

#ifndef _MAP_H_
#define _MAP_H_

#include "PropertyBag.h"
#include "Tile.h"
#include "QuadTreeNode.h"

namespace Engine {

/**
Manages a map made out of Tile objects.
Provides tools to edit the map graphically.
*/
class Map
{
private:
	/** The width of a tile, specified in meters */
	float tileMetersX;

	/** The width of the tile grid */
	int width;

	/** The height of the tile grid */
	int height;

	/**  Two dimensional array of tiles in the map */
	Tile *grid;

	/** Quad tree spatially organizes the tiles */
	QuadTreeNode *quadTree;

	/** all map material (index is the material ID) */
	vector<Material*> materialsLegend;

	/**
	Loads the complete materials legend from XML
	@param materialsLegend XML for the materials legend
	*/
	void loadMaterialsLegend(const PropertyBag &materialsLegend);

	/**
	Saves the material legend
	@return XML for the materials legend
	*/
	PropertyBag saveMaterialsLegend(void) const;

	/** Destroy the materials legend */
	void destroyMaterialsLegend(void);

public:
	/** Constructs an empty map */
	Map(void);

	/** Destructor */
	~Map(void);

	/** Resets a map that has already been cleanly destroyed */
	void clear(void);

	/** Cleanly destroys a map and then resets it */
	void destroy(void);

	/** Deletes the display list for the entire map */
	void release(void) const;

	/**
	Adds a material to the material legend
	@param materialID material ID code
	@return material
	*/
	const Material* getMapMaterial(MAP_MATERIAL_ID materialID) const;

	/**
	Adds a material to the material legend
	@param materialFileName The filename of the material
	@return material ID code
	*/
	MAP_MATERIAL_ID loadMapMaterial(const string &materialFileName);

	/**
	Creates a map from data in an XML source
	@param xml The XML source
	*/
	void create(const PropertyBag &xml);

	/**
	Saves the map
	@param xml The XML source
	@param zoneName Name of the World
	*/
	void save(PropertyBag &xml, const string &zoneName) const;

	/**
	Saves the map
	@return xml resultant XML element
	@param zoneName Name of the World
	*/
	PropertyBag save(const string &zoneName) const
	{
		PropertyBag xml;
		save(xml, zoneName);
		return xml;
	}

	/** Draws the map */
	void draw(void) const;

	/** Draws the map to the depth buffer to project shadows */
	void drawToDepthBuffer(void) const;

	/**
	Dtermines whether or not a position is within the tile map
	@param x The x-coordinate of the Tile, in tile-space
	@param z The z-coordinate of the Tile, in tile-space
	@return true if the positionis within the tile map
	*/
	inline bool onATile(int x, int z) const
	{
		return(x >= 0 && x < width && z >= 0 && z < height);
	}

	/**
	Determines whether or not a position is within the tile map
	@param x The x-coordinate of the Tile, in tile-space
	@param z The z-coordinate of the Tile, in tile-space
	@return true if the positionis within the tile map
	*/
	inline bool onATile(float x, float z) const
	{
		int tileX = (int)floorf(x / tileMetersX);
		int tileZ = (int)floorf(z / tileMetersX);
		return onATile(tileX, tileZ);
	}

	/**
	Gets a Tile from the map given its coordinates in tile-space
	@param x The x-coordinate of the Tile, in tile-space
	@param z The z-coordinate of the Tile, in tile-space
	@return a reference to the Tile
	*/
	inline Tile& getTile(int x, int z)
	{
		ASSERT(x >= 0 && x < width,  "x is invalid");
		ASSERT(z >= 0 && z < height, "z is invalid");

		return grid[z*width + x];
	}

	/**
	Gets a Tile from the map given its coordinates in tile-space
	@param x The x-coordinate of the Tile, in tile-space
	@param z The z-coordinate of the Tile, in tile-space
	@return a reference to the Tile
	*/
	inline const Tile& getTile(int x, int z) const
	{
		ASSERT(x >= 0 && x < width,  "x is invalid");
		ASSERT(z >= 0 && z < height, "z is invalid");

		return grid[z*width + x];
	}

	/**
	Gets a Tile from the map given its coordinates in world-space
	@param x The x-coordinate of the Tile, in world-space
	@param z The z-coordinate of the Tile, in world-space
	@return a reference to the Tile
	*/
	inline Tile &getTile(float x, float z)
	{
		int tileX = (int)floorf(x / tileMetersX);
		int tileZ = (int)floorf(z / tileMetersX);

		return getTile(tileX, tileZ);
	}

	/**
	Gets a Tile from the map given its coordinates in world-space
	@param x The x-coordinate of the Tile, in world-space
	@param z The z-coordinate of the Tile, in world-space
	@return a reference to the Tile
	*/
	inline const Tile &getTile(float x, float z) const
	{
		int tileX = (int)floorf(x / tileMetersX);
		int tileZ = (int)floorf(z / tileMetersX);

		return getTile(tileX, tileZ);
	}

	/**
	Gets the tile column that a coordinate lies on
	@param x X coord
	@return Tile x coordinate
	*/
	inline int tileX(float x) const
	{
		return (int)floorf(x / tileMetersX);
	}

	/**
	Gets the tile row that a coordinate lies on
	@param z Z coord
	@return Tile z coordinate
	*/
	inline int tileZ(float z) const
	{
		return (int)floorf(z / tileMetersX);
	}

	/**
	Fills the map with tiles of the given characteristics
	@param tileType The type of the tile as described according to the enumerated value
	@param floorFileName Specifies the file name of the texture to apply to the tile's floor
	@param wallFileName Specifies the file name of the texture to apply to the tile's walls
	@param tileHeight The height of te tiles, specified in meters
	*/
	void fill(TILE_TYPE tileType, TILE_PROPERTIES properties, const string &floorFileName, const string &wallFileName, float tileHeight);

	/**
	Fills a rectangle of the map with tiles of the given characteristics
	@param startX
	@param startZ
	@param endX
	@param endZ
	@param tileType The type of the tile as described according to the enumerated value
	@param floorFileName Specifies the file name of the texture to apply to the tile's floor
	@param wallFileName Specifies the file name of the texture to apply to the tile's walls
	@param tileHeight The height of te tiles, specified in meters
	*/
	inline void fillBlock(float startX, float startZ, float endX, float endZ, TILE_TYPE tileType, TILE_PROPERTIES properties, const string &floorFileName, const string &wallFileName, float tileHeight)
	{
		int sX = (int)floorf(startX / tileMetersX);
		int sZ = (int)floorf(startZ / tileMetersX);
		int eX = (int)floorf(endX / tileMetersX);
		int eZ = (int)floorf(endZ / tileMetersX);

		fillBlock(sX, sZ, eX, eZ, tileType, properties, floorFileName, wallFileName, tileHeight);
	}

	/**
	Fills a rectangle of the map with tiles of the given characteristics
	@param startX
	@param startZ
	@param endX
	@param endZ
	@param tileType The type of the tile as described according to the enumerated value
	@param floorFileName Specifies the file name of the texture to apply to the tile's floor
	@param wallFileName Specifies the file name of the texture to apply to the tile's walls
	@param tileHeight The height of te tiles, specified in meters
	*/
	void fillBlock(int startX, int startZ, int endX, int endZ, TILE_TYPE tileType, TILE_PROPERTIES properties, const string &floorFileName, const string &wallFileName, float tileHeight);

	/**
	Fills the map with tiles of the given characteristics except, with random heights
	@param tileType The type of the tile as described according to the enumerated value
	@param floorFileName Specifies the file name of the texture to apply to the tile's floor
	@param wallFileName Specifies the file name of the texture to apply to the tile's walls
	*/
	void fillRandom(TILE_TYPE tileType, TILE_PROPERTIES properties, const string &floorFileName, const string &wallFileName);

	/** Rebuilds the display list for the entire map */
	void reaquire(void) const;

	/**
	Resizes the map and deletes its previous contents
	@param width The new width of the Map
	*/
	void resize(int width);

	/** Reset the tile layout for a new/blank map */
	void makeNewMap(void);

	/**
	Gets the number of columns in the map
	@return number of columns
	*/
	inline int getNumColumns(void) const
	{
		return width;
	}

	/**
	Gets the number of rows in the map
	@return number of rows
	*/
	inline int getNumRows(void) const
	{
		return height;
	}

	/**
	Get the meters on each side of a Tile
	@return meters per Tile
	*/
	inline float getTileMetersX(void) const
	{
		return tileMetersX;
	}
};

} // namespace Engine

#endif
