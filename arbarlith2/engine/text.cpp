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

#include "stdafx.h" // Master Header
#include "gl.h"
#include "file.h"
#include "opengl.h"
#include "text.h"
#include "image.h"
#include "mat4.h"

namespace Engine {

TextWriter::TextWriter(void)
{
	fileName = "nill";

	fontSize.insert(make_pair(FONT_SIZE_HUGE, 24.f));
	fontSize.insert(make_pair(FONT_SIZE_LARGE, 16.f));
	fontSize.insert(make_pair(FONT_SIZE_NORMAL, 12.f));
	fontSize.insert(make_pair(FONT_SIZE_SMALL, 8.f));

	//spacing = 1.20f;
	lineHeight = 1.20f;
}

void TextWriter::release(void)
{
	for(int i=0; i<256; ++i)
	{
		glDeleteTextures(1, &characters[i].charTex);
		characters[i].charTex = 0;
	}
}

void TextWriter::reaquire(void)
{
	setup(fileName);
}

void TextWriter::setup(const string &fileName)
{
	this->fileName = fileName;

	PropertyBag xml;
	xml.loadFromFile(fileName);
	setup(xml);
}

void TextWriter::setup(PropertyBag &xml)
{
	PropertyBag FontBag;
	string fontImageFileName = File::fixFilename("data/fonts/default.png"); // default font image

	// Load the font configuration files
	xml.get("font", FontBag);
	FontBag.get("image", fontImageFileName);

	fontImageFileName = File::fixFilename(fontImageFileName);

	// Default font sizes
	float h = 24.f;
	float l = 16.f;
	float n = 12.f;
	float s = 8.f;

	// Load the font sizes
	FontBag.get("huge", h);
	FontBag.get("large", l);
	FontBag.get("normal", n);
	FontBag.get("small", s);

	fontSize.clear();
	fontSize.insert(make_pair(FONT_SIZE_HUGE, h));
	fontSize.insert(make_pair(FONT_SIZE_LARGE, l));
	fontSize.insert(make_pair(FONT_SIZE_NORMAL, n));
	fontSize.insert(make_pair(FONT_SIZE_SMALL, s));

	/*spacing = 0.75f;
	FontBag.getSym(spacing);*/

	lineHeight = 1.20f;
	FontBag.getSym(lineHeight);

	// Load the font image into a buffer
	Image fontImage(fontImageFileName);
	unsigned char *font	= fontImage.getImage();
	int fontWidth		= fontImage.getWidth();
	int fontHeight		= fontImage.getHeight();
	int bytesPerPixel	= fontImage.getDepth();
	bool alpha		= bytesPerPixel==4;

	ASSERT(font!=0,		"font was NULL");
	ASSERT(fontWidth>16,	"font width is too narrow");
	ASSERT(fontHeight>16,	"font height is too narrow");

	// The number of columns and rows of characters conatined in the font
	const int numCols = 16;
	const int numRows = 16;

	// Each character in the font has these dimensions
	GLint width = fontWidth / 16;
	GLint height = fontHeight / 16;

	// Create a buffer
	size_t size = width * height * bytesPerPixel;
	unsigned char *buffer = new unsigned char[size];
	memset(buffer, 0, size);

	// Create sub textures for each character
	glEnable(GL_TEXTURE_2D);
	for(unsigned char c=0; c<255; ++c)
	{
		int charX = c % 16;
		int charY = 15 - (c - charX) / 16;
		int x = charX * fontWidth / numCols;
		int y = charY * fontHeight / numRows;

		int imgLeft=width;
		int imgRight=0;

		// Copy the character into the smaller buffer
		for(int row=0; row<height; ++row)
		{
			size_t font_row = row + y;
			size_t font_rowLength = fontWidth * bytesPerPixel;
			size_t x_offset = x * bytesPerPixel;
			size_t font_idx = font_row * font_rowLength + x_offset;

			size_t char_row = row;
			size_t char_rowLength = width * bytesPerPixel;
			size_t char_idx = char_row * char_rowLength;

			memcpy(buffer + char_idx, font + font_idx, char_rowLength);

			const int threshold = 96;

			int left=0;
			while(left < width && buffer[char_idx + left*bytesPerPixel + 3] < threshold) ++left;

			int right=width-1;
			while(right >= 0 && buffer[char_idx + right*bytesPerPixel + 3] < threshold) --right;

			imgLeft = min(imgLeft, left);
			imgRight = max(imgRight, right);
		}

		if(imgLeft >= imgRight)
		{
			characters[c].width = 0.4f;
			characters[c].left = 0.0f;
		}
		else
		{
			characters[c].width = (imgRight - imgLeft) / (float)width;
			characters[c].left = imgLeft / (float)width;
		}

		// Create a new texture object
		characters[c].charTex=0;
		glGenTextures(1, &characters[c].charTex);

		// and bind it as the present texture
		glBindTexture(GL_TEXTURE_2D, characters[c].charTex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Now build mipmaps from the texture data
		gluBuild2DMipmaps(GL_TEXTURE_2D,
				  bytesPerPixel,
				  width,
				  height,
				  alpha ? GL_RGBA : GL_RGB,
				  GL_UNSIGNED_BYTE,
				  buffer);

		CHECK_GL_ERROR();
	} // loop

	// Delete the buffer
	delete[] buffer;
	buffer=0;

	TRACE(string("Created font: ") + fontImageFileName);
}

TextWriter::Character& TextWriter::getCharacter(char z)
{
	ASSERT(z >= 0, "Invalid character");

	return characters[  (size_t)((z >= 0) ? z : '?')  ];
}

const TextWriter::Character& TextWriter::getCharacter(char z) const
{
	ASSERT(z >= 0, "Invalid character");

	return characters[  (size_t)((z >= 0) ? z : '?')  ];
}

TextWriter::Character& TextWriter::getCharacter(wchar_t z)
{
	ASSERT(z < 128, "Invalid character");

	return characters[  (size_t)((z < 128) ? z : '?')  ];
}

const TextWriter::Character& TextWriter::getCharacter(wchar_t z) const
{
	ASSERT(z < 128, "Invalid character");

	return characters[  (size_t)((z < 128) ? z : '?')  ];
}

void TextWriter::drawChar(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, char z, const COLOR &color) const
{
	glBindTexture(GL_TEXTURE_2D, getCharacter(z).charTex);
	glColor4fv(color);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(a.x, a.y, a.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(b.x, b.y, b.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c.x, c.y, c.z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(d.x, d.y, d.z);
	glEnd();
}

void TextWriter::putChar(vec3 *offset, char character, const COLOR *color, FONT_SIZE size, bool useBillboard) const
{
	const float _size = fontSize.find(size)->second;

	float w = 0.0f;
	float h = 0.0f;
	vec3 right;
	vec3 up;
	vec3 nextOffset;

	if(useBillboard)
	{
		w = _size / 100.0f;
		h = _size / 100.0f;

		const mat4 &m = g_Camera.getOrientation();
		right = m.getAxisX().getNormal();
		up = m.getAxisY().getNormal();
	}
	else
	{
		w = _size;
		h = _size;
		right = vec3(1, 0, 0);
		up = vec3(0, 1, 0);
	}

	if(character == '\n')
	{
		if(useBillboard)
		{
			nextOffset = up*(h*lineHeight); // FIXME: this only allows 2 lines of text
		}
		else
		{
			nextOffset.x = 0;
			nextOffset.y = (*offset).y - h*lineHeight;
		}
	}
	else
	{
		/*               center the quad          place it        shift left by an offset  */
		const vec3 a = -right*(w*0.5f)        + (*offset) - right*w*getCharacter(character).left;
		const vec3 b =  right*(w*0.5f)        + (*offset) - right*w*getCharacter(character).left;
		const vec3 c =  right*(w*0.5f) + up*h + (*offset) - right*w*getCharacter(character).left;
		const vec3 d = -right*(w*0.5f) + up*h + (*offset) - right*w*getCharacter(character).left;

		drawChar(a, b, c, d, character, *color);

		float offsetRight = w*getCharacter(character).width + w*0.1f; // TODO: plus some spacing between characters

		nextOffset = (*offset) + right*offsetRight;
	}

	(*offset) = nextOffset;
}

void TextWriter::disableMultiTex(void)
{
	// Disable all extra texture units
	if(g_bUseMultitexture)
	{
		for(int textureUnit=1; textureUnit<g_MultitextureUnits; ++textureUnit)
		{
			glActiveTextureARB(textureStages[textureUnit]);
			glDisable(GL_TEXTURE_2D);
		}
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}
	glEnable(GL_TEXTURE_2D);
}

void TextWriter::write(const string &text, const COLOR &color, FONT_SIZE size, bool useBillboard) const
{
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

		disableMultiTex();

		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);

		// Enable blending, to smooth the characters' edges and show through to the background
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);

		// Write out the string
		vec3 offset;
		for_each(text.begin(), text.end(), bind(&TextWriter::putChar, this, &offset, _1, &color, size, useBillboard));

	glPopAttrib();
	CHECK_GL_ERROR();
}

vec2 TextWriter::getDimensions(const string &text, FONT_SIZE size) const
{
	float row_width=0;
	float max_width=0;
	int numLines=1;

	for(string::const_iterator i = text.begin(); i != text.end(); ++i)
	{
		if((*i) == '\n')
		{
			numLines++;
			max_width = max(max_width, row_width);
			row_width = 0;
		}
		else
		{
			row_width += getCharacter(*i).width;
		}
	}

	max_width = max(max_width, row_width);

	const float _size = fontSize.find(size)->second;

	return vec2(max_width, numLines*lineHeight) * _size;
}

float TextWriter::getLineHeight(FONT_SIZE size) const
{
	const float _size = fontSize.find(size)->second;

	return lineHeight * _size;
}

} // namespace Engine
