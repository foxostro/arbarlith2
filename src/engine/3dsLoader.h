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

#ifndef _3DSLOADER_H_
#define _3DSLOADER_H_

#include "ModelLoader.h"

namespace Engine {

/** 3DS model loader */
class _3dsLoader : public ModelLoader
{
private:
	/**
	Loads a model from file
	@param fileName The file name of the model
	*/
	virtual AnimationController* loadFromFile(const string &fileName) const;

	/** Attach material data to the model */
	class Wrapper
	{
	private:
		vector<Material> materials;

	public:
		Model model;

		void addMaterial(const Material &material)
		{
			materials.push_back(material); // copy the material
		}

		Material& getMostRecentMaterial(void)
		{
			ASSERT(!materials.empty(), "no materials to get");
			return materials[materials.size()-1];
		}

		Material& getMaterial(const string &materialName)
		{
			ASSERT(!materials.empty(), "no materials to get");
			for(size_t i=0; i<materials.size(); ++i)
			{
				if(materialName == materials[i].getName())
					return materials[i];
			}

			FAIL("failed to retrieve material: " + materialName);

			return materials[0]; // return anything at all
		}
	};

	/** structure for 3DS indices (since .3DS stores 4 unsigned shorts) */
	struct Indices
	{
		/** This will hold point1, 2, and 3 index's into the vertex array plus a visible flag */
		unsigned short a, b, c, bVisible;
	};

	/** Generic chunk */
	class Chunk : public File
	{
	public:
		/**
		Constructs a chunk by reading it out of a parent chunk
		@param parentChunk The parent chunk to read the chunk from
		*/
		Chunk(Chunk &parentChunk);

		/**
		Constructs a chunk by reading it out of a file
		@param file The file to read the chunk from
		*/
		Chunk(File &file);

		/**
		Reads a string from file
		@param file the file is assumed to be seek'd to the positon of the string
		@param s returns the string that was read
		@return the number of bytes read (including the null terminator)
		*/
		int getString(char *s);

		/**
		get the chunk ID
		@return ID
		*/
		unsigned short int getID(void) const
		{
			return ID;
		}

	private:
		/** The chunk's ID */
		unsigned short int ID;
	};

	/**
	Loads a single model from file
	@param fileName File name of the 3DS model
	@return Model
	*/
	Engine::Model loadKeyFrame(const string &fileName) const;

	/**
	Reads the next large chunk
	@param parentChunk The parent chunk
	@param wrapper the partially loaded model
	*/
	void processNextChunk(Chunk &parentChunk, Wrapper &wrapper) const;

	/**
	Processes the version data
	@param file Assumes that the file is seek'd to the data section
	@param currentChunk chunk data
	*/
	void processVersionChunk(Chunk &currentChunk) const;

	/**
	Processes the object info data
	@param currentChunk chunk data
	@param wrapper the partially loaded model
	*/
	void processObjectInfoChunk(Chunk &currentChunk, Wrapper &wrapper) const;

	/**
	Processes the object data
	@param currentChunk chunk data
	@param wrapper the partially loaded model
	*/
	void processObjectChunk(Chunk &currentChunk, Wrapper &wrapper) const;

	/**
	Processes a material chunk
	@param currentChunk chunk data
	@param model the partially loaded model
	*/
	void processMaterialChunk(Chunk &parentChunk, Wrapper &wrapper) const;

	/**
	Reads the next object chunk
	@param currentChunk the current chunk
	@param wrapper the partially loaded model
	@param mesh the next mesh allocated for the model
	*/
	void processNextObjectChunk(Chunk &currentChunk, Wrapper &wrapper, Mesh *mesh) const;

	/**
	Reads the next material chunk
	@param currentChunk chunk data
	@param material partially loaded material
	*/
	void processNextMaterialChunk(Chunk &currentChunk, Material &material) const;

	/**
	Reads the objects vertices
	@param currentChunk Chunk data
	@param mesh partially loaded mesh
	*/
	void readVertices(Chunk &currentChunk, Mesh *mesh) const;

	/**
	Because 3D Studio Max Models with the Z-Axis pointing up, we
	need to flip the y values with the z values in our vertices.
	That way it will be normal, with Y pointing up.
	@param mesh partially loaded mesh
	*/
	void fixVertices(Mesh *mesh) const;

	/**
	Truespace defaults to having the front view face the opposite direction on the Z-Axis
	It is a lot easier to fix it here than to try to get an artist to fix the models...
	@param mesh partially loaded mesh
	*/
	void fixTrueSpaceVertices(Engine::Model &model) const;

	/**
	Truespace defaults to having the front view face the opposite direction on the Z-Axis
	It is a lot easier to fix it here than to try to get an artist to fix the models...
	@param mesh partially loaded mesh
	*/
	void fixTrueSpaceVertices(Mesh *mesh) const;

	/**
	Removes and frees degenerate meshes within the model
	@param mesh partially loaded mesh
	*/
	void cullDegenerateMeshes(Engine::Model &model) const;

	/**
	Generate surface normals for the loaded vertices and faces
	@param mesh partially loaded mesh
	*/
	void generateNormals(Mesh *mesh) const;

	/**
	Reads the objects face information
	@param currentChunk Chunk data
	@param wrapper partially loaded mesh
	*/
	void readVertexIndices(Chunk &currentChunk, Wrapper &wrapper, Mesh *mesh) const;

	/**
	Reads the texture coordinates of the object
	@param currentChunk Chunk data
	@param mesh partially loaded mesh
	*/
	void readUVCoordinates(Chunk &currentChunk, Mesh *mesh) const;

	/**
	Reads in the material name assigned to the object and sets the materialID
	@param currentChunk Chunk data
	@param wrapper partially loaded model
	@param mesh partially loaded mesh
	*/
	void readObjectMaterial(Chunk &currentChunk, Wrapper &wrapper, Mesh *mesh) const;

	/**
	Reads the material name
	@param currentChunk chunk data
	@param material current material
	*/
	void readMaterialName(Chunk &currentChunk, Material &material) const;

	/**
	Reads the material texture file
	@param currentChunk chunk data
	@param material current material
	*/
	void readMaterialMapFile(Chunk &currentChunk, Material &material) const;

	/**
	Forces the skin to the specified value
	@param model The model to alter
	@param skinFileName The filename of the skin
	*/
	void forceSkin(Engine::Model &model, const string &skinFileName) const;
};

} // namespace Engine

#endif
