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

#ifndef _GAME_STATE_RUN_H_
#define _GAME_STATE_RUN_H_

#include "singleton.h"
#include "BlurEffect.h"
#include "Dimmer.h"
#include "TriggerPrompt.h"
#include "GameState.h"
#include "PerformanceLabel.h"
#include "DebugLabel.h"

namespace Engine {

/** Normal running state of the game */
class GameStateRun : public GameState, public Singleton<GameStateRun>
{
public:
	/**
	Constructor
	@param app Game engine kernel. The application framework that is running the game.
	*/
	GameStateRun(Application &app);

	/** Destructor */
	virtual ~GameStateRun(void);

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	virtual void update(float deltaTime);

	/** Called when the state is entered */
	virtual void onEnter(void);

	/** Called when the state is exitted */
	virtual void onExit(void);

	/** Release assets */
	virtual void release(void);

	/** Reaquire assets */
	virtual void reaquire(void);

	/**
	Starts the screen blur effect
	@param timePeriod The time that the screen is blurred for
	@param color The color to shift the blur towards
	*/
	virtual void startBlurEffect(float timePeriod, COLOR color)
	{
		blurEffect.start(timePeriod, color);
	}

	/** Gets a reference to the trigger prompt but denies users from altering the pointer */
	inline TriggerPrompt& getPrompt(void) const
	{
		ASSERT(triggerPrompt!=0, _T("triggerPrompt was null"));
		return(*triggerPrompt);
	}

private:
	/** The screen blur effect when the player is hurt */
	BlurEffect blurEffect;

	/** Screen Dimmer */
	Dimmer dim;

	/** Alerts the user to a context sensitive action */
	TriggerPrompt *triggerPrompt;

	/** FPS label */
	PerformanceLabel *performanceLabel;

	/** Debugging data */
	DebugLabel *debugLabel;
};

} // namespace Engine

#endif
