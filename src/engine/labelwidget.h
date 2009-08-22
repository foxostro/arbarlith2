/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2003-2007 Game Creation Society
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

#ifndef _LABELWIDGET_H_
#define _LABELWIDGET_H_

#include "tstring.h"
#include "COLOR.h"
#include "widget.h"
#include "text.h"

namespace Engine {

class LabelWidget : public Widget
{
public:
	LabelWidget(const string &label, const vec2 &pos);

	LabelWidget
		(
			const string &label,
			const vec2 &pos,
			const COLOR &color,
			FONT_SIZE FontSize,
			TextWriter &font
		);

	/**
	Gets the label
	@return label
	*/
	inline const string& getLabel(void) const
	{
		return labelText;
	}

	/**
	Sets the label text
	@param label The new label text
	@todo Consider newline characters when calculating the bounds of the text
	*/
	void setLabel(const string& label);

	/**
	Sets the label font
	@param label The new label text
	*/
	void setFont(TextWriter &font);

	/**
	Sets the label color
	@param color The new label color
	*/
	void setColor(const COLOR &color);

	/**
	Sets the label font size
	@param color The new label font size
	*/
	void setFontSize(FONT_SIZE fontSize);

public:
	/** Label color */
	COLOR m_Color;

protected:
	/** draws the widget*/
	virtual void draw(void) const;

private:
	/** Label text */
	string labelText;

	/** Font size */
	FONT_SIZE fontSize;

	/** The current font */
	TextWriter *font;
};

} // namespace Engine

#endif
