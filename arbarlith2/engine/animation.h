/*
Author: Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu

Copyright © 2006-2007 Game Creation Society
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

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "mesh.h"
#include "keyframe.h"

namespace Engine {

/** Records keyframe timing and arrangement for ana animation */
class AnimationSequence
{
public:
	/**
	Loads the AnimationSequence from an XML source
	@param keyFrames All keyframes in the mesh
	@param name The name of the animation
	@param priority The priority of the animation.  If priority > 1, then the animation is "high priority"
	@param looping true if the naimation loops
	@param start Index into keyFrames for the start of the animation sequence
	@param length Number of keyframes in the animation sequence
	@param fps The frames per second of the animation sequence
	*/
	AnimationSequence(vector<KeyFrame> keyFrames, const _tstring &name, float priority, bool looping, int start, int length, float fps);

	/**
	Copy constructor
	@param animation The animation to copy
	*/
	AnimationSequence(const AnimationSequence &animation);

	/** Destructor */
	~AnimationSequence(void);

	/** Gets the name of the animation */
	const _tstring& getName(void) const
	{
		return m_strName;
	}

	/** Gets the animation's priority */
	float getPriority(void) const
	{
		return m_Priority;
	}

	/** Gets the current time into the animation */
	float getTime(void) const
	{
		return m_Time;
	}

	/**
	Gets the length of the animation in milliseconds
	@return milliseconds
	*/
	float getLength(void) const
	{
		return keyFrames.size() * 1000.0f / fps;
	}

	/**
	Indicates whether or not the animation will
	be allowed to always replace the current one.
	@return true if so
	*/
	bool isHighPriority(void) const
	{
		return(m_Priority>1.0f);
	}

	/**
	Gets the animation FPS
	@return fps
	*/
	float getFPS(void) const
	{
		return fps;
	}

	/** Gets the time scalar of the animation */
	float getSpeed(void) const
	{
		return m_TimeScalar;
	}

	/** Indicates whether or not the animation has finished yet */
	bool isFinished(void) const
	{
		return m_bFinished;
	}

	/** Indicates whether or not the animation is looping */
	bool isLooping(void) const
	{
		return m_bLooping;
	}

	/**
	Gets the number of key frames in the animation
	@return The number of key frames
	*/
	size_t getNumKeyFrames(void) const
	{
		return keyFrames.size();
	}

	/** Draws the animation at the current time */
	void draw(void);

	/**
	Updates the animation
	@param timeDelta The time elapsed since the last update
	*/
	void update(float timeDelta);

	/** Change the time into animation */
	void SetTime(float time);

	/** Change the speed of the animation */
	void setSpeed(float timeScalar)
	{
		m_TimeScalar = timeScalar;
	}

	/** Calculates the sphere radius of the animation at the specified time */
	float CalculateRadius(float Time);

	/** Calculates the cylindrical radius of the animation at the specified time */
	float CalculateCylindricalRadius(float Time);

	/** Calculates the sphere radius of the animation at the current time */
	float CalculateRadius(void)
	{
		return CalculateRadius(getTime());
	}

	/** Calculates the cylindrical radius of the animation at the current time */
	float CalculateCylindricalRadius(void)
	{
		return CalculateCylindricalRadius(getTime());
	}

	/**
	Calculates the height of the model at the given time in the animation
	@param time The time in the animation
	@return height of the model
	*/
	float CalculateHeight(float time);

	/**
	Calculates the FOV of the Mesh at the current frame
	@param mat The camera
	@param lxMax Returns the horizontal angle of the Mesh
	@param lyMax Returns the vertical angle of the Mesh
	*/
	void calculateFOV(mat4 &mat, float &lxMax, float &lyMax) const;

private:
	/**
	Copy the meshes to create a working set.
	We assume all keyframes have the same number of meshes and each corresponding mesh is identical except in vertex placement
	@param model keyframes meshes
	*/
	void createWorkingSetOfMeshes(const Model &model);

	/**
	Gets the frame of animation for the specified time
	@param millisecondsIntoAnimation The time into the animation to the get the key frame for
	@return The frame of animation
	*/
	const Model& getFrame(float millisecondsIntoAnimation) const;

	/**
	Gets the frame of animation by interploating between the given keyframes
	@param lowerFrame Index of the lower keyframe
	@param upperFrame Index of the upper keyframe
	@param bias 0.0 to 1.0 indicating the bias between the keyframes
	@return The frame of animation
	*/
	const Model& getFrame(size_t lowerFrame, size_t upperFrame, float bias) const;

	/**
	Gets the frame of animation for the current time
	@return The frame of animation
	*/
	const Model& getFrame(void) const
	{
		return getFrame(getTime());
	}

	/** A working copy of the mesh */
	mutable vector<Mesh*> meshes;

	/** The key frames of the animation */
	vector<KeyFrame> keyFrames;

	/** The name of the animation */
	_tstring m_strName;

	/** Used for determing the animation of choice during animation selection */
	float m_Priority;

	/** Time into the animation (milliseconds) */
	float m_Time;

	/** Multiply all Time Elapsed values by this to control animation speed */
	float m_TimeScalar;

	/** Whether or not the animation loops at the end of the sequence. */
	bool m_bLooping;

	/** base animation FPS */
	float fps;

	/**
	Flags whether the animaton has been completed
	If m_bLooping, then this is always false
	*/
	bool m_bFinished;
};

} //namespace Engine

#endif
