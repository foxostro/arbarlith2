/*
Modified by Andrew Fox in 2004-2006
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

#ifndef __STATEMCH_H__
#define __STATEMCH_H__

#include "StateMachineFactory.h"
#include "world.h"
#include "creature.h"

namespace Engine {


//State Machine Language Macros (put these keywords in the file USERTYPE.DAT in the same directory as MSDEV.EXE)
#if 0

#define BeginStateMachine		if( state < 0 ) { char statename[64] = "STATE_Global"; if(0) {
#define EndStateMachine			return( true ); } } else { /*assert( 0 && "Invalid State" );*/ return( false ); } return( false );
#define State(a)				return( true ); } } else if( a == state ) { char statename[64] = #a; if(0) {
#define OnMsg(a)				return( true ); } else if( EVENT_Message == event && msg && a == msg->m_Type ) { /*g_debuglog.LogStateMachineEvent( m_Owner->GetID(), msg, statename, #a, true );*/
#define OnEvent(a)				return( true ); } else if( a == event ) { /*g_debuglog.LogStateMachineEvent( m_Owner->GetID(), msg, statename, #a, true );*/
#define OnUpdate				OnEvent( EVENT_Update )
#define OnEnter					OnEvent( EVENT_Enter )
#define OnExit					OnEvent( EVENT_Exit )

#else

#define BeginStateMachine		if( state < 0 ) { if(0) {
#define EndStateMachine			return( true ); } } else { return( false ); } return( false );
#define State(a)			return( true ); } } else if( a == state ) { if(0) {
#define OnMsg(a)			return( true ); } else if( EVENT_Message == event && msg && a == msg->m_Type ) {
#define OnEvent(a)			return( true ); } else if( a == event ) {
#define OnUpdate			OnEvent( EVENT_Update )
#define OnEnter				OnEvent( EVENT_Enter )
#define OnExit				OnEvent( EVENT_Exit )

#endif

enum StateMachineEvent { EVENT_INVALID,
						 EVENT_Update,
						 EVENT_Message,
						 EVENT_Enter,
						 EVENT_Exit
};

enum MSG_Scope
{
	NO_MSG_SCOPING,
	SCOPE_TO_THIS_STATE
};


class StateMachine
{
public:
	GEN_RTTI(StateMachine, "class Engine::StateMachine")

public:
	/** Destructor */
	virtual ~StateMachine(void)
	{}

	StateMachine(OBJECT_ID handle);

	void Initialize( void );
	void Update( Message_s * msg );
	void SetState( unsigned int newState );

	void SendMsg( MSG_TYPE name, OBJECT_ID receiver );
	void SendDelayedMsg( float delay, MSG_TYPE name, OBJECT_ID receiver );
	void SendDelayedMsgToMe( float delay, MSG_TYPE name, MSG_Scope scope );

	void setOwner(Creature *owner)
	{
		ASSERT(owner!=0, "Owner parameter was NULL");
		m_Owner = owner;
	}

	/**
	Loads the object state
	@param data data source
	*/
	virtual void load(const PropertyBag &data);

	int GetState( void )						{ return( m_currentState ); }
	double GetTimeInState( void );

	void SetCCReceiver( OBJECT_ID id )			{ m_ccMessagesToGameObject = id; }
	void ClearCCReceiver( void )				{ m_ccMessagesToGameObject = 0; }
	OBJECT_ID GetCCReceiver( void )				{ return( m_ccMessagesToGameObject ); }

	void Process( StateMachineEvent event, Message_s * msg );


protected:

	Creature * m_Owner;


private:

	unsigned int m_currentState;
	unsigned int m_nextState;
	bool m_stateChange;
	double m_timeOnEnter;
	OBJECT_ID m_ccMessagesToGameObject;

	virtual bool States( StateMachineEvent event, Message_s * msg, int state ) = 0;
};

} // namespace Engine

#endif	// __STATEMCH_H__
