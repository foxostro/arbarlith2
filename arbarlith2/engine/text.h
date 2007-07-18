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

#ifndef _TEXT_H_
#define _TEXT_H_

#include "COLOR.h"
#include "vec4.h"
#include "PropertyBag.h"

namespace Engine {

/** Type for definition the dimensions of a character on-screen */
enum FONT_SIZE
{
	FONT_SIZE_HUGE,
	FONT_SIZE_LARGE,
	FONT_SIZE_NORMAL,
	FONT_SIZE_SMALL
};

/**
Manages a single font and can print strings to the screen with this font
*/
class TextWriter
{
private:
	/** Character in the font ad measurements associated with it */
	class Character
	{
	public:
		/** Constructor */
		Character(void)
		{
			clear();
		}

		/** Clears out the character data */
		void clear(void)
		{
			charTex = 0;
			width = 1.0f;
			left = 0.0f;
		}

		/** The texture handle of the character */
		unsigned int charTex;

		/** Width of the character from the leftmost edge of the glyph to the rightmost edge (0.0 to 1.0, a portion of the Quad Width) */
		float width;

		/** Distance from the edge of the quad to the leftmost edge of the glyph (0.0 to 1.0, a portion of the Quad Width) */
		float left;
	};

	/** the font setup file */
	_tstring fileName;

	/** Characters in the font */
	Character characters[256];

	/** Height of a line (as a portion of the Quad Height) */
	float lineHeight;

	/** Associates the FONT_SIZE tokens with an actual size measurement */
	map<FONT_SIZE, float> fontSize;

	/** Disable extra texture units */
	static void disableMultiTex(void);

	/**
	Prints a single character to the screen
	@param a Corner of the character's quad
	@param b Corner of the character's quad
	@param c Corner of the character's quad
	@param d Corner of the character's quad
	@param z The character to print out
	@param color The color of the text
	@param size The size of the text
	*/
	void drawChar(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, TCHAR z, const COLOR &color) const;

	/**
	Prints a single character to the screen and advances the cursor
	@param offset The position of the chracter on the screen.  Returns the position of the next character.
	@param character The character to print out
	@param color The coor of the text
	@param size The size of the text
	@param useBillboard Should use a billboard when displaying
	*/
	void putChar(vec3 *offset, TCHAR character, const COLOR *color, FONT_SIZE size, bool useBillboard) const;

	/**
	Gets character information
	@param z Character
	@return Character information
	*/
	Character& getCharacter(char z);

	/**
	Gets character information
	@param z Character
	@return Character information
	*/
	const Character& getCharacter(char z) const;

	/**
	Gets character information
	@param z Character
	@return Character information
	*/
	Character& getCharacter(wchar_t z);

	/**
	Gets character information
	@param z Character
	@return Character information
	*/
	const Character& getCharacter(wchar_t z) const;

public:
	/** The origin of the font characters */
	vec3 origin;

	/** Constructor */
	TextWriter(void);

	/**
	Setup the font
	@param fileName XML source for the font data
	*/
	void setup(const _tstring &fileName);

	/**
	Setup the font
	@param xml XML source for the font data
	*/
	void setup(PropertyBag &xml);

	/** reloads all font textures again */
	void reaquire(void);

	/** frees all textures from memory */
	void release(void);

	/**
	Prints out a _tstring
	@param text The text to print
	@param color The coor of the text
	@param size The size of the text
	@param useBillboard Should use a billboard when displaying
	*/
	void write(const _tstring &text, const COLOR &color, FONT_SIZE size, bool useBillboard=false) const;

	/**
	Gets the dimensions of the given string
	@param text text
	@param size	The size of the text
	@return (width, height) of the text
	*/
	vec2 getDimensions(const _tstring &text, FONT_SIZE size) const;

	/**
	Gets the height of a line of text in the given size
	@return The height of a line of text
	*/
	float getLineHeight(FONT_SIZE size) const;
};


} // namespace Engine


#endif
