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
#include "file.h"
#include "Tile.h"
#include "Map.h"

namespace Engine {

void texCoord(float u, float v)
{
	glTexCoord2f(u, v);

	if(g_bUseMultitexture)
	{
		glMultiTexCoord2fARB(textureStages[0], u, v);
	}
}

Tile::Tile(void)
:type(TILE_BLOCK),
propertiesBitmap(0),
wallMaterialMatID(0),
floorMaterialMatID(0),
wallMat(0),
floorMat(0),
north(0),
south(0),
east(0),
west(0)
{}

void Tile::load(int x, int z, File &file, Map &owner)
{
	float tileHeight = 0.0f;

	type = TILE_BLOCK;
	propertiesBitmap = 0;
	floorMaterialMatID = 0;
	wallMaterialMatID = 0;
	floorMat = 0;
	wallMat = 0;

	file.read(&type, sizeof(TILE_TYPE));
	file.read(&propertiesBitmap, sizeof(TILE_PROPERTIES));
	file.read(&tileHeight, sizeof(float));
	file.read(&wallMaterialMatID, sizeof(MAP_MATERIAL_ID));
	file.read(&floorMaterialMatID, sizeof(MAP_MATERIAL_ID));

	create(x, z, type, propertiesBitmap, tileHeight, floorMaterialMatID, wallMaterialMatID, owner);
}

void Tile::save(File &file)
{
	file.write(&type, sizeof(TILE_TYPE));
	file.write(&propertiesBitmap, sizeof(TILE_PROPERTIES));
	file.write(&boundingBox.m_Max.y, sizeof(float));
	file.write(&wallMaterialMatID, sizeof(MAP_MATERIAL_ID));
	file.write(&floorMaterialMatID, sizeof(MAP_MATERIAL_ID));
}

void Tile::create(int x, int z, TILE_TYPE type, TILE_PROPERTIES propertiesBitmap, float tileHeight, const string &floorFileName, const string &wallFileName, Map &owner)
{
	this->propertiesBitmap = propertiesBitmap;
	this->type = type;

	this->floorMaterialMatID = owner.loadMapMaterial(floorFileName);
	this->wallMaterialMatID = owner.loadMapMaterial(wallFileName);

	floorMat = owner.getMapMaterial(floorMaterialMatID);
	wallMat = owner.getMapMaterial(wallMaterialMatID);

	setupBoundingBox(owner.getTileMetersX(), tileHeight);

	setupNeighbors(owner, x, z);
}

void Tile::create(int x, int z, TILE_TYPE type, TILE_PROPERTIES propertiesBitmap, float tileHeight, MAP_MATERIAL_ID floorMaterialMatID, MAP_MATERIAL_ID wallMaterialMatID, Map &owner)
{
	this->propertiesBitmap = propertiesBitmap;
	this->type = type;
	this->floorMaterialMatID = floorMaterialMatID;
	this->wallMaterialMatID = wallMaterialMatID;

	floorMat = owner.getMapMaterial(floorMaterialMatID);
	wallMat = owner.getMapMaterial(wallMaterialMatID);

	setupBoundingBox(owner.getTileMetersX(), tileHeight);

	setupNeighbors(owner, x, z);
}

void Tile::setupNeighbors(Map &owner, int x, int z)
{
	north=0;
	south=0;
	east=0;
	west=0;

	if(z+1 < owner.getNumColumns())	north = &owner.getTile(x,z+1);
	if(z-1 > 0)						south = &owner.getTile(x,z-1);
	if(x+1 < owner.getNumColumns())	east = &owner.getTile(x+1,z);
	if(x-1 > 0)						west = &owner.getTile(x-1,z);
}

void Tile::drawWalls(const Tile *north, const Tile *south, const Tile *east, const Tile *west) const
{
	if(fabsf(getTileHeight()) < FLT_EPSILON)
	{
		return;
	}

	float minV = 1.0f - getTileHeight() / 2.4f;

	// Set the material of the Tile
	if(wallMat) wallMat->bind();

	// Draw the cube
	glBegin(GL_QUADS);

		if(SHOULD_DRAW_WALL(north))
		{
			glNormal3f( 0.0f, 0.0f, 1.0f);
			texCoord(0.0f, minV); glVertex3f(0.0f, 0.0f, 1.0f);
			texCoord(1.0f, minV); glVertex3f(1.0f, 0.0f, 1.0f);
			texCoord(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			texCoord(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
		}

		if(SHOULD_DRAW_WALL(south))
		{
			glNormal3f( 0.0f, 0.0f, -1.0f);
			texCoord(1.0f, minV); glVertex3f(0.0f, 0.0f, 0.0f);
			texCoord(1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
			texCoord(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
			texCoord(0.0f, minV); glVertex3f(1.0f, 0.0f, 0.0f);
		}

		if(SHOULD_DRAW_WALL(east))
		{
			glNormal3f( 1.0f, 0.0f, 0.0f);
			texCoord(1.0f, minV); glVertex3f(1.0f, 0.0f, 0.0f);
			texCoord(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
			texCoord(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			texCoord(0.0f, minV); glVertex3f(1.0f, 0.0f, 1.0f);
		}

		if(SHOULD_DRAW_WALL(west))
		{
			glNormal3f(-1.0f, 0.0f, 0.0f);
			texCoord(0.0f, minV); glVertex3f(0.0f, 0.0f, 0.0f);
			texCoord(1.0f, minV); glVertex3f(0.0f, 0.0f, 1.0f);
			texCoord(1.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
			texCoord(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		}
	glEnd();
}

void Tile::drawFloor(const Tile *, const Tile *, const Tile *, const Tile *) const
{
	// Set the material of the floor
	if(floorMat) floorMat->bind();

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 1.0f, 0.0f);
		texCoord(0.0f, 1.0f); glVertex3f(0.0f, 1.0f, 1.0f);
		texCoord(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		texCoord(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		texCoord(0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
}

void Tile::drawBlockTypeTile(const Tile *north, const Tile *south, const Tile *east, const Tile *west) const
{
	drawFloor(north, south, east, west);
	drawWalls(north, south, east, west);
}

void Tile::setupBoundingBox(float width, float height)
{
	boundingBox.m_Max = vec3(width/2, height, width/2);
	boundingBox.m_Min = vec3(-width/2, 0, -width/2);
	boundingBox.m_Pos = vec3(0,0,0); // we don't know where in the map we are
}

void Tile::draw(float x, float z) const
{
	if(type == TILE_EMPTY)
	{
		return;
	}

	float width = boundingBox.m_Max.x-boundingBox.m_Min.x;
	float height = boundingBox.m_Max.y-boundingBox.m_Min.y;
	float depth = boundingBox.m_Max.z-boundingBox.m_Min.z;

	glPushMatrix();
	glTranslatef(x - width/2, 0.0f, z - depth/2);
	glScalef(width, height, depth);

	switch(type)
	{
	case TILE_BLOCK: drawBlockTypeTile(north, south, east, west); break;
	default: break;
	};

	glPopMatrix();
}

bool Tile::setPassable(bool passable)
{
	if(passable)
		propertiesBitmap = 0;
	else
		propertiesBitmap = TILE_PROPERTY_IMPASSABLE;

	return passable;
}

}; // namespace
