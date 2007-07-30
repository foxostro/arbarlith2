/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2005-2007 Game Creation Society
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

#ifndef _VEC4_H_
#define _VEC4_H_

#include "misc.h"
#include "primitivedatatypes.h"

namespace Engine {

/**
Represents a point in two dimensional space
@author Andrew Fox
@version July 2005
*/
struct Point2
{
	/**
	Default constructor sets all coordinates to zero
	*/
	Point2(void)
	{
		x = 0.0f;
		y = 0.0f;
	}

	float x, y;
};

/**
Represents a point in three dimensional space
@author Andrew Fox
@version July 2005
*/
struct Point3
{
	/**
	Default constructor sets all coordinates to zero
	*/
	Point3(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/**
	Constructs the point from an array of doubles
	@param v The point
	*/
	Point3(double *v)
	{
		x = (float)v[0];
		y = (float)v[1];
		z = (float)v[2];
	}

	float x, y, z;
};

/**
Represents a point in four dimensional space
@author Andrew Fox
@version July 2005
*/
struct Point4
{
	/**
	Default constructor sets all coordinates to zero
	*/
	Point4(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	float x, y, z, w;
};

/**
Represents a mathematical vector in space up to four dimensions.
@author Andrew Fox
@version January 2006
*/
class vec4 : public XmlDataType
{
public:
	float x, y, z, w;

	/**	Default constructor. Sets all coordinates to zero */
	vec4(void)
	: XmlDataType()
	{
	    x = 0.0f;
	    y = 0.0f;
	    z = 0.0f;
	    w = 0.0f;
    }

	/**
	Copy constructor
	@param vec The vector to copy
	*/
	vec4(const vec4 &vec)
	: XmlDataType()
	{
		set(vec);
	}

	/**
	Creates the vector from a two dimensional point.
	The higher dimensional coordinates are set to zero.
	@param p The point
	*/
	vec4(const Point2 &p)
	: XmlDataType()
	{
		set(p);
	}

	/**
	Creates the vector from a three dimensional point.
	The higher dimensional coordinates are set to zero.
	@param p The point
	*/
	vec4(const Point3 &p)
	: XmlDataType()
	{
		set(p);
	}

	/**
	Creates the vector from a four dimensional point.
	@param p The point
	*/
	vec4(const Point4 &p)
	: XmlDataType()
	{
		set(p);
	}

	/**
	Creates the vector directly from a series of coordinates
	@param x X-Coordinate
	@param y Y-Coordinate
	@param z Z-Coordinate
	@param w W-Coordinate
	*/
	vec4(float x, float y, float z=0.0f, float w=0.0f)
	: XmlDataType()
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	Calculates the magnitde of the vector, squared.
	|v|^2

	@return The magnitude of the vector, squared^2.
	*/
	inline float getMagnitudeSqr(void) const
	{
		return getX()*getX() + getY()*getY() + getZ()*getZ();
	}

	/**
	Calculates the magnitude of the vector.
	L = |v|

	@return The magnitude of the vector.
	*/
	inline float getMagnitude(void) const
	{
		return sqrtf(getMagnitudeSqr());
	}

	/**
	Calculates the unit-length vector.
	v = v1 / |v1|

	@return The unit-length vector.
	*/
	inline const vec4 getNormal(void) const
	{
		return scale(1.0f / getMagnitude());
	}

	/**
	Calculates the vector after scaling by a constant
	v = v1 * s

	@param scalar The scalar constant
	@return The scaled vector.
	*/
	inline const vec4 scale(float scalar) const
	{
		return vec4(getX()*scalar, getY()*scalar, getZ()*scalar, getW()*scalar);
	}

	/**
	Calculates the vector after adding a constant to it.
	v = v1 + c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec4 add(float value) const
	{
		return vec4(getX()+value, getY()+value, getZ()+value, getW()+value);
	}

	/**
	Calculates the vector after adding another vector to it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec4 add(const vec4& vec) const
	{
		return vec4(getX()+vec.getX(), getY()+vec.getY(), getZ()+vec.getZ(), getW()+vec.getW());
	}

	/**
	Calculates the vector after subtracting a constant from it.
	v = v1 - c

	@param value The constant value
	@return The resultant vector.
	*/
	inline const vec4 subtract(float value) const
	{
		return vec4(getX()-value, getY()-value, getZ()-value, getW()-value);
	}

	/**
	Calculates the vector after subtracting another vector from it.
	v = v1 + v2

	@param vec The other vector
	@return The resultant vector.
	*/
	inline const vec4 subtract(const vec4& vec) const
	{
		return vec4(getX()-vec.getX(), getY()-vec.getY(), getZ()-vec.getZ(), getW()-vec.getW());
	}

	/**
	Calculates the vector after performing a cross product operation with it and another vector.
	Determines the vector orthoganal to this vector and the given vector.

	v = < (v1.y * v2.z) - (v1.z * v2.y), (v1.z * V2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x) >

	@param vec The other vector
	@return The resultant vector.
	*/
	inline vec4 cross(const vec4 &vec) const
	{
		Point3 crossProduct;

		crossProduct.x = (getY() * vec.getZ()) - (getZ() * vec.getY());
		crossProduct.y = (getZ() * vec.getX()) - (getX() * vec.getZ());
		crossProduct.z = (getX() * vec.getY()) - (getY() * vec.getX());

		return vec4(crossProduct);
	}

	/**
	Calculates the value after performing a dot product operation with this vector and another vector.

	v1.v2 = |v1| |v2| cos theta

	@param vec The other vector
	@return The dot product
	*/
	inline float dot(const vec4 &vec) const
	{
		return (getX() * vec.getX()) + (getY() * vec.getY()) + (getZ() * vec.getZ()) + (getW() * vec.getW());
	}

	/**
	Gets the distance between this vector and the other vector, squared.
	@param vec The other vector
	@return The distance, squared
	*/
	inline float distanceSqr(const vec4 &vec) const
	{
		float dx = getX() - vec.getX();
		float dy = getY() - vec.getY();
		float dz = getZ() - vec.getZ();

		return(dx*dx + dy*dy + dz*dz);
	}

	/**
	Gets the distance between this vector and the other vector.
	@param vec The other vector
	@return The distance
	*/
	inline float distance(const vec4 &vec) const
	{
		return sqrtf(distanceSqr(vec));
	}

	/**
	Gets the x-coordinate of this vector.
	@return The x-coordinate
	*/
	inline float getX(void) const
	{
		return x;
	}

	/**
	Gets the y-coordinate of this vector.
	@return The y-coordinate
	*/
	inline float getY(void) const
	{
		return y;
	}

	/**
	Gets the z-coordinate of this vector.
	@return The z-coordinate
	*/
	inline float getZ(void) const
	{
		return z;
	}

	/**
	Gets the w-coordinate of this vector.
	@return The w-coordinate
	*/
	inline float getW(void) const
	{
		return w;
	}

public:
	/**
	Cast to floating point array
	@return array
	*/
	operator float* () const
	{
		return (float*)(this);
	}

	/**
	Sets the vector to the value on the right-hand side
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator=(const vec4 &rh)
	{
		set(rh);
		return(*this);
	}

	/**
	Sets the vector to the value on the right-hand side
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator=(const Point2 &rh)
	{
		set(rh);
		return(*this);
	}

	/**
	Sets the vector to the value on the right-hand side
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator=(const Point3 &rh)
	{
		set(rh);
		return(*this);
	}

	/**
	Sets the vector to the value on the right-hand side
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator=(const Point4 &rh)
	{
		set(rh);
		return(*this);
	}

	float operator[](size_t idx) const
	{
		switch(idx)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: return 0.0f;
		};
	}

	float& operator[](size_t idx)
	{
		switch(idx)
		{
		case 0:  return x;
		case 1:  return y;
		case 2:  return z;
		case 3:  return w;
		default: return w;
		};
	}

	/**
	Subtracts the right-hand side from this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator - (vec4 rh) const
	{
		return subtract(rh);
	}

	/**
	Adds the right-hand side to this vector.
	@param rh The right-hand side
	@return The new vector
	*/
	vec4 operator + (vec4 rh) const
	{
		return add(rh);
	}

	/**
	Scales this vector by the value on the right-hand side.
	@param scalar The right-hand side scalar
	@return The new vector
	*/
	vec4 operator * (float scalar) const
	{
		return scale(scalar);
	}

	/**
	Determines boolean equality between this vector and another vector on the right-hand side.
	@param rh The right-hand side
	@return The new vector
	*/
	bool operator == (vec4 rh) const
	{
		return (getX()==rh.getX() && getY()==rh.getY() && getZ()==rh.getZ());
	}

	/**
	Negates this vector
	@return The new vector
	*/
	vec4 operator - (void) const
	{
		return vec4(-getX(), -getY(), -getZ());
	}

public:
	/**
	Resets all coordinates in this vector to zero
	*/
	void zero(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	/**
	Sets the vector to the given value.
	@param vec The other value
	*/
	void set(const vec4 &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	/**
	Sets the vector to the given value.
	@param p The other value
	*/
	void set(const Point2 &p)
	{
		x = p.x;
		y = p.y;
		z = 0.0f;
		w = 0.0f;
	}

	/**
	Sets the vector to the given value.
	@param p The other value
	*/
	void set(const Point3 &p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		w = 0.0f;
	}

	/**
	Sets the vector to the given value.
	@param p The other value
	*/
	void set(const Point4 &p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		w = p.w;
	}

	/**
	Sets the vector to the given value.
	@param x The new x-value
	@param y The new y-value
	@param z The new z-value
	@param w The new w-value
	*/
	void set(float x, float y, float z=0.0f, float w=0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	Sets this vector to the unit-length version of itself.
	*/
	void normalize(void)
	{
		(*this) = getNormal();
	}

	/**
	Retrieves the vector from a _tstring reresentation
	@param str The _tstring representation of the vector
	@return true if successful, false otherwise.
	*/
	bool FromString(_tstring str)
	{
		vector<_tstring> tokens;

		str = toLowerCase(str); // establishes case-insensitivity

		Tokenize(str, tokens, _T("(,)"));
		if(tokens[0] == _T("&vec"))
        {
			x = stof(tokens[1]);
			y = stof(tokens[2]);
			z = stof(tokens[3]);
			w = 0.0f;

			return(true);
		}
		return(false);  // unknown format
	}

	/**
	Serializes the vector as a _tstring
	@return The _tstring to represent the vector
	*/
	_tstring ToString(void) const
	{
		return _tstring(_T("&vec(")) + ftoa(x, 4) + _T(", ") + ftoa(y, 4) + _T(", ") + ftoa(z, 4) + _T(")");
	}
};

/**
Two-Dimensional vector
@author Andrew Fox
@version July 2005
*/
typedef vec4 vec2;

/**
Three-Dimensional vector
@author Andrew Fox
@version July 2005
*/
typedef vec4 vec3;


} // namespace Engine



#endif
