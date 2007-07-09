/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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
#include "ListPaneWidget.h"
#include "ListElementTweaker.h"
#include "ToggleWidgetText.h"
#include "ActorSet.h"
#include "World.h"
#include "Zone.h"
#include "Detector.h"

namespace Engine {

GEN_ACTOR_RTTI_CPP(Detector, "class Engine::Detector")

Detector::Detector(OBJECT_ID ID)
:Trigger(ID)
{}

void Detector::clear(void)
{
	Trigger::clear();
	signalSuccess=-1;
	signalFail=-1;
	triggeredOnce=false;
	onlyTripOnce=false;
	disallowBounce=true;
	debounce=false;
}

bool Detector::pollConditions(void) const
{
	return true;
}

void Detector::onDoesntTrigger(void)
{
	getZone().router.raiseSignal(signalFail);
}

void Detector::onTrigger(void)
{
	if(onlyTripOnce && triggeredOnce) return;

	getZone().router.raiseSignal(signalSuccess);
	triggeredOnce = true;
}

void Detector::update(float deltaTime)
{
	Actor::update(deltaTime);

	if(pollConditions())
	{
		if(!debounce || disallowBounce==false)
		{
			debounce = true;
			onTrigger();
		}
	}
	else
	{
		debounce = false;
		onDoesntTrigger();
	}
}

bool Detector::LoadXml(CPropBag &xml)
{
	Trigger::LoadXml(xml);

	xml.getSym(signalSuccess);
	xml.getSym(signalFail);
	xml.getSym(onlyTripOnce);

	return true;
}

bool Detector::saveTidy(CPropBag &xml, CPropBag &editorData) const
{
	saveTag(xml, editorData, _T("signalSuccess"), signalSuccess);
	saveTag(xml, editorData, _T("signalFail"), signalFail);
	saveTag(xml, editorData, _T("onlyTripOnce"), onlyTripOnce);

	return Trigger::saveTidy(xml, editorData);
}

void Detector::createToolBar(ListPaneWidget *pane)
{
	Trigger::createToolBar(pane);

	pane->addElement(new ToggleWidgetText(		_T("Only trip once"),		&onlyTripOnce));
	pane->addElement(new ListElementTweaker<int>(	_T("Signal on Success"),	&signalSuccess));
	pane->addElement(new ListElementTweaker<int>(	_T("Signal on Failure"),	&signalFail));
}

}; // namespace
