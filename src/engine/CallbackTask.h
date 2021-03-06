/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2007,2009 Game Creation Society
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

#ifndef _CALLBACK_TASK_H_
#define _CALLBACK_TASK_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "task.h"

namespace Engine {

/** After a delay period, a callback function is executed */
template < class FN >
class CallbackTask : public Task
{
public:
	/** Destructor */
	virtual ~CallbackTask(void)
	{}

	/**
	Constructs the task
	@param delay After this delay, the callback function is called
	@param fn The callback function
	*/
	CallbackTask(float delay, FN fn)
	{
		this->myAge = 0.0f;
		this->delay = delay;
		this->fn = fn;
	}

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime)
	{
		myAge += deltaTime;

		if(myAge >= delay)
		{
			dead = true;
			fn();
		}
	}

private:
	/** Tracks the age of the task (milliseconds) */
	float myAge;

	/** The time during which interpolation will occur*/
	float delay;

	/** Callback function */
	FN fn;
};

/**
Creates an interpolator
@param delay The callback function will be called after this delay
@param fn The callback function
*/
template<typename T>
static CallbackTask<T>* makeCallbackTask(float delay, T fn)
{
	return new CallbackTask<T>(delay, fn);
}

} // namespace Engine

#endif
