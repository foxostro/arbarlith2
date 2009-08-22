/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2004-2007,2009 Game Creation Society
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

#include "stdafx.h" // Master Header
#include "gl.h"

#include "Md3Loader.h"
#include "3dsLoader.h"
#include "mesh.h"

#include "Effect.h"
#include "EffectManager.h"

#ifdef _WIN32
#define strcpy strcpy_s // avoid deprecation warnings and keep portable syntax
#endif


namespace Engine {


Mesh::Mesh(const Mesh *obj)
:m_pVerts(0),
m_pNormals(0),
m_pTexVerts(0),
m_pFaces(0),
m_pElements(0)
{
	copy(*obj);
}

Mesh::Mesh(const Mesh &obj)
:m_pVerts(0),
m_pNormals(0),
m_pTexVerts(0),
m_pFaces(0),
m_pElements(0)
{
	copy(obj);
}

Mesh &Mesh::operator=(const Mesh &obj)
{
	copy(obj);
	return(*this);
}

void Mesh::copy(const Mesh &obj)
{
	// Copy simple data members
	m_numOfVerts   = obj.m_numOfVerts;
	m_numOfFaces   = obj.m_numOfFaces;
	m_numTexVertex = obj.m_numTexVertex;

	// Free the old arrays
	delete(m_pVerts);
	delete(m_pNormals);
	delete(m_pTexVerts);
	delete(m_pFaces);
	delete(m_pElements);

	// Copy the vertices
	if(obj.m_pVerts != 0)
	{
		m_pVerts = new Point3[m_numOfVerts];
		memcpy(m_pVerts,    obj.m_pVerts,    sizeof(Point3) * m_numOfVerts);
	}

	// Copy the noramls
	if(obj.m_pNormals != 0)
	{
		m_pNormals = new Point3[m_numOfVerts];
		memcpy(m_pNormals,    obj.m_pNormals,    sizeof(Point3) * m_numOfVerts);
	}

	// Copy the tex coords
	if(obj.m_pTexVerts != 0)
	{
		m_pTexVerts = new Point2[m_numOfVerts];
		memcpy(m_pTexVerts, obj.m_pTexVerts, sizeof(Point2) * m_numOfVerts);
	}

	// Copy the faces
	if(obj.m_pFaces != 0)
	{
		m_pFaces = new Face[m_numOfFaces];
		memcpy(m_pFaces, obj.m_pFaces, sizeof(Face) * m_numOfFaces);
	}

	// Copy the name
	strcpy(m_strName, obj.m_strName);

	// Copy the material
	material = obj.material;

	// Process the model
	reallocElements();
}

void Mesh::draw(void) const
{
	Effect &currentEffect = EffectManager::GetSingleton().getEffect();

	CHECK_GL_ERROR();

	material.bind();
	currentEffect.passVertexStream((float*)m_pVerts);
	currentEffect.passNormalStream((float*)m_pNormals);
	currentEffect.passTexCoordStream((float*)m_pTexVerts, 0);
	glDrawElements(GL_TRIANGLES, m_numOfFaces*3, GL_UNSIGNED_INT, m_pElements);

	FLUSH_GL_ERROR();
}

void Mesh::reallocElements()
{
	// Destroy the old array
	delete[](m_pElements);

	// Create a new elements array
	m_pElements = new unsigned int[m_numOfFaces*3];

	for(int i=0, j=0; i<m_numOfFaces; ++i, j+=3)
	{
		Face &face = m_pFaces[i];

		m_pElements[j+0] = face.vertIndex[0];
		m_pElements[j+1] = face.vertIndex[1];
		m_pElements[j+2] = face.vertIndex[2];
	}
}

float Mesh::calculateCylindricalRadius()
{
	float furthest = 0.0f;
	float closest = 0.0f;

	for(size_t i=0; i<(size_t)m_numOfVerts; ++i)
	{
		float radius = sqrtf(  SQR(m_pVerts[i].x) + SQR(m_pVerts[i].z)  );

		furthest = max(furthest, radius);
		closest = min(closest, radius);
	}

	return(furthest-closest);
}

float Mesh::calculateRadius()
{
	double furthest = 0.0;

	vec3 centerPt(0,0,0);

	// Find the center point
	for(int i=0; i<m_numOfVerts; ++i)
	{
		centerPt = centerPt + m_pVerts[i];
	}
	centerPt = centerPt * (1.0f / (float)m_numOfVerts);

	// Find the point furthest from the center point
	for(int i=0; i<m_numOfVerts; ++i)
	{
		double radius = sqrt(  SQR(m_pVerts[i].x-centerPt.x) + SQR(m_pVerts[i].y-centerPt.y) + SQR(m_pVerts[i].z-centerPt.z)  );

		furthest = max(furthest, radius);
	}

	return (float)(furthest);
}

BoundingBox Mesh::calculateBoundingBox()
{
	BoundingBox box;

	vec3 &boxMax = box.m_Max;
	vec3 &boxMin = box.m_Min;

	for(int i=0; i<m_numOfVerts; ++i)
	{
		boxMax.x = max(boxMax.x, m_pVerts[i].x);
		boxMax.y = max(boxMax.y, m_pVerts[i].y);
		boxMax.z = max(boxMax.z, m_pVerts[i].z);

		boxMin.x = min(boxMin.x, m_pVerts[i].x);
		boxMin.y = min(boxMin.y, m_pVerts[i].y);
		boxMin.z = min(boxMin.z, m_pVerts[i].z);
	}

	box.m_Pos = (boxMax +boxMin) * 0.5f;

	return box;
}

void Mesh::interpolate(float bias, Mesh *pA, Mesh *pB)
{
	ASSERT(pA!=0, "Mesh::interpolate  ->  pA was null");
	ASSERT(pB!=0, "Mesh::interpolate  ->  pB was null");

	ASSERT(m_numOfVerts == pA->m_numOfVerts, "Mesh::interpolate  ->  Different number of vertices from keyframe A");
	ASSERT(m_numOfVerts == pB->m_numOfVerts, "Mesh::interpolate  ->  Different number of vertices from keyframe B");

	ASSERT(m_numOfFaces == pA->m_numOfFaces, "Mesh::interpolate  ->  Different number of faces from keyframe A");
	ASSERT(m_numOfFaces == pB->m_numOfFaces, "Mesh::interpolate  ->  Different number of faces from keyframe B");

	// Set some pointers up for the arrays to avoid dereferencing pA and pB a million times a frame
	unsigned int *elA = pA->m_pElements;
	unsigned int *elB = pB->m_pElements;
	Point3 *vertsA = pA->m_pVerts;
	Point3 *vertsB = pB->m_pVerts;
	Point3 *normsA = pA->m_pNormals;
	Point3 *normsB = pB->m_pNormals;

	// Linear Interpolation  ...   p(t) = p0 + t(p1 - p0)
	for(int i=0; i<m_numOfFaces*3; ++i)
	{
		unsigned int a = elA[i];
		unsigned int b = elB[i];

		// Make sure conectivity is the same as one of the keyframes
		m_pElements[i] = a;
		unsigned int x = m_pElements[i];

		Point3 &vertexA = vertsA[a];
		Point3 &normalA = normsA[a];

		Point3 &vertexB = vertsB[b];
		Point3 &normalB = normsB[b];

		// Interpolate Vertices
		m_pVerts[x].x = vertexA.x + bias*(vertexB.x - vertexA.x);
		m_pVerts[x].y = vertexA.y + bias*(vertexB.y - vertexA.y);
		m_pVerts[x].z = vertexA.z + bias*(vertexB.z - vertexA.z);

		// Interpolate Normals
		m_pNormals[x].x = normalA.x + bias*(normalB.x - normalA.x);
		m_pNormals[x].y = normalA.y + bias*(normalB.y - normalA.y);
		m_pNormals[x].z = normalA.z + bias*(normalB.z - normalA.z);
	}
}

void Mesh::calculateFOV(mat4 &mat, float &lxMax, float &lyMax) const
{
	vec3 vert1, vert2;

	// For each vertex
	for(int i=0; i<m_numOfVerts; ++i)
	{
		// Transform the vertex by the matrix
		vert1 = m_pVerts[i];
		vert1.w = 1.0f;
		vert2 = mat.transformVector(vert1);

		// Calculate the spread, keep the max
		lxMax = max(lxMax, fabsf(vert2.x / vert2.z));
		lyMax = max(lyMax, fabsf(vert2.y / vert2.z));
	}
}

}; // namespace
