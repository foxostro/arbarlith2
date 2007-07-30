/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#include "stdafx.h"
#include "gl.h"
#include "Application.h"
#include "labelwidget.h"

namespace Engine {

LabelWidget::LabelWidget
			(
				const _tstring &label,
				const vec2 &pos
			)
: fontSize(FONT_SIZE_NORMAL),
  font(0)
{
	setFont(g_Application.fontLarge);
	setFontSize(FONT_SIZE_NORMAL);
	setLabel(label);
	setColor(white);
	setRelativePos(pos);
}

LabelWidget::LabelWidget
			(
				const _tstring &label,
				const vec2 &pos,
				const COLOR &color,
				FONT_SIZE FontSize,
				TextWriter &font
			)
: fontSize(FONT_SIZE_NORMAL),
  font(0)
{
	setFont(font);
	setFontSize(FontSize);
	setLabel(label);
	setColor(color);
	setRelativePos(pos);
}

void LabelWidget::setLabel(const _tstring& labelTxt)
{
	ASSERT(font != 0, _T("font was null!  Call setFont method first!"));

	labelText = labelTxt;

	vec2 dim = font->getDimensions(labelText, fontSize);
	setWidth(dim.x);
	setHeight(dim.y);
}

void LabelWidget::setColor(const COLOR &color)
{
	this->m_Color = color;
}

void LabelWidget::setFont(TextWriter &font)
{
	this->font = &font;
}

void LabelWidget::setFontSize(FONT_SIZE fontSize)
{
	this->fontSize = fontSize;
}

void LabelWidget::draw(void) const
{
	ASSERT(font != 0, _T("font is null!  call setFont method first!"));

	if(isVisible())
	{
		font->write(labelText, m_Color, fontSize);
	}
}

} // namespace Engine
