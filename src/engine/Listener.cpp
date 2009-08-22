/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2007 Game Creation Society
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
#include "ActorSet.h"
#include "world.h"
#include "WaitScreen.h"
#include "ListPaneWidget.h"
#include "ListElementTweaker.h"
#include "Listener.h"

namespace Engine {

GEN_ACTOR_RTTI_CPP(Listener, "class Engine::Listener")

Listener::Listener(OBJECT_ID ID)
:Trigger(ID)
{}

void Listener::clear(void)
{
	Trigger::clear();
	signals.clear();
	anySignal=false;
	expression = "any: 1 ; 2 ; 3 ; 4 ; 5";
}

bool Listener::pollConditions(void) const
{
	if(anySignal == true)
	{
		for(map<int, float>::const_iterator iter = signals.begin(); iter != signals.end(); ++iter)
		{
			float time = iter->second;
			if(time < 1000.0f)
				return true;
		}

		return false;
	}
	else
	{
		for(map<int, float>::const_iterator iter = signals.begin(); iter != signals.end(); ++iter)
		{
			float time = iter->second;
			if(time > 1000.0f)
				return false;
		}

		return true;
	}
}

void Listener::setupSignals(const string &expression)
{
	// reset
	anySignal = true;
	signals.clear();
	getZone().router.unsubscribeToAllSignals(m_ID);

    char_separator<char> delimeters(":;, \t\n");
    tokenizer<char_separator<char> > tokens(expression, delimeters);

	for(tokenizer<char_separator<char> >::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
	{
		int signalIndex=0;

		if(toLowerCase(*iter) == "any")
		{
			anySignal = true;
		}
		else if(toLowerCase(*iter) == "all")
		{
			anySignal = false;
		}
		else
		{
			stringstream ss;
			ss << (*iter);
			ss >> signalIndex;
			signals.insert(make_pair(signalIndex, 5000.0f));
			getZone().router.subscribeToSignal(m_ID, signalIndex);
		}
	}
}

void Listener::load(const PropertyBag &xml)
{
	Trigger::load(xml);

	xml.getSym(expression);
	setupSignals(expression);
}

bool Listener::saveTidy(PropertyBag &xml, PropertyBag &editorData) const
{
	saveTag(xml, editorData, "expression", expression);
	return Trigger::saveTidy(xml, editorData);
}

void Listener::OnMessage(Message_s message)
{
	if(message.m_Type == MSG_SIGNAL)
	{
		for(map<int, float>::iterator iter = signals.begin(); iter != signals.end(); ++iter)
		{
			if(iter->first == message.m_iData)
			{
				iter->second = 0;
			}
		}
	}
}

void Listener::update(float milliseconds)
{
	Trigger::update(milliseconds);

	for(map<int, float>::iterator iter = signals.begin(); iter != signals.end(); ++iter)
	{
		(iter->second) += milliseconds;
	}
}

void Listener::createToolBar(ListPaneWidget *pane)
{
	// hide options from base classes other than the Actor

	pane->addElement(new ListElementTweakerString("Expression", &expression));
}

void Listener::sync(void)
{
	Trigger::sync();

	setupSignals(expression);
}

}; // namespace
