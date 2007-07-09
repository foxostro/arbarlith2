/*
Modified in 2004-2006 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Original Author:
Ben Humphrey (DigiBen)
DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com
*/

#ifndef _3DMATH_H_
#define _3DMATH_H_

#include "misc.h"
#include "primitivedatatypes.h" // for XmlDataType
#include "vec4.h"
#include "mat4.h"

namespace Engine {

/**
Represents the intersection of a ray with a plane or face
@author Andrew Fox
@version March 2005
*/
class Intersection
{
public:
	/** Position of the intersection*/
	vec3 p;

	/** Normal of the intersection plane */
	vec3 n;

	/** Vector tangent to the intersection plane */
	vec3 t;

	/**
	Gets the transformation frame of the intersection with the colliding plane
	@return The transformation frame of the intersection, relative to the colliding plane
	*/
	inline mat4 getFrame(void) const
	{
		return mat4(p, t, n);
	}
};

// These constants are used as return values from ClassifySphere().  Depending
// on where the sphere lies in accordance with the plane being checked, these
// will allow us to label if the sphere is in front, behind or intersecting the plane.

#define SPH_BEHIND		0	// This is returned if the sphere is completely behind the plane
#define SPH_INTERSECTS	1	// This is returned if the sphere intersects the plane
#define SPH_FRONT		2	// This is returned if the sphere is completely in front of the plane

//	This returns the normal of a polygon (The direction the polygon is facing)
vec3 Normal(vec3 vPolygon[]);

// This returns the distance between 2 3D points
float Distance(vec3 vPoint1, vec3 vPoint2);

// This returns the point on the line segment vA_vB that is closest to point vPoint
vec3 ClosestPointOnLine(vec3 vA, vec3 vB, vec3 vPoint);

// This returns the distance the plane is from the origin (0, 0, 0)
// It takes the normal to the plane, along with ANY point that lies on the plane (any corner)
float PlaneDistance(vec3 Normal, vec3 Point);

// This takes a triangle (plane) and line and returns true if they intersected
bool IntersectedPlane(vec3 vPoly[], vec3 vLine[], vec3 &vNormal, float &originDistance);

// This returns the angle between 2 vectors
double AngleBetweenVectors(vec3 Vector1, vec3 Vector2);

// This returns an intersection point of a polygon and a line (assuming intersects the plane)
vec3 IntersectionPoint(vec3 vNormal, vec3 vLine[], double distance);

// This returns true if the intersection point is inside of the polygon
bool InsidePolygon(vec3 vIntersection, vec3 Poly[], long verticeCount);

// Use this function to test collision between a line and polygon
bool IntersectedPolygon(vec3 vPoly[], vec3 vLine[], int verticeCount);

// This function classifies a sphere according to a plane.  The information returned
// tells us if the sphere is BEHIND, in FRONT, or INTERSECTS the plane.  This takes
// the sphere's center, the plane's normal, a point on the plane, the sphere's radius
// and a referenced variable to hold the distance.  This way we can return the distance
// and the sphere's relationship to the plane.  The distance is from the plane to the center
// of the sphere.  With this information it enables us to offset the sphere if needed.
int ClassifySphere(vec3 &vCenter,
				   vec3 &vNormal, vec3 &vPoint, float radius, float &distance);

// This takes in the sphere center, radius, polygon vertices and vertex count.
// This function is only called if the intersection point failed.  The sphere
// could still possibly be intersecting the polygon, but on it's edges.
bool EdgeSphereCollision(vec3 &vCenter,
						 vec3 vPolygon[], int vertexCount, float radius);

// This returns true if the sphere is intersecting with the polygon.
// The parameters are the vertices of the polygon, vertex count, along with the center
// and radius of the sphere.
bool SpherePolygonCollision(vec3 vPolygon[],
							vec3 &vCenter, int vertexCount, float radius);

/////////////////////////////////////////////////////////////////////////////////
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//


/*
Name:   RayIntersectTriangle
Pre:    Accepts a ray, a triangle, and a vector object to return the normal in
Post:   Returns true if the given ray intersects the given triangle
        The "normal" variable is filled with the normal of the triangle

Origin: "Game Programming Tricks of the Trade" chapter 18 source code
Author: Richard Benson
Email:  rbenson@earthlink.net
*/
bool RayIntersectTriangle(vec3& P, vec3& Dir, vec3& v0, vec3& v1, vec3& v2, float& height, vec3& normal);

/**
Determines whether a ray ha intersected the given sphere
@param rayPoint A point on the ray
@param rayDirection The direction of the ray
@param sphereCenter The center point of the ray
@param sphereRadius The radius of the sphere
@author Andrew Fox
*/
bool rayDoesIntersectSphere(const vec3 &rayPoint, const vec3 &rayDirection, const vec3 &sphereCenter, float sphereRadius);

/**
Bounding boxes are used to represent the
volume of an object and test for collisions
@author Andrew Fox
*/
class BoundingBox
{
public:
	/** Position of the box */
	vec3 m_Pos;

	/** Minimum vertex of the box relative to m_Pos */
	vec3 m_Min;

	/** Maximum vertex of the box relative to m_Pos*/
	vec3 m_Max;

	/*
	Determines whether a collision between two boxes has occurred
	@param box The bounding box to test against
	@return Returns true if this bounding box intersects the given bounding box, otherwise, returns false.
	*/
	bool Collision(BoundingBox &box);

	/**
	Determines whether the point is within the vertical projection of the box
	@param p The point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(vec2 p)
	{
		return Collision(p.x, p.y);
	}
	/**
	Determines whether the point is within the vertical projection of the box
	@param x The x-coordinate of the point to test
	@param z The y-coordinate of the point to test
	@return Returns true if the point resides within the vertical projection of the box and false otherwise
	*/
	bool Collision(float x, float z);

	/**
	Tests whether a sphere intersects the bounding box
	@param center The center of the sphere
	@param radius The radius of the sphere
	@return true if the two intersect
	*/
	bool testBoxVersusSphere(const vec3 &center, float radius) const;

	/**
	Saves the bounding box to xml
	@param xml The xml source to save into
	@return true if sucessful, false otherwise
	*/
	bool SaveXml(CPropBag &xml);

	/**
	Loads the bounding box from xml
	@param xml The xml source to load from
	@return true if sucessful, false otherwise
	*/
	bool LoadXml(CPropBag &xml);
};

} // namespace Engine

#endif
