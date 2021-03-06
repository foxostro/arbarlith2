/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009,2010 Game Creation Society
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
#include "engine/ActorSet.h"
#include "engine/world.h"
#include "engine/random.h"
#include "engine/WaitScreen.h"
#include "engine/ListPaneWidget.h"
#include "engine/ListElementTweaker.h"
#include "engine/ToggleWidgetText.h"
#include "GateOpener.h"
#include "TileGate.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(GateOpener, "class Arbarlith2::GateOpener")

GateOpener::GateOpener(OBJECT_ID ID)
:Listener(ID)
{}

void GateOpener::clear(void)
{
	Listener::clear();
	open=true;
}

void GateOpener::onTrigger(void)
{
	ActorSet &s = getZone().getObjects();

	vector<Actor*> gates = s.getByName(gateName);

	for(vector<Actor*>::iterator iter = gates.begin(); iter != gates.end(); ++iter)
	{
		TileGate *gate = dynamic_cast<TileGate*>(*iter);

		if(gate)
		{
			if(open)
				gate->unlockGate();
			else
				gate->lockGate();
		}
	}
}

void GateOpener::load(const PropertyBag &xml)
{
	Listener::load(xml);

	xml.get("gateName", gateName);
	xml.get("open", open);
}

bool GateOpener::saveTidy(PropertyBag &xml, PropertyBag &editorData) const
{
	saveTag(xml, editorData, "gateName", gateName);
	saveTag(xml, editorData, "open", open);
	return Listener::saveTidy(xml, editorData);
}

void GateOpener::createToolBar(ListPaneWidget *pane)
{
	Listener::createToolBar(pane);

	pane->addElement(new ListElementTweakerString	("Gate Name",	&gateName));
	pane->addElement(new ToggleWidgetText		("Opens Gate",	&open));
}

} // namespace Arbarlith2
