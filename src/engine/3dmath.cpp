/*
Modified in 2004-2007 by Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Original Author:
Ben Humphrey (DigiBen)
DigiBen@GameTutorials.com
Co-Web Host of www.GameTutorials.com
*/

#include "stdafx.h"
#include "3dmath.h"

namespace Engine {

bool vec4::FromString(string _s)
{
    string s = toLowerCase(_s);

    boost::char_separator<char> delimeters("(,)\t\n");
    boost::tokenizer<boost::char_separator<char> > tokens(s, delimeters);

    boost::tokenizer<boost::char_separator<char> >::const_iterator i = tokens.begin();

	if(i == tokens.end())
        return false;
    
    if(string(*i) != "&vec")
        return false;

    x = stof(*(++i));
	y = stof(*(++i));
	z = stof(*(++i));
	w = 0.0f;

	return true;
}

bool IntersectedPlane(vec3 vPoly[], vec3 vLine[], vec3 &vNormal, float &originDistance)
{
	float distance1=0, distance2=0; // The distances from the 2 points of the line from the plane

	vNormal = vec3(vPoly[2] - vPoly[0]).cross(vec3(vPoly[1] - vPoly[0])).getNormal();

	// Let's find the distance our plane is from the origin.  We can find this value
	// from the normal to the plane (polygon) and any point that lies on that plane (Any vertex)
	originDistance = -vNormal.dot(vPoly[0]);

	// Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)

	distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
		         (vNormal.y * vLine[0].y)  +					// Bx +
				 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D

	// Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)

	distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
		         (vNormal.y * vLine[1].y)  +					// Bx +
				 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

	// Now that we have 2 distances from the plane, if we times them together we either
	// get a positive or negative number.  If it's a negative number, that means we collided!
	// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).

	if(distance1 * distance2 >= 0)			// Check to see if both point's distances are both negative or both positive
	   return false;						// Return false if each point has the same sign.  -1 and 1 would mean each point is on either side of the plane.  -1 -2 or 3 4 wouldn't...

	return true;							// The line intersected the plane, Return TRUE
}

vec3 IntersectionPoint(vec3 vNormal, vec3 vLine[], double distance)
{
	vec3 vPoint, vLineDir;					// Variables to hold the point and the line's direction
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	// 1)  First we need to get the vector of our line, Then normalize it so it's a length of 1
	vLineDir = vLine[1] - vLine[0];		// Get the vec3 of the line
	vLineDir.normalize();				// normalize the lines vector


	// 2) Use the plane equation (distance = Ax + By + Cz + D) to find the
	// distance from one of our points to the plane.
	Numerator = - (vNormal.x * vLine[0].x +		// Use the plane equation with the normal and the line
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);

	// 3) If we take the dot product between our line vector and the normal of the polygon,
	Denominator = vNormal.dot(vLineDir);		// Get the dot product of the line's vector and the normal of the plane

	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (Normal.vec3 = 0)).
	// In this case, we should just return any point on the line.

	if( Denominator == 0.0)						// Check so we don't divide by zero
		return vLine[0];						// Return an arbitrary point on the line

	dist = Numerator / Denominator;				// Divide to get the multiplying (percentage) factor

	// Now, like we said above, we times the dist by the vector, then add our arbitrary point.
	vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

	return vPoint;								// Return the intersection point
}

} // namespace Engine
