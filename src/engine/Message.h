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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "ActorFactory.h"

namespace Engine {

enum MSG_TYPE
{
	MSG_NOTHING,   // No message
	MSG_TIMEOUT,   // A waiting period of some sort has ended
	MSG_COLLISION, // One object has collided with another
	MSG_ATTACKED,  // The receiving creature has been attacked
	MSG_SANITY,    // The recieving creature is under pain of sanity loss
	MSG_SIGNAL     // The recieving creature is subscribed to a signal (specified in the message)
};

struct Message_s
{
	Message_s()
	{
		m_Type = MSG_NOTHING;
		m_Sender = m_Recipient = INVALID_ID;
		m_fData = 0.0f;
		m_iData = 0;
		m_TimeDelay = 0.0;
		m_Timestamp = 0.0;
		m_bSent = false;
		m_pData = NULL;
	}

	MSG_TYPE m_Type;
	OBJECT_ID m_Sender, m_Recipient;
	float m_fData;
	int m_iData;
	double m_TimeDelay;

	// Special data handled by specific message handlers and senders
	void *m_pData;

	// Reserved for exclusive us by the message routing system
	double m_Timestamp;
	bool m_bSent;
};

} // namespace Engine

#endif
