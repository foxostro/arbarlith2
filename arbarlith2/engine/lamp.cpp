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
#include "ListPaneWidget.h"
#include "ListElementTweaker.h"
#include "ToggleWidgetText.h"
#include "Light.h"
#include "World.h"
#include "Player.h"
#include "Lamp.h"

namespace Engine {

GEN_ACTOR_RTTI_CPP(Lamp, "class Engine::Lamp")

Lamp::Lamp(OBJECT_ID ID)
:Switch(ID)
{
	clear();
}


void Lamp::onUse(Actor *a)
{
	active = !active;

	Light &L = getZone().getLightManager().getLight(light);

	L.enable = active;
	L.constantAttenuation = constant;
	L.linearAttenuation = linear;
	L.quadraticAttenuation = quadratic;

	Switch::onUse(a);
}

void Lamp::destroy(void)
{
	getZone().getLightManager().removeLight(light);

	Switch::destroy();
}

void Lamp::clear(void)
{
	light = INVALID_LIGHT;
	active = true;
	constant = 0.0f;
	linear = 0.0f;
	quadratic = 0.4f;

	Switch::clear();
}

void Lamp::update(float deltaTime)
{
	Switch::update(deltaTime);

	Light &L = getZone().getLightManager().getLight(light);

	L.setPosition(getPos() + vec3(0,getSphereRadius(),0));
	L.constantAttenuation = constant;
	L.linearAttenuation = linear;
	L.quadraticAttenuation = quadratic;
	L.enable = active;

	L.lookAt(
				getPos() + vec3(0,getSphereRadius(),0),
				getPos() + vec3(0,getSphereRadius(),0) - getOrientation().getAxisZ()
			);
}

bool Lamp::LoadXml(PropertyBag &xml)
{
	Switch::LoadXml(xml);

	xml.getSym(active);
	xml.getSym(constant);
	xml.getSym(linear);
	xml.getSym(quadratic);

	// Create a light object
	Light *newLight = new Light;
	newLight->create();
	newLight->constantAttenuation = constant;
	newLight->linearAttenuation = linear;
	newLight->quadraticAttenuation = quadratic;

	// Add the light to the World
	light = getZone().getLightManager().addLight(newLight);

	return true;
}

bool Lamp::saveTidy(PropertyBag &xml, PropertyBag &dataFile) const
{
	saveTag(xml, dataFile, _T("active"), active);
	saveTag(xml, dataFile, _T("constant"), constant);
	saveTag(xml, dataFile, _T("linear"), linear);
	saveTag(xml, dataFile, _T("quadratic"), quadratic);

	return Switch::saveTidy(xml, dataFile);
}

void Lamp::sync(void)
{
	Switch::sync();

	Light &L = getZone().getLightManager().getLight(light);

	L.constantAttenuation = constant;
	L.linearAttenuation = linear;
	L.quadraticAttenuation = quadratic;
	L.enable = active;

	L.lookAt(
				getPos() + vec3(0,getSphereRadius(),0),
				getPos() + vec3(0,getSphereRadius(),0) - getOrientation().getAxisZ()
			);
}

void Lamp::createToolBar(ListPaneWidget *pane)
{
	Switch::createToolBar(pane);

	// Add to the tool bar
	pane->addElement(new ListElementTweaker<float>	(_T("constant"),	&constant));
	pane->addElement(new ListElementTweaker<float>	(_T("linear"),		&linear));
	pane->addElement(new ListElementTweaker<float>	(_T("quadratic"),	&quadratic));
	pane->addElement(new ToggleWidgetText			(_T("active"),		&active));
}

}; // namespace
