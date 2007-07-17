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

#include "stdafx.h"
#include "gl.h"
#include "EffectManager.h"
#include "Tile.h"
#include "material.h"
#include "searchfile.h"
#include "Map.h"
#include "profile.h"

namespace Engine { 

Map::Map(void)
{
	clear();
}

Map::~Map(void)
{
	destroy();
}

void Map::clear(void)
{
	grid=0;
	width=0;
	height=0;
	quadTree=0;
}

void Map::destroy(void)
{
	delete[] quadTree;
	quadTree=0;

	delete [] grid;
	grid = 0;

	destroyMaterialsLegend();

	release();

	clear();
}

void Map::destroyMaterialsLegend(void)
{
	materialsLegend.clear();
}

void Map::loadMaterialsLegend(PropertyBag &materialsLegend)
{
	destroyMaterialsLegend();

	const size_t numberOfEntries = materialsLegend.getNumInstances(_T("material"));
	for(size_t i=0; i<numberOfEntries;++i)
	{
		_tstring materialFileName;
		materialsLegend.get(_T("material"), materialFileName, i);
		loadMapMaterial(materialFileName);
	}
}

PropertyBag Map::saveMaterialsLegend(void) const
{
	PropertyBag xml;

	for(MAP_MATERIAL_ID i=0; (size_t)i<materialsLegend.size(); ++i)
	{
		xml.add(  _T("material"), File::fixFilename(materialsLegend[i]->getName())  );
	}

	return xml;
}

MAP_MATERIAL_ID Map::loadMapMaterial(const _tstring &materialFileName)
{
	_tstring fixedMaterialFileName = File::fixFilename(materialFileName);

	for(size_t i=0; i < materialsLegend.size(); ++i)
	{
		const _tstring &fileName = File::fixFilename(materialsLegend[i]->getName());

		if(fileName == fixedMaterialFileName)
			return (MAP_MATERIAL_ID)i;
	}

	materialsLegend.push_back(new Material(fixedMaterialFileName));

	size_t numMaterials = materialsLegend.size();

	return (MAP_MATERIAL_ID)(numMaterials-1);
}

const Material* Map::getMapMaterial(MAP_MATERIAL_ID materialID) const
{
	ASSERT((size_t)materialID<materialsLegend.size(), _T("materialID invalid"));
	return ((size_t)materialID<materialsLegend.size()) ? materialsLegend[materialID] : 0;
}

void Map::create(PropertyBag &xml)
{
	PROFILE

	
	destroy();

	// Get the scaling of the map along the X and Z directions
	xml.get(_T("tileMetersX"), tileMetersX);

	// Load materials used in this map
	PropertyBag materialsLegend;
	xml.get(_T("materialsLegend"), materialsLegend);
	loadMaterialsLegend(materialsLegend);

	// Get the dimensions of the map
	xml.get(_T("width"), width);
	xml.get(_T("height"), height);

	// Get the name of the binary map file
	_tstring tileDataFileName;
	if(!xml.get(_T("tileDataFileName"), tileDataFileName))
	{
		FAIL(_T("Failed to retrieve the name of the binary tile data file from the Map definition!"));
	}

	// Allocate tile data
	grid = new Tile[width * height];

	// Load the binary map file
	File tileDataFile;
	tileDataFile.openFile(tileDataFileName, true);

	if(!tileDataFile.loaded())
	{
		ERR(_T("Failed to load the binary tile data file from the Map definition: ") + tileDataFileName);
		makeNewMap();
	}
	else
	{
		// Load the tiles from the binary file
		for(int y=0; y<height; ++y)
		{
			for(int x=0; x<width; ++x)
			{
				getTile(x,y).load(x, y, tileDataFile, *this);
			}
		}
	}

	quadTree = new QuadTreeNode(grid, 0, 0, width, width, tileMetersX);
	
	// Allocate OpenGL resources
	reaquire();
}

void Map::makeNewMap(void)
{
	const int SIZE = 128;
	const int BORDER = 6;

	resize(SIZE);
	fillBlock(BORDER, BORDER, SIZE-BORDER, SIZE-BORDER, TILE_BLOCK, 0, _T("data/tiles/floor/floor.jpg"), _T("data/tiles/wall/wall.jpg"), 0.0f);
}

void Map::resize(int width)
{
	ASSERT(width > 0, _T("width is zero or negative"));
	ASSERT(width < 512, _T("width is too large"));

	// Delete the old map
	delete[] grid;

	this->width = this->height = width;

	// Allocate the new map
	grid = new Tile[width * width];

	// Fill the test map
	fill(TILE_BLOCK, TILE_PROPERTY_IMPASSABLE, _T("data/tiles/floor/floor.jpg"), _T("data/tiles/wall/wall.jpg"), 2.4f);

	// remake the tree
	delete quadTree;
	quadTree = new QuadTreeNode(grid, 0, 0, width, width, tileMetersX);
}

void Map::save(PropertyBag &xml, const _tstring &zoneName)
{
	xml.add(_T("width"), width);
	xml.add(_T("height"), height);
	xml.add(_T("tileMetersX"), tileMetersX);

	// Save materials used in this map
	xml.add(_T("materialsLegend"), saveMaterialsLegend());

	// save tile data	
	_tstring tileDataFileName = pathAppend(_T("data/zones"), zoneName + _T(".bin"));
	xml.add(_T("tileDataFileName"), tileDataFileName);

	File file;
	for(int i=0; i<width*height; ++i)
	{
		grid[i].save(file);
	}
	file.saveFile(tileDataFileName, true);
}

void Map::fill(TILE_TYPE tileType, TILE_PROPERTIES properties, const _tstring &floorFileName, const _tstring &wallFileName, float tileHeight)
{
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
			getTile(x,y).create(x,y,tileType, properties, tileHeight, floorFileName, wallFileName, *this);
	}
	
	reaquire();
}

void Map::fillRandom(TILE_TYPE tileType, TILE_PROPERTIES properties, const _tstring &floorFileName, const _tstring &wallFileName)
{
	for(int y=0; y<height; ++y)
	{
		for(int x=0; x<width; ++x)
			getTile(x,y).create(x,y, tileType, properties, FRAND_RANGE(0.0f, 2.0f), loadMapMaterial(floorFileName), loadMapMaterial(wallFileName), *this);
	}

	reaquire();
}

void Map::draw(void) const
{
	PROFILE;
	quadTree->draw(vec3(width/2*tileMetersX, 0, height/2*tileMetersX));
}

void Map::drawToDepthBuffer(void) const
{
	draw();
}

void Map::reaquire(void) const
{}

void Map::release(void) const
{}

void Map::fillBlock(int startX, int startZ, int endX, int endZ, TILE_TYPE tileType, TILE_PROPERTIES properties, const _tstring &floorFileName, const _tstring &wallFileName, float tileHeight)
{
	for(int z=min(startZ, endZ); z<=max(startZ, endZ); ++z)
	{
		for(int x=min(startX, endX); x<=max(startX, endX); ++x)
		{
			getTile(x,z).create(x,z, tileType, properties, tileHeight, loadMapMaterial(floorFileName), loadMapMaterial(wallFileName), *this);
		}
	}

	reaquire();
}

}; // namespace
