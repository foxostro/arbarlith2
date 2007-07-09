/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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

#ifndef _MESH_H_
#define _MESH_H_

#include "3dmath.h"
#include "material.h"
#include "file.h"
#include "propbag.h"


namespace Engine {


/** Holds mesh data and can render that data */
class Mesh
{
public:
	/**
	Face structure.  This is is used for indexing into the vertex
	and texture coordinate arrays.  From this information we know which vertices
	from our vertex array go to which face, along with the correct texture coordinates.
	*/
	struct Face
	{
		/** indicies for the verts that make up this triangle */
		int vertIndex[3];

		/** indicies for the tex coords to texture this face */
		int coordIndex[3];
	};

	/**
	Default constructor
	*/
	Mesh(void)
	{
		clear();
	}

	/**
	Copy constructor
	@param obj The object to copy from
	*/
	Mesh(const Mesh &obj);

	/**
	Copy constructor
	@param obj The object to copy from
	*/
	Mesh(const Mesh *obj);

	/** Destructor */
	~Mesh(void)
	{
		destroy();
	}

	/**	Resets class member to a just-constructed state	*/
	void clear(void)
	{
		m_numOfVerts = 0;
		m_numOfFaces = 0;
		m_numTexVertex = 0;
		m_pVerts = 0;
		m_pTexVerts = 0;
		m_pFaces = 0;
		m_pElements = 0;
		m_pNormals = 0;
		memset(m_strName, 0, sizeof(char) * 255);
		material.clear();
	}

	/**	Cleanly destroys the object and clears data members */
	void destroy(void)
	{
		delete[] (m_pVerts);
		delete[](m_pTexVerts);
		delete[](m_pFaces);
		delete[](m_pElements);
		delete[](m_pNormals);

		material.destroy();
		clear();
	}

	/**
	Assignment operator
	@param obj The object to copy from
	@return The assigned value
	*/
	Mesh& operator=(const Mesh &obj);

	/** Draws the object */
	void draw(void) const;

	/**
	Calculates the radius of the smallest sphere that encloses the object.
	@return radius
	*/
	float calculateRadius(void);

	/**
	Calculates the radius of the smallest cylinder that encloses the object.
	@return radius
	*/
	float calculateCylindricalRadius(void);

	/**
	Returns the smallest bounding box that encloses the object.
	@return BoundingBox
	*/
	BoundingBox calculateBoundingBox(void);

	/** Reallocate and recreate the elements */
	void reallocElements(void);

	/**
	Creates an object interpolated from two other existing objects
	@param bias Interpolation bias between 0.0 and 1.0
	@param a The first keyframe
	@param b The second keyframe
	*/
	void interpolate(float bias, Mesh *a, Mesh *b);

	/**
	Calculates the FOV of the Mesh
	@param mat The camera
	@param lxMax Returns the horizontal angle of the Mesh
	@param lyMax Returns the vertical angle of the Mesh
	*/
	void calculateFOV(mat4 &mat, float &lxMax, float &lyMax) const;

public:
	int  m_numOfVerts;			 /** The number of verts in the model */
	int  m_numOfFaces;			 /** The number of faces in the model */
	int  m_numTexVertex;	     /** The number of texture coordinates */
	char m_strName[255];	     /** The name of the object */
	Point3  *m_pVerts;			 /** The object's vertices */
	Point3  *m_pNormals;	     /** Vertex Normals */
	Point2  *m_pTexVerts;		 /** The texture's UV coordinates */
	Face   *m_pFaces;		     /** The faces information of the object */

	/** The material to apply to the model */
	Material material;

	/** The object's elements array */
	unsigned int *m_pElements;

private:
	/**
	Copies the object
	@param obj The object to copy from
	*/
	void copy(const Mesh &obj);
};

} // namespace Engine

#endif
