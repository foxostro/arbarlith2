/*
Modified in 2005-2007 by Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Original Author:
Ben Humphrey (DigiBen)
DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com
*/

#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "vec4.h"
#include "mat4.h"

namespace Engine {

/**
We create an enum of the sides so we don't have to call each side 0 or 1.
This way it makes it more understandable and readable when dealing with frustum sides.
*/
enum FrustumSide
{
	RIGHT	= 0,
	LEFT	= 1,
	BOTTOM	= 2,
	TOP	= 3,
	BACK	= 4,
	FRONT	= 5
};

/** This will allow us to create an object to keep track of our frustum */
class Frustum
{
public:
	// Calculates the frustum from the modelview/projection frustum currently set
	void CalculateFrustum(void);

	// Calculates the frustum from the given matrices
	void CalculateFrustum(const mat4 &modl, const mat4 &proj);

	// This takes a 3D point and returns TRUE if it's inside of the frustum
	bool PointInFrustum(float x, float y, float z) const;

	// This takes the center and half the length of the cube.
	bool CubeInFrustum( float x, float y, float z, float size ) const;

	// This takes a 3D point and a sphere diameter and returns TRUE if the sphere is inside of the frustum
	bool SphereInFrustum(float x, float y, float z, float diameter) const;

	// This takes a 3D point and a sphere diameter and returns TRUE if the sphere is inside of the frustum
	inline bool SphereInFrustum2(const vec4 &sphereCenter, float diameter) const
	{
		for(int i = 0; i < 6; i++)
		{
			if( m_Frustum[i][0] * sphereCenter.x +
				m_Frustum[i][1] * sphereCenter.y +
				m_Frustum[i][2] * sphereCenter.z +
				m_Frustum[i][3] <= -diameter )
			{
				return false;
			}
		}

		return true;
	}

	/**
	Gets the plane equation of a single side of the frustum
	@param side The side of the frustum to return
	@return The plane equation of that side of the frustum
	*/
	inline vec4 getPlane(FrustumSide side) const
	{
		return vec4(m_Frustum[side][0],
					m_Frustum[side][1],
					m_Frustum[side][2],
					m_Frustum[side][3]);
	}

	/** Begin clipping against the frustum */
	void beginClipping(void) const;

	Frustum& operator=(const Frustum &o);

private:
	/** This holds the A B C and D values for each side of our frustum. */
	float m_Frustum[6][4];
};

} // namespace Engine

#endif

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES *
//
// This file holds the frustum class prototypes.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
