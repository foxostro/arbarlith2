/*
Original Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

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

#include "stdafx.h"
#include "engine/ListElementTweaker.h"
#include "engine/ListPaneWidget.h"
#include "engine/ToggleWidgetText.h"
#include "Teleporter.h"

namespace Engine {

GEN_ACTOR_RTTI_CPP(Teleporter, "class Engine::Teleporter")

Teleporter::Teleporter(OBJECT_ID ID)
: Switch(ID)
{}

void Teleporter::clear(void)
{
	Switch::clear();
	actionLabel = _T("enter.");
	destinationPosition.zero();
}

void Teleporter::onUse(Actor*)
{
	const size_t numOfPlayers = getZone().getNumOfPlayers();

	if(numOfPlayers < 2)
	{
		// Place the single player at the destination point
		getZone().getPlayer(0).Place(destinationPosition);
	}
	else
	{
		// Place players around the destination point
		for(size_t i=0; i<numOfPlayers; ++i)
		{
			Player &player = getZone().getPlayer(i);

			float angle = 2.0f * ((float)i/numOfPlayers) * (float)M_PI;
			vec3 offset = vec3(cosf(angle), 0, sinf(angle)) * 2.0f;

			player.Place(destinationPosition + offset);
		}
	}
}

void Teleporter::createToolBar(ListPaneWidget *pane)
{
	// hide options from base classes other than the Actor

	pane->addElement(new ToggleWidgetText(		_T("Lit"),				&isLit));
	pane->addElement(new ListElementTweaker<float>(	_T("Height (m)"),			&m_desiredHeight));
	pane->addElement(new ListElementTweakerString(	_T("Model"),			&m_strModelFilename));
	pane->addElement(new ListElementTweakerXML(	_T("Position"),			&position));
	pane->addElement(new ListElementTweakerXML(	_T("Destination Position"),	&destinationPosition));
}

void Teleporter::load(const PropertyBag &xml)
{
	Switch::load(xml);

	xml.getSym(destinationPosition);
}

bool Teleporter::saveTidy(PropertyBag &xml, PropertyBag &editorData) const
{
	saveTag(xml, editorData, _T("destinationPosition"),	destinationPosition);

	return Switch::saveTidy(xml, editorData);
}

}; // namespace
