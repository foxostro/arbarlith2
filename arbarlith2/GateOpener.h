/*
Author: Andrew Fox
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

#ifndef _GATE_OPENER_H_
#define _GATE_OPENER_H_

#include "engine/Listener.h"

using namespace Engine;

namespace Arbarlith2 { 

/** Opens or closes a specified gate when triggered */
class GateOpener : public Listener
{
public:
	GEN_RTTI(GateOpener)

public:
	/**
	Constructs the opener
	@param ID The desired unique ID of the object
	*/
	GateOpener(OBJECT_ID ID);

	/** Clear out everything to defaults */
	void clear(void);

	/**
	Loads Player from XML
	@param xml XML source
	@return true if the XML loaded successfully
	*/
	virtual bool LoadXml(Engine::CPropBag &xml);

protected:
	/** Called in the event of the Trigger activating */
	virtual void onTrigger(void);

	/**
	Saves the object state to an XML data source, but only if it differs from the default value
	@param xml The XML data source returned
	@param dataFile The data file containing the default values
	@return true if successful, false otherwise
	*/
	virtual bool saveTidy(CPropBag &xml, CPropBag &dataFile) const;

	/**
	Creates the editor toolbar for the gate
	@param pane The actor pane
	*/
	virtual void createToolBar(ListPaneWidget *pane);

private:
	/** Name of the gate to affect */
	_tstring gateName;

	/** Opens the gate when triggered, as opposed to closing it */
	bool open;
};

}; // namespace

#endif
