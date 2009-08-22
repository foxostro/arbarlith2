/*
Author: Andrew Fox
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

#include "stdafx.h"
#include "AnimationController.h"



namespace Engine {




AnimationController::AnimationController(void)
{
	clear();
}

void AnimationController::draw(void)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	m_Animations[m_nCurrentAnimation].draw();
}

void AnimationController::update(float milliseconds)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	m_Animations[m_nCurrentAnimation].update(milliseconds);
}

const AnimationSequence& AnimationController::getAnimation(void) const
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid animation handle: ") + itoa((int)m_nCurrentAnimation));
	return m_Animations[m_nCurrentAnimation];
}

AnimationSequence& AnimationController::getAnimation(void)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid animation handle: ") + itoa((int)m_nCurrentAnimation));
	return m_Animations[m_nCurrentAnimation];
}

const AnimationSequence& AnimationController::getAnimation(size_t handle) const
{
	ASSERT(handle < m_Animations.size(), string("Invalid animation handle: ") + itoa((int)handle));
	return m_Animations[handle];
}

size_t AnimationController::addAnimation(AnimationSequence &animation)
{
	m_Animations.push_back(animation);
	return m_Animations.size()-1;
}

size_t AnimationController::getAnimationHandle(const string &name) const
{
	// for all animations
	for(size_t i=0; i<m_Animations.size(); ++i)
	{
		const AnimationSequence &animation = m_Animations[i];

		// If this animation has the same name as the one we seek
		if(animation.getName() == name)
		{
			return i;
		}
	}

	return 0;
}

bool AnimationController::requestAnimationChange(size_t handle, float speed)
{
	ASSERT(handle				< m_Animations.size(),	string("Invalid handle: ") + itoa((int)handle));
	ASSERT(m_nCurrentAnimation	< m_Animations.size(),	string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	AnimationSequence &current = m_Animations[m_nCurrentAnimation];
	AnimationSequence &requested = m_Animations[handle];

	// Fail if the current is animation is super high priority
	if(!current.isFinished() && current.isHighPriority())
		return false;

	// Fail if the current animation has a higher priority
	if(!current.isFinished() && current.getPriority() > requested.getPriority())
		return false;

	// Fail if the requested animation is already playing
	if(m_nCurrentAnimation == handle)
	{
		// change the speed though
		m_Animations[m_nCurrentAnimation].setSpeed(speed);

		// If the current animation is already playing, but IS finished, then fall-through to restart the animation
		if(!current.isFinished())
			return false;
	}

	// Fulfill the request
	m_nCurrentAnimation = handle;
	m_Animations[m_nCurrentAnimation].SetTime(0.0f);
	m_Animations[m_nCurrentAnimation].setSpeed(speed);

	return true;
}

float AnimationController::calculateCylindricalRadius(float Time)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	return m_Animations[m_nCurrentAnimation].CalculateCylindricalRadius(Time);
}

float AnimationController::calculateRadius(float Time)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	return m_Animations[m_nCurrentAnimation].CalculateRadius(Time);
}

float AnimationController::calculateHeight(float Time)
{
	ASSERT(m_nCurrentAnimation < m_Animations.size(), string("Invalid m_nCurrentAnimation: ") + itoa((int)m_nCurrentAnimation));

	return m_Animations[getAnimationHandle("idle")].CalculateHeight(Time);
}

void AnimationController::clear(void)
{
	m_nCurrentAnimation = 0;
	m_Animations.clear();
}

}; // namespace
