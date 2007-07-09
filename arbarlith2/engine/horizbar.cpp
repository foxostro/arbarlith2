/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2004-2007 Game Creation Society
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
#include "OpenGL.h"
#include "HorizBar.h"

namespace Engine {

HorizBar::HorizBar(float x,
                   float y,
                   float capacity,
                   float startingPosition,
                   const _tstring &strForeground,
                   const _tstring &strBackground,
                   bool leftAlign)
: m_ForeImage(0),
  m_BackImage(0),
  m_Position(startingPosition),
  m_Capacity(capacity),
  initialWidth(0),
  isLeftAligned(leftAlign)
{
	// Create child widgets
	AddChild(m_BackImage = new WindowWidget(0, 0, strBackground));
	AddChild(m_ForeImage = new WindowWidget(0, 0, strForeground));

	// Don't stretch the bar image, crop it
	m_ForeImage->stretchToFit = false;

	// Set the dimensions of the widget
	setWidth(initialWidth = m_BackImage->getWidth());
	setHeight(m_BackImage->getHeight());

	// Set the position of the widget
	setRelativePos(vec2(x, y));
}

void HorizBar::update(float)
{
	ASSERT(m_Capacity!=0, _T("invalid member data: Bar capacity was zero"));
	ASSERT(m_ForeImage!=0, _T("invalid member data: m_ForeImage was NULL"));
	ASSERT(m_BackImage!=0, _T("invalid member data: m_BackImage was NULL"));

	const float width = (m_Position / m_Capacity) * initialWidth;

	// Update the width of the bar
	setWidth(width);
	m_ForeImage->setWidth(width);
	m_BackImage->setWidth(width);
}

}; // namespace
