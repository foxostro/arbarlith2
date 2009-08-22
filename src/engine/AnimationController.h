/*
Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2004-2007,2009 Game Creation Society
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

#ifndef _ANIMATION_CONTROLLER_H_
#define _ANIMATION_CONTROLLER_H_

#include "animation.h"

namespace Engine {

/** Manages a set of animation sequences */
class AnimationController
{
private:
	/** Stores a list of all the model's animations */
	vector<AnimationSequence> m_Animations;

	/** Records which of the above animation objects is currently being utilized by the model */
	size_t m_nCurrentAnimation;

public:
	/** Constructs a blank animation controller */
	AnimationController(void);

	/** Draws the current frame of the current animation at the current coordinate system's origin */
	void draw(void);

	/**
	Updates the current animation
	@param timeStep time since the last tick
	*/
	void update(float timeStep);

	/**
	Gets the specified animation
	@param handle the animation to retrieve
	*/
	const AnimationSequence& getAnimation(size_t handle) const;

	/**
	Gets the current animation
	@return The current animation
	*/
	const AnimationSequence& getAnimation(void) const;

	/**
	Gets the current animation
	@return The current animation
	*/
	AnimationSequence& getAnimation(void);

	/**
	Adds an animation sequence to the controller
	@param animation Animation sequence to add
	@return The animation handle
	*/
	size_t addAnimation(AnimationSequence &animation);

	/**
	Gets the handle of the current animation
	@return Current animation's handle
	*/
	size_t getAnimationHandle(void)
	{
		return m_nCurrentAnimation;
	}

	/**
	Gets the number of animations contained within the controller
	@return Number of animations
	*/
	size_t getNumAnimations(void) const
	{
		return m_Animations.size();
	}

	/**
	Looks up an animation handle
	@param name The name of the animation
	@return The animation handle
	@throws std::invalid_argument When the animation does not exist
	*/
	size_t getAnimationHandle(const string &name) const;

	/**
	Records an animation request
	@param name The name of the animation
	@param speed The speed of the animation
	*/
	inline bool requestAnimationChange(const string &name, float speed)
	{
		return requestAnimationChange(getAnimationHandle(name), speed);
	}

	/**
	Records an animation request
	@param handle The handle of the animation
	@param speed The speed of the animation
	*/
	bool requestAnimationChange(size_t handle, float speed);

	/**
	Gets the time into the current animation
	@return time into the animation
	*/
	float getTime(void) const
	{
		return getAnimation().getTime();
	}

	/**
	Gets the duration of the current animation
	@return duration of the current animation
	*/
	float getDuration(void) const
	{
		return getAnimation().getLength();
	}

	/**
	Calculates and returns the radius of the smallest sphere entirely enclosing the model
	@param time The time into the current animation
	@return radius
	*/
	float calculateRadius(float time);

	/**
	Calculates and returns the radius of the smallest cylinder entirely enclosing the model
	@param time The time into the current animation
	@return cylinder radius
	*/
	float calculateCylindricalRadius(float time);

	/**
	Calculates and returns the height of the model
	@param time The time into the current animation
	@return height
	*/
	float calculateHeight(float time);

private:
	/** Reset all variables to their default state; A constructor helper function. */
	void clear(void);
};

} //namespace Engine

#endif
