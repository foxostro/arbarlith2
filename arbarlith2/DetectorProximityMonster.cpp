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
#include "stdafx.h"
#include "engine/World.h"
#include "engine/Zone.h"
#include "DetectorProximityMonster.h"

namespace Arbarlith2 {

GEN_ACTOR_RTTI_CPP(DetectorProximityMonster, "class Arbarlith2::DetectorProximityMonster")

bool DetectorProximityMonster::isInProximity(OBJECT_ID actor, float triggerRadius) const
{
	const ActorSet &s = getZone().getObjects();

	// Ignore invalid handles and zombies
	if(zombie || !s.isMember(actor))
		return false;

	// Ignore dead monsters
	const Monster *monster = dynamic_cast<const Monster*>(s.getPtr(actor));
	if(monster!=0 && !monster->isAlive())
		return false;

	return Engine::DetectorProximity<Monster>::isInProximity(actor, triggerRadius);
}

} // namespace Arbarlith2
