/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2006-2007,2009 Game Creation Society
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

#ifndef _TRIGGER_PROMPT_H_
#define _TRIGGER_PROMPT_H_

#include "tstring.h"
#include "widget.h"

namespace Engine {

class TextWriter;

/** Maximum number of messages that can be stored */
const int MAX_PROMPT_MESSAGES = 128;

/** Prompt that is displayed to alert the player to a context-sensitive action */
class TriggerPrompt : public Widget
{
public:
	/** Handle to a particular message */
	typedef int HANDLE;

	/** Constructor */
	TriggerPrompt(void);

	/**
	Adds a message to the prompt display.
	May return a handle of -1 when there are too many messages.
	@param message The message to display
	@param lifeSpan Milliseconds before the message expires
	@return Handle to the message that was added
	*/
	HANDLE add(const string &message, float lifeSpan);

	/**
	Immediately removes a specific message
	@param handle Handle to a specific message
	*/
	void remove(HANDLE handle);

	/**
	Renews the life of a message that already exists
	@param handle Handle to a specific message
	*/
	void renew(HANDLE handle);

protected:
	/** Draws the widget */
	void draw(void) const;

	/**
	Updates the widget
	@param deltaTime Milliseconds since the last tick
	*/
	void update(float deltaTime);

private:
	/** Instance of a single message on the prompt */
	class Message
	{
	public:
		/** Constructor */
		Message(void)
		:lifeSpan(0.0f),
		age(0.0f)
		{}

		/**
		Creates the message
		@param msg The message to display
		@param lifespan Milliseconds before the message expires
		*/
		inline void create(const string &msg, float lifespan)
		{
			message = msg;
			lifeSpan = lifespan;
			age = 0.0f;
		}

		/**
		Renews the message
		@param msg The message to display
		@param lifespan Milliseconds before the message expires
		*/
		inline void renew(void)
		{
			age = 0.0f;
		}

		/** Removes the message */
		inline void remove(void)
		{
			age = lifeSpan;
		}

		/**
		Periodically update the message
		@param deltaTime Milliseconds since the last update
		*/
		inline void update(float deltaTime)
		{
			if(age > lifeSpan)
			{
				age = lifeSpan; // dead message
			}
			else
			{
				age += deltaTime;
			}
		}

		/** Indicates that the message is still alive and relevant */
		inline bool isAlive(void) const
		{
			return age < lifeSpan;
		}

		/** Gets the message string */
		inline const string& getMessage(void) const
		{
			return message;
		}

	private:
		/** The message string */
		string message;

		/** Milliseconds the message will live before expiring */
		float lifeSpan;

		/** Current age of the message in milliseconds */
		float age;
	};

	/** Maps message handles to the messages themselves */
	Message messages[MAX_PROMPT_MESSAGES];

	/** the font that will be used to display messages */
	TextWriter *font;
};

}; // namespace

#endif
