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
#include "Object.h"
#include "World.h"
#include "MessageRouter.h"


namespace Engine { 



MessageRouter::MessageRouter(void)
: zone(0)
{
	for(size_t i=0; i<NUM_SIGNALS; ++i)
	{
		signals[i] = 0.0f;
	}
}

void MessageRouter::setZone(World *theZone)
{
	zone = theZone;
}

bool MessageRouter::Send(Message_s &Msg)
{
	ASSERT(zone != 0, _T("zone was null"));

	// Set the time stamp member
	Msg.m_bSent = false;
	Msg.m_Timestamp = zone->getClockTicks();

	// Should the message be sent immediately or be delayed
	if(Msg.m_TimeDelay == 0.0)
	{
		// Deliver immediately
		MailIt(Msg);

		// Return whether the message will actually be sent
		return true;
	}
	else
	{
		// Check for duplicate messages
		for(vector<Message_s>::iterator iter=m_Messages.begin(); iter!=m_Messages.end(); ++iter)
		{
			if((*iter).m_bSent     == false           &&
			   (*iter).m_fData     == Msg.m_fData     &&
			   (*iter).m_iData     == Msg.m_iData     &&
			   (*iter).m_Recipient == Msg.m_Recipient &&
			   (*iter).m_Sender    == Msg.m_Sender    &&
			   (*iter).m_Type      == Msg.m_Type)
			{
				// Ignore the message, another copy already exists
				return false;
			}
		}

		// Add it to the message queue
		m_Messages.push_back(Msg);

		// Return whether the message will actually be sent
		return true;
	}
}

void MessageRouter::MailIt(Message_s Msg)
{
	ASSERT(zone != 0, _T("zone was null"));
	ASSERT(zone->getObjects().isMember(Msg.m_Recipient), _T("Recipient does not exist"));

	if(zone->getObjects().isMember(Msg.m_Recipient))
	{
		Actor &object = zone->getObjects().get(Msg.m_Recipient);
		object.OnMessage(Msg);
	}
}

void MessageRouter::update(float deltaTime)
{
	ASSERT(zone != 0, _T("zone was null"));

	double Time = zone->getClockTicks();

	vector< vector<Message_s>::iterator > ToDelete;

	// Process all delayed messages
	for(vector<Message_s>::iterator iter=m_Messages.begin(); iter!=m_Messages.end(); ++iter)
	{
		if(  (*iter).m_Timestamp + (*iter).m_TimeDelay - Time < 1.0  )
		{
			MailIt(*iter);
			(*iter).m_bSent = true;
		}

		if(  (*iter).m_bSent == true  )
		{
			ToDelete.push_back(iter);
		}
	}

	// Delete sent messages
	for(size_t i=0; i<ToDelete.size(); ++i)
	{
		m_Messages.erase(ToDelete[i]);
	}

	for(size_t i=0; i<NUM_SIGNALS; ++i)
	{
		signals[i] += deltaTime;
	}
}

void MessageRouter::raiseSignal(int signalIndex)
{
	if(signalIndex<0) return;

	vector<OBJECT_ID> receivers = signalReceivers[signalIndex];

	for(vector<OBJECT_ID>::const_iterator iter = receivers.begin(); iter != receivers.end(); ++iter)
	{
		Message_s message;

		message.m_Recipient = *iter;
		message.m_Type = MSG_SIGNAL;
		message.m_iData = signalIndex;

		this->Send(message);
	}

	signals[signalIndex] = 0;
}

void MessageRouter::subscribeToSignal(OBJECT_ID id, int signalIndex)
{
	// check for duplicates
	for(vector<OBJECT_ID>::const_iterator iter = signalReceivers[signalIndex].begin();
		iter != signalReceivers[signalIndex].end();
		++iter)
	{
		if(*iter == id)
		{
			return;
		}
	}

	signalReceivers[signalIndex].push_back(id);
}

void MessageRouter::unsubscribeToAllSignals(OBJECT_ID id)
{
	for(size_t i=0; i<NUM_SIGNALS; ++i)
	{
		vector<OBJECT_ID> receivers = signalReceivers[i];

		vector<OBJECT_ID>::iterator iter = receivers.begin();

		while(iter != receivers.end())
		{
			if(*iter == id)
				iter = receivers.erase(iter);
			else
				++iter;
		}
	}
}

} // namespace Engine
