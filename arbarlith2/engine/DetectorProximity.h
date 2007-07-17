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

#ifndef _DETECTOR_PROXIMITY_H_
#define _DETECTOR_PROXIMITY_H_

#include "Detector.h"

namespace Engine {

/** Sends a signal when an object of a given type is very close to the detector */
template<class T>
class DetectorProximity : public Detector
{
public:
	/**
	Constructs the Detector
	@param handle The desired unique ID of the object
	*/
	DetectorProximity(OBJECT_ID handle)
	:Detector(handle)
	{}

protected:
	/**
	Determine whether the proper conditions have been attained for trigger activation.
	By default the trigger condition is the mere proximity of the PLAYER
	@return true if so, false otherwise
	*/
	virtual bool pollConditions(void) const
	{
		OBJECT_ID unused;

		const ActorSet &objects = getZone().getObjects();

		ActorSet s = objects.typeFilter<T>();

		return isAnythingInProximity(s, unused, triggerRadius);
	}

public:
	GEN_RTTI(DetectorProximity, "class Engine::DetectorProximity")
};

} // namespace Engine

#endif
