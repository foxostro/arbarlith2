/*
Modified in 2005-2007 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
*/

/* Copyright © Steve Rabin, 2001.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright © Steve Rabin, 2001"
 */

#include "stdafx.h"
#include "World.h"
#include "StateMachine.h"


namespace Engine {



StateMachine::StateMachine(OBJECT_ID) // RTTI expects to pass us our ID, but we don't really care about it
{
	m_Owner = 0;
	m_currentState = 0;
	m_stateChange = false;
	m_nextState = false;
	m_timeOnEnter = 0.0f;
	m_ccMessagesToGameObject = INVALID_ID;
}


void StateMachine::Initialize( void )
{
	Process( EVENT_Enter, 0 );
}


void StateMachine::Update( Message_s * msg )
{
	Process( EVENT_Update, msg );
}


void StateMachine::Process( StateMachineEvent event, Message_s * msg )
{
	ASSERT(m_Owner!=0, _T("m_Owner was null"));

	if( event == EVENT_Message && msg && GetCCReceiver() > INVALID_ID )
	{	// CC this message
		SendMsg( msg->m_Type, GetCCReceiver() );
	}

	if( States( event, msg, m_currentState ) == false )
	{	// Current state didn't handle msg, try Global state (-1)
		States( event, msg, -1 );
	}

	// Check for a state change
	int safetyCount = 50;
	while( m_stateChange && (--safetyCount >= 0) )
	{
		ASSERT(safetyCount > 0, _T("StateMachine::Process - States are flip-flopping in an infinite loop."));

		m_stateChange = false;

		// Let the last state clean-up
		States( EVENT_Exit, 0, m_currentState );

		// Set the new state
		m_currentState = m_nextState;

		// Remember the time we entered this state
		m_timeOnEnter = m_Owner->getZone().getClockTicks();

		// Let the new state initialize
		States( EVENT_Enter, 0, m_currentState );
	}

}


void StateMachine::SetState( unsigned int newState )
{
	m_stateChange = true;
	m_nextState = newState;
}


void StateMachine::SendMsg( MSG_TYPE name, OBJECT_ID receiver )
{
	ASSERT(m_Owner!=0, _T("m_Owner was null"));

	Message_s Msg;

	Msg.m_Recipient = receiver;
	Msg.m_Sender = m_Owner->m_ID;
	Msg.m_TimeDelay = 0.0;
	Msg.m_Type = name;

	// The message is sent via the message router in the realm where our owner is
	m_Owner->getZone().router.Send(Msg);
}


void StateMachine::SendDelayedMsg( float delay, MSG_TYPE name, OBJECT_ID receiver )
{
	ASSERT(m_Owner!=0, _T("m_Owner was null"));

	Message_s Msg;

	Msg.m_Recipient = receiver;
	Msg.m_Sender = m_Owner->m_ID;
	Msg.m_TimeDelay = double(delay);
	Msg.m_Type = name;

	// The message is sent via the message router in the realm where our owner is
	m_Owner->getZone().router.Send(Msg);
}


void StateMachine::SendDelayedMsgToMe( float delay, MSG_TYPE name, MSG_Scope )
{
	ASSERT(m_Owner!=0, _T("m_Owner was null"));

	Message_s Msg;

	Msg.m_Recipient = m_Owner->m_ID;
	Msg.m_Sender = m_Owner->m_ID;
	Msg.m_TimeDelay = double(delay);
	Msg.m_Type = name;

	// The message is sent via the message router in the realm where our owner is
	m_Owner->getZone().router.Send(Msg);
}

double StateMachine::GetTimeInState( void )
{
	ASSERT(m_Owner!=0, _T("m_Owner was null"));
	return( m_Owner->getZone().getClockTicks() - m_timeOnEnter );
}

void StateMachine::load(const PropertyBag &Bag)
{
	Bag.get(_T("currentState"), m_currentState);
	Bag.get(_T("nextState"),    m_nextState);
	Bag.get(_T("stateChange"),  m_stateChange);
	Bag.get(_T("timeOnEnter"),  m_timeOnEnter);

	m_Owner = 0;
	m_ccMessagesToGameObject = INVALID_ID;
}


} // namespace Engine
