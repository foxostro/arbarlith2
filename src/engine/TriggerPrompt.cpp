/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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
#include "TriggerPrompt.h"

namespace Engine {

TriggerPrompt::TriggerPrompt(void)
{
	m_bVisible = true;

	font = &g_Application.fontLarge;

	setRelativePos(vec2(20, 700));
}

TriggerPrompt::HANDLE TriggerPrompt::add(const string &text, float lifeSpan)
{
	for(HANDLE handle=0; handle<MAX_PROMPT_MESSAGES; ++handle)
	{
		Message &message = messages[handle];

		if(!message.isAlive())
		{
			message.create(text, lifeSpan);
			return handle;
		}
	}

	// Failed, too many messages
	return -1;
}

void TriggerPrompt::remove(HANDLE handle)
{
	if(handle==-1)
	{
		return;
	}

	ASSERT(handle>=0 && handle<MAX_PROMPT_MESSAGES, "Invalid handle: " + itoa(handle));
	messages[handle].remove();
}

void TriggerPrompt::renew(HANDLE handle)
{
	if(handle==-1)
	{
		return;
	}

	ASSERT(handle>=0 && handle<MAX_PROMPT_MESSAGES, "Invalid handle: " + itoa(handle));
	messages[handle].renew();
}

void TriggerPrompt::draw(void) const
{
	ASSERT(font!=0, "Font is null");

	if(!isVisible()) return;

	for(HANDLE handle=0; handle<MAX_PROMPT_MESSAGES; ++handle)
	{
		const Message &message = messages[handle];

		if(message.isAlive())
		{
			font->write(message.getMessage(), white, FONT_SIZE_LARGE);

			float lineHeight = font->getLineHeight(FONT_SIZE_LARGE);
			glTranslatef(0.0f, -lineHeight, 0.0f);
		}
	}
}

void TriggerPrompt::update(float deltaTime)
{
	for(HANDLE handle=0; handle<MAX_PROMPT_MESSAGES; ++handle)
	{
		Message &message = messages[handle];
		message.update(deltaTime);
	}
}

}; // namespace
