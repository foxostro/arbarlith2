/*
Modified by Andrew Fox in 2003-2006
Original Source: McCuskey, Mason. "Game Programming Tricks of the Trade". "Trick 15: Serialization Using XML Property Bags". Premier Press. 2002.
E-Mail: mailto:andrewfox@cmu.edu
*/

#ifndef PRIMITIVEDATATYPES_H__INCLUDED
#define PRIMITIVEDATATYPES_H__INCLUDED

#include "misc.h"

namespace Engine
{
	/** Object may be represented by a string and retrieved from a string representation */
	class XmlDataType
	{
	public:
        /** Default construtor */
        XmlDataType(void)
        {}

		/**
		Gets the string representation of the object
		@return string representation
		*/
		virtual _tstring ToString() const = 0;

		/**
		Retrieve from a string representation
		@param str string representation
		@return true if successful, false if otherwise.  Ususally, this means that the given string representation was invalid
		*/
		virtual bool FromString(_tstring str) = 0;

		/**
		Cast to a string
		@return string representation
		*/
		operator _tstring ()
		{
			return ToString();
		}
	};

} // namespace Engine

#endif
