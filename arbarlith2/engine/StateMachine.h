/*
Modified by Andrew Fox in 2004-2006
E-Mail: mailto:andrewfox@cmu.edu
*/

/* Copyright � Steve Rabin, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright � Steve Rabin, 2001"
 */

#ifndef __STATEMCH_H__
#define __STATEMCH_H__

#include "StateMachineFactory.h"
#include "world.h"
#include "creature.h"



namespace Engine { 

template<class T> struct StateMachineRegistrar
{
	StateMachineRegistrar(void)
	{
		::Engine::getStateMachineFactory().registerType<T>();
	}
};

#define GEN_FSM_RTTI_CPP(TYPE) namespace { ::Engine::StateMachineRegistrar< TYPE > _registrar; }


//State Machine Language Macros (put these keywords in the file USERTYPE.DAT in the same directory as MSDEV.EXE)
#define BeginStateMachine		if( state < 0 ) { char statename[64] = "STATE_Global"; if(0) {
#define EndStateMachine			return( true ); } } else { /*assert( 0 && "Invalid State" );*/ return( false ); } return( false );
#define State(a)				return( true ); } } else if( a == state ) { char statename[64] = #a; if(0) { 
#define OnMsg(a)				return( true ); } else if( EVENT_Message == event && msg && a == msg->m_Type ) { /*g_debuglog.LogStateMachineEvent( m_Owner->GetID(), msg, statename, #a, true );*/
#define OnEvent(a)				return( true ); } else if( a == event ) { /*g_debuglog.LogStateMachineEvent( m_Owner->GetID(), msg, statename, #a, true );*/
#define OnUpdate				OnEvent( EVENT_Update )
#define OnEnter					OnEvent( EVENT_Enter )
#define OnExit					OnEvent( EVENT_Exit )



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
	GEN_RTTI(StateMachine)

public:
	StateMachine(OBJECT_ID handle);
	~StateMachine( void ) {}

	void Initialize( void );
	void Update( Message_s * msg );
	void SetState( unsigned int newState );

	void SendMsg( MSG_TYPE name, OBJECT_ID receiver );
	void SendDelayedMsg( float delay, MSG_TYPE name, OBJECT_ID receiver );
	void SendDelayedMsgToMe( float delay, MSG_TYPE name, MSG_Scope scope );

	void setOwner(Creature *owner)
	{
		ASSERT(owner!=0, _T("Owner parameter was NULL"));
		m_Owner = owner;
	}

	/**
	Save to XML
	@param bag XML source
	@return true if the calling child class should save fully, false
	if the calling child class should save just enough for editor mode
	*/
	virtual bool SaveXml(CPropBag &bag);
	
	/**
	Load from XML
	@param bag XML source
	@return true if successful
	*/
	virtual bool LoadXml(CPropBag &bag);

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

}; // namespace


#endif	// __STATEMCH_H__