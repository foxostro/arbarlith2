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
#include "ListPaneWidget.h"
#include "ListElementTweaker.h"
#include "World.h"
#include "TriggerParticles.h"


namespace Engine {



GEN_ACTOR_RTTI_CPP(TriggerParticles, "class Engine::TriggerParticles")




TriggerParticles::TriggerParticles(OBJECT_ID ID)
: Trigger(ID)
{}

void TriggerParticles::onTrigger(void)
{
	getZone().SpawnPfx(pfxFileName, pfxLocation);
	Trigger::onTrigger();
}

bool TriggerParticles::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, _T("pfxFileName"), pfxFileName);
	saveTag(xml, dataFile, _T("pfxLocation"), pfxLocation);

	return Trigger::saveTidy(xml, dataFile);
}

bool TriggerParticles::LoadXml(PropertyBag &xml)
{
	Trigger::LoadXml(xml);

	xml.getSym(pfxFileName);
	xml.getSym(pfxLocation);

	showModel = false;

	return true;
}

void TriggerParticles::createToolBar(ListPaneWidget *pane)
{
	// Create the initial tool bar
	Trigger::createToolBar(pane);

	// Add to the tool bar
	pane->addElement(new ListElementTweakerString(	_T("Particle FX"),	&pfxFileName));
	pane->addElement(new ListElementTweakerXML(	_T("PFX Location"),	&pfxLocation));
}

}; // namespace
