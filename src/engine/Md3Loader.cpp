/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#include <SDL/SDL.h> // we want to borrow SDL's types (e.g. Uint32)

#include "profile.h"
#include "file.h"
#include "mesh.h"
#include "Md3Loader.h"

namespace Engine {

/** Max length of file names in the MD3 model */
const size_t MAX_QPATH = 64;

/** The max number of frames that may be in the model */
const size_t MD3_MAX_FRAMES = 1024;

/** The max number of surfaces that may be in the model */
const size_t MD3_MAX_SURFACES = 16;

/** The max number of tags */
const size_t MD3_MAX_TAGS = 32;

/** The max number of shaders */
const size_t MD3_MAX_SHADERS = 256;

/** The max number of vertices */
const size_t MD3_MAX_VERTS = 4096;

/** The max number of triangles */
const size_t MD3_MAX_TRIANGLES = 8192;

/** Scaling for vertices */
const float MD3_XYZ_SCALE = 1.0f / 64.0f;

/** Unsigned eight-bit character */
typedef char U8;

/** Signed 16-bit integer */
typedef Sint16 S16; // aliases an SDL type

/** Signed 32-bit integer */
typedef Sint32 S32; // aliases an SDL type

/** IEEE-754 floating point value */
typedef float F32;

/** Triple of floating point values */
typedef Engine::Point3 VEC3;

/** Header to a MD3 file */
struct Header
{
	/** Magic number.  Should read "IDP3".  Will not be null-terminated */
	U8 magicNumber[4];

	/** MD3 version number */
	S32 version;

	/** Name of the MD3 object, usually its file name.  Null-terminated */
	U8 name[MAX_QPATH];

	/** ??? */
	S32 flags;

	/** Number of frame objects */
	S32 numFrames;

	/** Number of tag objects */
	S32 numTags;

	/** Number of surface objects */
	S32 numSurfaces;

	/** Number of skin objects (unused) */
	S32 numSkins;

	/** Offset from the start of the MD3 object where the Frame objects start */
	S32 framesOffset;

	/** Offset from the start of the MD3 object where the Tag objects start */
	S32 tagsOffset;

	/** Offset from the start of the MD3 object where the Surfaces objects start */
	S32 surfacesOffset;

	/** Offset from the start of the MD3 object where the MD3 object ends */
	S32 eofOffset;
};

/** Structure to store a Frame object */
struct Frame
{
	/** First corner of the bounding box */
	VEC3 minBounds;

	/** Second corner of the bounding box */
	VEC3 maxBounds;

	/** Local origin */
	VEC3 localOrigin;

	/** Radius of bounding sphere */
	F32 radius;

	/** Name of the Frame */
	U8 name[16];
};

/** Structure to hold a Tag object */
struct Tag
{
	/** Name of Tag object */
	U8 name[MAX_QPATH];

	/** Coordinates of Tag object */
	VEC3 origin;

	/** Orthonormal basis of the Tag */
	VEC3 axis[3];
};

/** MD3 surface object */
class Surface
{
public:
	/** Default constructor */
	Surface(void);

	/** Destructor */
	~Surface(void);

	/**
	Loads a Surface from file
	@param file The file to load the surface from.
	Assume the file cursor is set at the start of the surface already
	*/
	void create(File &file, size_t surfacesOffset);

	/**
	Allocates memory for a mesh object and copies the Surface into it.
	@param frame Frame index
	@return the mesh object.  memory-management is the responsibility of the caller.

	@todo trignometry (an issue of loading time)

	There are 255 discrete values for encodedNormal.  So
	sine/cosine precision is not really an issue.  Do we
	need to use the std lib?  A little McClaurin series
	would be much more efficient.
	*/
	Mesh* getObject(size_t frame) const;

private:
	/** Structure to hold the header to a Surface object */
	struct SurfaceHeader
	{
		/** Magic number.  Should read "IDP3".  Will not be null-terminated */
		U8 magicNumber[4];

		/** Name of the Surface object.  null-terminated */
		U8 name[MAX_QPATH];

		/** ??? */
		S32 flags;

		/** Number of animation frames.  This should match numFrames in the header */
		S32 numFrames;

		/** Number of shader objects within the Surface */
		S32 numShaders;

		/** Number of Vertex objects defined in this Surface */
		S32 numVerts;

		/** Number of Triangle objects defined within this Surface */
		S32 numTris;

		/** Offset from the Surface start where the list of Triangle objects begin */
		S32 trianglesOffset;

		/** Offset from the Surface start where the list of Shader objects begin */
		S32 shadersOffset;

		/** Offset from the Surface start where the list of tex-coords begin */
		S32 texCoordsOffset;

		/** Offset from the Surface start where the list of vertices begin */
		S32 vertexOffset;

		/** Offset from the Surface start where the Surface will end */
		S32 endOffset;
	};

	/** Structure to hold a Shader object*/
	struct Shader
	{
		/** Pathname of the shader  Null-terminated */
		U8 name[MAX_QPATH];

		/** The index of the shader */
		S32 shaderIndex;
	};

	/** Structure to hold a Triangle object */
	struct Triangle
	{
		/** Offset values into the list of verte objects */
		S32 indices[3];
	};

	/** Structure to hold a TexCoord object */
	struct TexCoord
	{
		/** Texture coodinate */
		F32 st[2];
	};

	/** Structure to hold a Vertex object */
	struct Vertex
	{
		/** X */
		S16 x;

		/** Y */
		S16 y;

		/** Y */
		S16 z;

		/** Encoded normal vector */
		S16 encodedNormal;
	};

public:
	/** Header of the surface */
	SurfaceHeader header;

	/** Shaders list */
	Shader* shaders;

	/** Triangles list */
	Triangle* triangles;

	/** TexCoord list */
	TexCoord* texCoords;

	/** Vertex list */
	Vertex* vertices;
};

AnimationController* Md3Loader::loadFromFile(const string &fileName) const
{
	string md3;

	PropertyBag xml;
	xml.loadFromFile(fileName);

	// Allocate a blank animation controller
	AnimationController *controller = new AnimationController();

	// Get key frames from the first source
	xml.get("md3", md3, 0);
	vector<KeyFrame> keyFrames = loadKeyFrames(md3);

	// Get the rest of the key frames
	const size_t numMD3 = xml.getNumInstances("md3");
	for(size_t i=1; i<numMD3; ++i)
	{
		xml.get("md3", md3, i);

		vector<KeyFrame> k = loadKeyFrames(md3);

		for(size_t i=0; i<k.size(); ++i)
		{
			keyFrames[i].merge(k[i]);
		}
	}

	// Build the animations from these keyframes
	const size_t numAnimations = xml.getNumInstances("animation");
	for(size_t i=0; i<numAnimations; ++i)
	{
		PropertyBag animation;
		string name;
		bool looping=false;
		int start=0;
		float priority=0;
		int length=0;
		float fps=0;

		xml.get("animation", animation, i);
		animation.getSym(name);
		animation.getSym(priority);
		animation.getSym(looping);
		animation.getSym(start);
		animation.getSym(length);
		animation.getSym(fps);

		// Add it to the controller
		AnimationSequence animationSequence(keyFrames, name, priority, looping, start, length, fps);
		controller->addAnimation(animationSequence);
	}

	return controller;
}

vector<KeyFrame> Md3Loader::loadKeyFrames(const string &fileName)
{
	vector<KeyFrame> keyFrames;

	Header header;
	memset(&header, 0, sizeof(header));

	Frame *frames = 0;
	Tag *tags = 0;
	Surface *surfaces = 0;

	File file(fileName, true);

	ASSERT(file.loaded(), "MD3 file failed to open: " + fileName);

	if(!file.loaded())
	{
		FAIL("MD3 failed to open: " + fileName);
	}

	// Only the first frame loads texture files.  The other frames use a copy of the handles and that copy is stored in here.
	Material md3Material;

	// Load the header
	file.read((unsigned char*)(&header), sizeof(header));

	ASSERT
	(
	header.magicNumber[0] == 'I' &&
	header.magicNumber[1] == 'D' &&
	header.magicNumber[2] == 'P' &&
	header.magicNumber[3] == '3',
	"Magic number failed to validate! This file is not an MD3 file: " + fileName
	);

	// Allocate Frame objects
	if(header.numFrames>0) frames = new Frame[header.numFrames];

	// Allocate Tag objects
	if(header.numTags>0) tags = new Tag[header.numTags];

	// Allocate Surface objects
	if(header.numSurfaces>0) surfaces = new Surface[header.numSurfaces];

	// Read Frame objects
	file.seek(header.framesOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)frames, sizeof(Frame) * header.numFrames);

	// Read Tag objects
	file.seek(header.tagsOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)tags, sizeof(Tag) * header.numTags);

	// Read Surface objects
	file.seek(header.surfacesOffset, File::FILE_SEEK_BEGIN);
	for(int i=0; i<header.numSurfaces; ++i)
	{
		// Load the next surface
		surfaces[i].create(file, header.surfacesOffset);
	}

	// Take the all the surfaces and push each frame into the mesh manager
	for(int i=0; i<surfaces[0].header.numFrames; ++i)
	{
		string name = fileName + "#" + itoa(i);

		// Create a mesh from the surface
		Mesh *mesh = surfaces[0].getObject(i);

		// Load a material for the first mesh in the first model.
		// First model only! Ignored for subsequent models
		if(i==0)
		{
			if(header.numSurfaces > 0)
			{
				mesh->material.clear();
				mesh->material.loadTexture(surfaces[0].shaders[0].name, 0);

				// Keep a copy of the material to propagate to the subsequent frames
				md3Material = mesh->material;
			}
		}
		else
		{// After the first frame, this branch executes instead.

			// Do a shallow copy of the texture handles and such
			mesh->material = md3Material;
		}

		// Record the model in a key frame
		keyFrames.push_back(KeyFrame(mesh));
	}

	delete[] frames;
	delete[] tags;
	delete[] surfaces;

	return keyFrames;
}

Surface::Surface(void)
{
	shaders = 0;
	triangles = 0;
	texCoords = 0;
	vertices = 0;
}

void Surface::create(File &file, size_t)
{
	const S32 surfaceStart = (S32)file.tell();

	delete[] shaders;
	delete[] triangles;
	delete[] texCoords;
	delete[] vertices;

	shaders = 0;
	triangles = 0;
	texCoords = 0;
	vertices = 0;

	// Load the header
	file.read((void*)&header, sizeof(header));

	ASSERT
	(
	header.magicNumber[0] == 'I' &&
	header.magicNumber[1] == 'D' &&
	header.magicNumber[2] == 'P' &&
	header.magicNumber[3] == '3',
	"MD3::Surface::create  ->  This file is a corrupted MD3 file."
	);

	// Allocate Shaders
	shaders = new Shader[header.numShaders];

	// Allocate Triangles
	if(header.numTris>0) triangles = new Triangle[header.numTris];

	// Allocate TexCoords
	if(header.numVerts>0) texCoords = new TexCoord[header.numVerts];

	// Allocate Vertices
	if(header.numVerts>0) vertices = new Vertex[header.numVerts * header.numFrames];

	// Load Shaders
	file.seek(surfaceStart + header.shadersOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)shaders, sizeof(Shader) * header.numShaders);

	// Load Triangles
	file.seek(surfaceStart + header.trianglesOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)triangles, sizeof(Triangle) * header.numTris);

	// Load TexCoords
	file.seek(surfaceStart + header.texCoordsOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)texCoords, sizeof(TexCoord) * header.numVerts);

	// Load Vertices
	file.seek(surfaceStart + header.vertexOffset, File::FILE_SEEK_BEGIN);
	file.read((void*)vertices, sizeof(Vertex) * header.numVerts * header.numFrames);
}

Surface::~Surface(void)
{
	delete[] shaders;
	delete[] triangles;
	delete[] texCoords;
	delete[] vertices;
}

Mesh* Surface::getObject(size_t frame) const
{
	Mesh *mesh = new Mesh();

	// Record the number of relevant elements
	mesh->m_numOfFaces		= header.numTris;
	mesh->m_numOfVerts		= header.numTris * 3;
	mesh->m_numTexVertex	= header.numTris * 3;

	memcpy(mesh->m_strName, header.name, MAX_QPATH);

	// Allocate memory
	mesh->m_pFaces		= new Mesh::Face[mesh->m_numOfFaces];
	mesh->m_pVerts		= new Point3[mesh->m_numOfVerts];
	mesh->m_pTexVerts	= new Point2[mesh->m_numTexVertex];
	mesh->m_pNormals	= new Point3[mesh->m_numOfVerts];

	// This rotation will be applied to the vertices
	mat4 rotation;
	rotation.rotateX((float)M_PI / 2.0f);

	// Copy faces
	for(int i=0; i<mesh->m_numOfFaces; ++i)
	{
		Mesh::Face &face = mesh->m_pFaces[i];

		for(int j=0; j<3; ++j)
		{
			int counter = i*3 + j;
			size_t texIdx = triangles[i].indices[j];
			size_t vertIdx = triangles[i].indices[j] + frame*header.numVerts;

			face.vertIndex[j] = counter;
			face.coordIndex[j] = counter;

			// Copy vert for the face
			Vertex &v = vertices[vertIdx];

			vec3 out = vec3(0, 0, 0);
			vec3 in = vec3(v.x, v.y, v.z) * MD3_XYZ_SCALE;

			out = rotation.transformVector(in);

			// Set the vertex
			mesh->m_pVerts[counter].x = out.x;
			mesh->m_pVerts[counter].y = out.y;
			mesh->m_pVerts[counter].z = out.z;

			// Decode the compressed normal
			const float normScale = (float)(2.0 * M_PI) / 255.0f;
			float lat = ((v.encodedNormal >> 8) & 255)	* normScale;
			float lng = (v.encodedNormal & 255)			* normScale;

			mesh->m_pNormals[counter].x = cosf(lat) * sinf(lng);
			mesh->m_pNormals[counter].y = sinf(lat) * sinf(lng);
			mesh->m_pNormals[counter].z = cosf(lng);

			// Set the tex coord
			mesh->m_pTexVerts[counter].x =  texCoords[texIdx].st[0];
			mesh->m_pTexVerts[counter].y = -texCoords[texIdx].st[1];
		}
	}

	// Re-allocate elements
	mesh->reallocElements();

	return mesh;
}

}; // namespace Engine

