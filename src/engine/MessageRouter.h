/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

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

#ifndef _MESSAGE_ROUTER_H_
#define _MESSAGE_ROUTER_H_

#include "Message.h"


namespace Engine {



class World;

const size_t NUM_SIGNALS = 22;

class MessageRouter
{
private:
	/** World that the router operates within */
	World *zone;

	vector<Message_s> m_Messages;

	/** Objects that are subscribed to receive messages from a specific signal */
	vector<OBJECT_ID> signalReceivers[NUM_SIGNALS];

public:
	/** Debug data! Records the tim since a signal has been raised */
	float signals[NUM_SIGNALS];

public:
	/**
	Constructor
	*/
	MessageRouter(void);

	/**
	Sets the World for the router
	@param theZone World that the router operates within
	*/
	void setZone(World *theZone);

	bool Send(Message_s &Msg);

	/**
	@param deltaTime Milliseconds since the last update
	*/
	void update(float deltaTime);

	/**
	Raises a signal
	@param signalIndex Index of the signal
	*/
	void raiseSignal(int signalIndex);

	/**
	Subscribes to a signal
	@param id The object that is subscribing
	@param signalIndex Index of the signal
	*/
	void subscribeToSignal(OBJECT_ID id, int signalIndex);

	/**
	Unsubscribe to all signals
	@param id The receier to cancel
	*/
	void unsubscribeToAllSignals(OBJECT_ID id);

private:
	void MailIt(Message_s Msg);
};

} // namespace Engine

#endif

