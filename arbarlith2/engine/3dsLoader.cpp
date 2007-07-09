/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2007 Game Creation Society
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
#include "3dsLoader.h"
#include "WaitScreen.h"
#include "profile.h"

namespace Engine {

/** Identifies the primary chunk at the beginning of the file */
const unsigned short PRIMARY = 0x4D4D;

/** Main chunk, gives the version of the mesh and is found right before the material and object information */
const unsigned short OBJECTINFO = 0x3D3D;

/** Main chunk, gives the version of the .3ds file */
const unsigned short VERSION = 0x0002;

/** Main chunk, header for all of the key frame info */
const unsigned short EDITKEYFRAME = 0xB000;

/** Sub-chunk of OBJECTINFO, stores the texture info */
const unsigned short MATERIAL = 0xAFFF;

/** Sub-chunk of OBJECTINFO, stores the faces, vertices, etc... */
const unsigned short OBJECT = 0x4000;

/** Sub-chunk of MATERIAL, holds the material name */
const unsigned short MATNAME = 0xA000;

/** Sub-chunk of MATERIAL, holds the color of the object/material */
const unsigned short MATDIFFUSE = 0xA020;

/** Sub-chunk of MATERIAL, header for a new material */
const unsigned short MATMAP = 0xA200;

/** Sub-chunk of MATERIAL, holds the file name of the texture */
const unsigned short MATMAPFILE = 0xA300;

/** lets us know that we are reading a new object */
const unsigned short OBJECT_MESH = 0x4100;

/** Sub-chunk of OBJECT_MESH, object vertices */
const unsigned short OBJECT_VERTICES = 0x4110;

/** Sub-chunk of OBJECT_MESH, object faces */
const unsigned short OBJECT_FACES = 0x4120;

/** Sub-chunk of OBJECT_MESH, references a material defined elsewhere */
const unsigned short OBJECT_MATERIAL = 0x4130;

/** Sub-chunk of OBJECT_MESH, texture coordinates */
const unsigned short OBJECT_UV = 0x4140;

AnimationController* _3dsLoader::loadFromFile(const _tstring &fileName) const
{
    CPropBag xml;
	_tstring skin;
	bool truespaceModel=false;

	if(!xml.Load(fileName))
	{
		ERR(_tstring(_T("File failed to open: ")) + fileName);
		FAIL(_tstring(_T("File failed to open: ")) + fileName);
		return 0;
	}

	AnimationController* controller = new AnimationController();

	xml.Get(_T("Truespace"), truespaceModel);
	xml.Get(_T("forceSkin"), skin);

	size_t numAnimations = xml.GetNumInstances(_T("animation"));
	for(size_t i=0; i<numAnimations; ++i)
	{
		CPropBag animation;
		_tstring name;
		bool looping=false;
		float priority=0;
		float fps=0;

		xml.Get(_T("animation"), animation, (int)i);
		animation.getSym(name);
		animation.getSym(priority);
		animation.getSym(looping);
		animation.getSym(fps);

		// Load all the keyframes
		vector<KeyFrame> keyFrames;
		int length = animation.GetNumInstances(_T("keyframe"));
		for(int j=0; j<length; ++j)
		{
			_tstring keyFrameFile;
			animation.Get(_T("keyframe"), keyFrameFile, j);

			Engine::Model keyFrame = loadKeyFrame(keyFrameFile);

			cullDegenerateMeshes(keyFrame);

			if(!skin.empty())
				forceSkin(keyFrame, skin);

			if(truespaceModel)
				fixTrueSpaceVertices(keyFrame);

			keyFrames.push_back(keyFrame);
		}

		// Add it to the controller
		AnimationSequence animationSequence(keyFrames, name, priority, looping, 0, length, fps);
		controller->addAnimation(animationSequence);
	}

	return controller;
}

Engine::Model _3dsLoader::loadKeyFrame(const _tstring &fileName) const
{
	// Open the file
	File file(fileName, true);
	if(!file.loaded())
	{
		FAIL(_T("Failed to open 3DS file: ") + fileName);
	}

	// Make sure this is a 3DS file
	Chunk currentChunk(file);
	if(currentChunk.getID() != PRIMARY)
	{
		FAIL(_T("PRIMARY chunk not found!  This is not a valid 3DS file: ") + fileName);
	}

	// recursively load all chunks
	Wrapper wrapper;
	processNextChunk(currentChunk, wrapper);

	return wrapper.model;
}

_3dsLoader::Chunk::Chunk(File &file) : ID(0)
{
	unsigned int chunkLength=0;

	file.read(&ID, 2);
	file.read(&chunkLength, 4);
	file.read(*this, chunkLength-6); // chunk length is measured from the position of the ID to the end of the chunk
	rewind();

	fileName = file.getFilename();
}

_3dsLoader::Chunk::Chunk(Chunk &parentChunk)
: File(parentChunk),
  ID(0)
{
	if(!parentChunk.endOfFile())
	{
		unsigned int chunkLength=0;
		parentChunk.read(&ID, 2);
		parentChunk.read(&chunkLength, 4);
		parentChunk.read(*this, chunkLength-6); // chunk length is measured from the position of the ID to the end of the chunk
		rewind();
	}

	fileName = parentChunk.getFilename();
}

int _3dsLoader::Chunk::getString(char *s)
{
	ASSERT(s!=0, _T("string was null"));

	int index = 0;

	// Read 1 byte of data which is the first letter of the string
	s[0] = getChar();

	while (*(s + index++) != 0) {
		s[index] = getChar();
	}

	return (int)(strlen(s) + 1); // plus one for the null terminator
}








void _3dsLoader::processNextChunk(Chunk &parentChunk, Wrapper &model) const
{
	while(!parentChunk.endOfFile())
	{
		Chunk currentChunk(parentChunk);

		// It is normal to encounter unknown chunks every once in a while.
		switch(currentChunk.getID())
		{
		case VERSION:		processVersionChunk(currentChunk);				break;
		case OBJECTINFO:	processObjectInfoChunk(currentChunk, model);	break;
		case OBJECT:		processObjectChunk(currentChunk, model);		break;
		case MATERIAL:		processMaterialChunk(currentChunk, model);		break;
		}
	}
}

void _3dsLoader::processVersionChunk(Chunk &currentChunk) const
{
	ASSERT(currentChunk.getID()==VERSION, _T("Expected chunk ID VERSION"));

	// If the file version is over 3, give a warning that there could be a problem
	if((currentChunk.getSize() == 4) && (currentChunk.peekChar() > 0x03))
	{
		ERR(_T("This 3DS file is over version 3 so it may load incorrectly"));
	}
}

void _3dsLoader::processObjectInfoChunk(Chunk &currentChunk, Wrapper &model) const
{
	ASSERT(currentChunk.getID()==OBJECTINFO, _T("Expected chunk ID OBJECTINFO"));

	// Ignore the mesh version data
	// if the object has a texture, the next chunk should be MATERIAL, followed by OBJECT.
	processNextChunk(currentChunk, model);
}

void _3dsLoader::processObjectChunk(Chunk &currentChunk, Wrapper &wrapper) const
{
	ASSERT(currentChunk.getID()==OBJECT, _T("Expected chunk ID OBJECT"));

	Mesh *mesh = new Mesh;

	currentChunk.getString(mesh->m_strName);

	// set up a little trap to tell if an object material was encountered
	const _tstring &trap = _T("FOOBARBARIAN");
	mesh->material.setName(trap);


	processNextObjectChunk(currentChunk, wrapper, mesh);


	// if no material was loaded
	if(mesh->material.getName()==trap)
	{
		mesh->material = wrapper.getMostRecentMaterial();
	}

	wrapper.model.push_back(mesh);
}

void _3dsLoader::processMaterialChunk(Chunk &parentChunk, Wrapper &wrapper) const
{
	Material material;
	processNextMaterialChunk(parentChunk, material);
	wrapper.addMaterial(material);
}

void _3dsLoader::processNextMaterialChunk(Chunk &parentChunk, Material &material) const
{
	while(!parentChunk.endOfFile())
	{
		Chunk innerChunk(parentChunk);

		// purposefully ingore chunks except those listed below
		switch(innerChunk.getID())
		{
		case MATNAME:		readMaterialName(innerChunk, material);			break;
		case MATMAPFILE:	readMaterialMapFile(innerChunk, material);		break;
		case MATMAP:		processNextMaterialChunk(innerChunk, material);	break;
		}
	}
}

void _3dsLoader::processNextObjectChunk(Chunk &currentChunk, Wrapper &wrapper, Mesh *mesh) const
{
	while(!currentChunk.endOfFile())
	{
		Chunk innerChunk(currentChunk);

		// purposefully ignore chunks not listed below
		switch(innerChunk.getID())
		{
		case OBJECT_MESH:		processNextObjectChunk(innerChunk, wrapper, mesh);	break;
		case OBJECT_VERTICES:	readVertices(innerChunk, mesh);						break;
		case OBJECT_FACES:		readVertexIndices(innerChunk, wrapper, mesh);		break;
		case OBJECT_MATERIAL:	readObjectMaterial(innerChunk, wrapper, mesh);		break;
		case OBJECT_UV:			readUVCoordinates(innerChunk, mesh);				break;
		}
	}
}

void _3dsLoader::readVertices(Chunk &currentChunk, Mesh *mesh) const
{
	ASSERT(currentChunk.getID()==OBJECT_VERTICES, _T("Expected chunk ID OBJECT_VERTICES"));

	currentChunk.read(&mesh->m_numOfVerts, 2);

	if(mesh->m_numOfVerts>1)
	{
		mesh->m_pVerts = new Point3[mesh->m_numOfVerts];

		currentChunk.read(mesh->m_pVerts, currentChunk.getSize() - currentChunk.tell());

		fixVertices(mesh);
	}
}

void _3dsLoader::fixVertices(Mesh *mesh) const
{
	for(int i=0; i < mesh->m_numOfVerts; ++i)
	{
		std::swap(mesh->m_pVerts[i].y, mesh->m_pVerts[i].z);

		mesh->m_pVerts[i].z *= -1; // negate
	}
}

void _3dsLoader::fixTrueSpaceVertices(Engine::Model &model) const
{
	for(size_t i=0; i<model.size(); ++i)
	{
		fixTrueSpaceVertices(model[i]);
	}
}

void _3dsLoader::fixTrueSpaceVertices(Mesh *mesh) const
{
	mat4 rot;
	rot.rotateY((float)M_PI);

	for(int i=0; i < mesh->m_numOfVerts; ++i)
	{
		vec3 corrected = rot.transformVector(mesh->m_pVerts[i]);

		mesh->m_pVerts[i].x = corrected.x;
		mesh->m_pVerts[i].y = corrected.y;
		mesh->m_pVerts[i].z = corrected.z;
	}
}

void _3dsLoader::cullDegenerateMeshes(Engine::Model &model) const
{
	Engine::Model::iterator i = model.begin();

	while(i != model.end())
	{
		Mesh *mesh = *i;

		if(mesh->m_numOfFaces<1 || mesh->m_numOfVerts<3 || mesh->m_numTexVertex<3)
		{
			delete mesh;
			i = model.erase(i);
		}
		else
			++i;
	}
}

void _3dsLoader::readVertexIndices(Chunk &currentChunk, Wrapper &wrapper, Mesh *mesh) const
{
	ASSERT(currentChunk.getID()==OBJECT_FACES, _T("Expected chunk ID OBJECT_FACES"));

	unsigned short int trashVisibilityFlag=0;

	currentChunk.read(&(mesh->m_numOfFaces), 2);

	if(mesh->m_numOfFaces>0)
	{
		mesh->m_pFaces = new Mesh::Face[mesh->m_numOfFaces];
		memset(mesh->m_pFaces, 0, sizeof(Mesh::Face) * mesh->m_numOfFaces);

		for(int i = 0; i < mesh->m_numOfFaces; ++i)
		{
			currentChunk.read(&mesh->m_pFaces[i].vertIndex[2], 2);
			currentChunk.read(&mesh->m_pFaces[i].vertIndex[1], 2);
			currentChunk.read(&mesh->m_pFaces[i].vertIndex[0], 2);
			currentChunk.read(&trashVisibilityFlag, 2);
		}
	}

	mesh->reallocElements();
	generateNormals(mesh);

	// An OBJECT_MATERIAL tag may be nested within this chunk
	processNextObjectChunk(currentChunk, wrapper, mesh);
}

void _3dsLoader::generateNormals(Mesh *mesh) const
{
	if(mesh->m_numOfVerts > 2)
	{
		mesh->m_pNormals = new Point3[mesh->m_numOfVerts];

		for(int i=0; i<mesh->m_numOfFaces; ++i)
		{
			size_t iA = mesh->m_pFaces[i].vertIndex[0];
			size_t iB = mesh->m_pFaces[i].vertIndex[1];
			size_t iC = mesh->m_pFaces[i].vertIndex[2];

			Point3 A = mesh->m_pVerts[iA];
			Point3 B = mesh->m_pVerts[iB];
			Point3 C = mesh->m_pVerts[iC];

			vec3 vA(A.x, A.y, A.z);
			vec3 vB(B.x, B.y, B.z);
			vec3 vC(C.x, C.y, C.z);

			vec3 AC = vA - vC;
			vec3 BC = vB - vC;
			vec3 norm = AC.cross(BC).getNormal();

			mesh->m_pNormals[iA].x = -norm.x;
			mesh->m_pNormals[iA].y = -norm.y;
			mesh->m_pNormals[iA].z = -norm.z;

			mesh->m_pNormals[iB].x = -norm.x;
			mesh->m_pNormals[iB].y = -norm.y;
			mesh->m_pNormals[iB].z = -norm.z;

			mesh->m_pNormals[iC].x = -norm.x;
			mesh->m_pNormals[iC].y = -norm.y;
			mesh->m_pNormals[iC].z = -norm.z;
		}
	}
}

void _3dsLoader::readUVCoordinates(Chunk &currentChunk, Mesh *mesh) const
{
	ASSERT(currentChunk.getID()==OBJECT_UV, _T("Expected chunk ID OBJECT_UV"));

	currentChunk.read(&mesh->m_numTexVertex, 2);

	if(mesh->m_numTexVertex>3)
	{
		mesh->m_pTexVerts = new Point2[mesh->m_numTexVertex];
		currentChunk.read(mesh->m_pTexVerts, currentChunk.getSize() - currentChunk.tell());
	}
}

void _3dsLoader::readObjectMaterial(Chunk &currentChunk, Wrapper &model, Mesh *mesh) const
{
	ASSERT(currentChunk.getID()==OBJECT_MATERIAL, _T("Expected chunk ID OBJECT_MATERIAL"));

	char materialName[255] = {0};
	currentChunk.getString(materialName);

	mesh->material = model.getMaterial(toTString(materialName));

	// We don't care about shared vertices, so skip the rest
}

void _3dsLoader::readMaterialMapFile(Chunk &currentChunk, Material &material) const
{
	ASSERT(currentChunk.getID()==MATMAPFILE, _T("Expected chunk ID MATMAPFILE"));

	size_t size = currentChunk.getSize() - currentChunk.tell();
	char *relativeFileName = new char[size];
	currentChunk.read(relativeFileName, size);

	_tstring path = currentChunk.getPath();
	_tstring relativeFileNameT = toTString(relativeFileName);
	_tstring absoluteFileName = path + relativeFileNameT;

	material.loadTexture(absoluteFileName, 0);

	delete[] relativeFileName;
}

void _3dsLoader::readMaterialName(Chunk &currentChunk, Material &material) const
{
	ASSERT(currentChunk.getID()==MATNAME, _T("Expected chunk ID MATNAME"));

	size_t size = currentChunk.getSize() - currentChunk.tell();
	char *materialName = new char[size];
	currentChunk.read(materialName, size);

	material.setName(toTString(materialName));

	delete[] materialName;
}

void _3dsLoader::forceSkin(Engine::Model &model, const _tstring &skinFileName) const
{
	Material material;
	material.loadTexture(skinFileName, 0);

	for(size_t i=0; i<model.size(); ++i)
	{
		 model[i]->material = material;
	}
}

};
