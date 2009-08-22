/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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
#include "Application.h"
#include "Tile.h"
#include "profile.h"
#include "QuadTreeNode.h"

namespace Engine { 

QuadTreeNode::~QuadTreeNode(void)
{
	delete northWest;
	delete southWest;
	delete northEast;
	delete southEast;
}

QuadTreeNode::QuadTreeNode(Tile *grid, size_t leftColumn, size_t lowerRow, size_t numColumns, size_t totalNumColumns, float tileMetersX)
:northWest(0),
southWest(0),
northEast(0),
southEast(0),
tile(0),
diameter(0)
{
	size_t halfWidth = numColumns/2;

	setupBoundingBox(tileMetersX*numColumns);

	if(numColumns>1)
	{
		southWest = new QuadTreeNode(grid, leftColumn,				lowerRow,				halfWidth, totalNumColumns, tileMetersX);
		southEast = new QuadTreeNode(grid, leftColumn + halfWidth,	lowerRow,				halfWidth, totalNumColumns, tileMetersX);
		northWest = new QuadTreeNode(grid, leftColumn,				lowerRow + halfWidth,	halfWidth, totalNumColumns, tileMetersX);
		northEast = new QuadTreeNode(grid, leftColumn + halfWidth,	lowerRow + halfWidth,	halfWidth, totalNumColumns, tileMetersX);
	}
	else
	{
		tile = &grid[leftColumn + lowerRow*totalNumColumns];
	}
}

bool QuadTreeNode::isVisible(const vec3 &nodeCenter) const
{
	PROFILE
	return g_Camera.getFrustum().SphereInFrustum2(nodeCenter, diameter);
}

void QuadTreeNode::draw(const vec3 &nodeCenter) const
{
	if(tile)
	{
		tile->draw(nodeCenter.x, nodeCenter.z);
	}
	else if(isVisible(nodeCenter))
	{
		if(southWest) southWest->draw(nodeCenter + vec3(boundingBox.m_Min.x, 0, boundingBox.m_Min.z));
		if(southEast) southEast->draw(nodeCenter + vec3(boundingBox.m_Max.x, 0, boundingBox.m_Min.z));
		if(northWest) northWest->draw(nodeCenter + vec3(boundingBox.m_Min.x, 0, boundingBox.m_Max.z));
		if(northEast) northEast->draw(nodeCenter + vec3(boundingBox.m_Max.x, 0, boundingBox.m_Max.z));
	}
}

void QuadTreeNode::setupBoundingBox(float size)
{
	boundingBox.m_Max = vec3(size/4, size, size/4);
	boundingBox.m_Min = vec3(-size/4, 0, -size/4);
	boundingBox.m_Pos = vec3(0,0,0); // we don't know where in the map we are

	diameter = vec3(boundingBox.m_Max-boundingBox.m_Min).getMagnitude();
}

}; // namespace
