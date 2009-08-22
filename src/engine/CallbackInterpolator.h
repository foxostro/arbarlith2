/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright © 2004-2007 Game Creation Society
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

#ifndef _CALLBACK_INTERPOLATOR_H_
#define _CALLBACK_INTERPOLATOR_H_

#include "task.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace Engine {



/**
Interpolates a value over time.  Kernel task to cause a value
to be interpolated between a starting and ending value over a
period of time.  Once its started, it carries out its
execution seamlessly and without intervention.

When the task has completed, a callback is executed

While the interpolator is running, the value should be
considered read only.  Writes to the value will be overwritten
by the interpolator task on its next update.
*/
template < class FN >
class CallbackInterpolator : public Task
{
public:
	/** Destructor */
	virtual ~CallbackInterpolator(void)
	{}

	/**
	Constructs the interpolator
	@param value Points to the value to modify
	@param startingValue The starting value
	@param endingValue The ending value
	@param timeLimit The value will reach the end value at this time (milliseconds)
	@param fn The callback function
	*/
	CallbackInterpolator(float *value, float startingValue, float endingValue, float timeLimit, FN fn)
	{
		ASSERT(value!=0, "value was null");

		this->value = value;
		this->startingValue = startingValue;
		this->endingValue = endingValue;
		this->myAge = 0.0f;
		this->timeLimit = timeLimit;
		this->fn = fn;
	}

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime)
	{
		myAge += deltaTime;

		if(myAge < timeLimit)
		{
			float bias = myAge / timeLimit;

			(*value) = startingValue + bias*(endingValue - startingValue);
		}
		else
		{
			dead = true;

			(*value) = endingValue;

			fn();
		}
	}

private:
	/** The value to be modified */
	float *value;

	/** The starting value*/
	float startingValue;

	/** The ending value */
	float endingValue;

	/** Tracks the age of the task (milliseconds) */
	float myAge;

	/** The time during which interpolation will occur*/
	float timeLimit;

	/** Callback function */
	FN fn;
};

/**
Creates an interpolator
@param value Points to the value to modify
@param startingValue The starting value
@param endValue The ending value
@param timeLimit The value will reach the end value at this time (milliseconds)
@param fn The callback function
*/
template<typename T>
static CallbackInterpolator<T>* makeCallbackInterpolator(float *value, float startingValue, float endValue, float time, T fn)
{
	return new CallbackInterpolator<T>(value, startingValue, endValue, time, fn);
}



} // namespace Engine

#endif
