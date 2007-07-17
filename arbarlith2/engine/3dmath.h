/*
Modified in 2004-2007 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Original Author:
Ben Humphrey (DigiBen)
DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com
*/

#ifndef _3DMATH_H_
#define _3DMATH_H_

#include "misc.h"
#include "vec4.h"
#include "mat4.h"

namespace Engine {

// This takes a triangle (plane) and line and returns true if they intersected
bool IntersectedPlane(vec3 vPoly[], vec3 vLine[], vec3 &vNormal, float &originDistance);

// This returns an intersection point of a polygon and a line (assuming intersects the plane)
vec3 IntersectionPoint(vec3 vNormal, vec3 vLine[], double distance);

} // namespace Engine

#endif
