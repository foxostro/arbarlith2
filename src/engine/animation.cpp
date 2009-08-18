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

#include "stdafx.h"
#include "animation.h"

namespace Engine {

AnimationSequence::AnimationSequence
(
	vector<KeyFrame> keyFrames,
	const string &name,
	float priority,
	bool looping,
	size_t start,
	size_t length,
	float fps
)
: m_strName(name),
  m_Priority(priority),
  m_Time(0.0f),
  m_TimeScalar(1.0f),
  m_bLooping(looping),
  fps(fps),
  m_bFinished(false)
{
	for(size_t i=0; i<length; ++i)
	{
		this->keyFrames.push_back(keyFrames[start+i]);
	}

	createWorkingSetOfMeshes(keyFrames[start].getMeshes());
}

AnimationSequence::AnimationSequence(const AnimationSequence &animation)
: keyFrames(animation.keyFrames),
  m_strName(animation.m_strName),
  m_Priority(animation.m_Priority),
  m_Time(animation.m_Time),
  m_TimeScalar(animation.m_TimeScalar),
  m_bLooping(animation.m_bLooping),
  fps(animation.fps),
  m_bFinished(animation.m_bFinished)
{
	createWorkingSetOfMeshes(animation.meshes);
}

AnimationSequence::~AnimationSequence(void)
{
	for(size_t i=0; i<meshes.size(); ++i) delete meshes[i];
	meshes.clear();
}

void AnimationSequence::createWorkingSetOfMeshes(const Model &model)
{
	for_each(model.begin(), model.end(),
				bind(&vector<Mesh*>::push_back,
						&meshes,
						bind(new_ptr<Mesh>(), _1)
					)
			);
}

void AnimationSequence::draw(void)
{
	const vector<Mesh*> & meshes = getFrame();
	for_each(meshes.begin(), meshes.end(), bind(&Mesh::draw, _1));
}

void AnimationSequence::update(float millisecondsDelta)
{
	m_Time += millisecondsDelta * m_TimeScalar;

	// Loop the animation if it goes past the end
	if(m_Time > getLength())
	{
		if(m_bLooping == true)
		{
			while(m_Time > getLength()) m_Time -= getLength();
			m_bFinished = false;
		}
		else
		{
			m_bFinished = true;
			m_Time = getLength();
		}
	}
}

void AnimationSequence::SetTime(float Time)
{
	m_Time = Time;
	m_bFinished = false;
}

float AnimationSequence::CalculateCylindricalRadius(float Time)
{
	vector<Mesh*> meshes = getFrame(Time);

	if(meshes.empty())
	{
		return 0.0f;
	}
	else
	{
		vector<float> radii;
		transform(meshes.begin(), meshes.end(), back_inserter(radii), bind(&Mesh::calculateCylindricalRadius, _1));
		sort(radii.begin(), radii.end(), greater<float>());

		return radii[0];
	}
}

static float getTop(BoundingBox b) { return b.m_Max.y; }
static float getBtm(BoundingBox b) { return b.m_Min.y; }

float AnimationSequence::CalculateHeight(float Time)
{
	vector<Mesh*> meshes = getFrame(Time);

	if(meshes.empty())
	{
		return 0.0f;
	}
	else
	{
		vector<float> top, btm;

		transform(meshes.begin(), meshes.end(), back_inserter(top), bind(&getTop, bind(&Mesh::calculateBoundingBox, _1)));
		transform(meshes.begin(), meshes.end(), back_inserter(btm), bind(&getBtm, bind(&Mesh::calculateBoundingBox, _1)));

		sort(top.begin(), top.end(), greater<float>());
		sort(btm.begin(), btm.end(), less<float>());

		return(top[0] - btm[0]);
	}
}

float AnimationSequence::CalculateRadius(float Time)
{
	vector<Mesh*> meshes = getFrame(Time);

	if(meshes.empty())
	{
		return 0.0f;
	}
	else
	{
		vector<float> radii;

		transform(meshes.begin(), meshes.end(), back_inserter(radii), bind(&Mesh::calculateRadius, _1));
		sort(radii.begin(), radii.end(), greater<float>());

		return radii[0];
	}
}

void AnimationSequence::calculateFOV(mat4 &mat, float &lxMax, float &lyMax) const
{
	vec3 vert1, vert2;

	vector<Mesh*> s = getFrame();

	// for each mesh
	for(vector<Mesh*>::iterator i=s.begin(); i!=s.end(); ++i)
	{
		const Mesh *mesh = (*i);

		// For each vertex
		for(int i=0; i<mesh->m_numOfVerts; ++i)
		{
			// Transform the vertex by the matrix
			vert1 = mesh->m_pVerts[i];
			vert1.w = 1.0f;
			vert2 = mat.transformVector(vert1);

			// Calculate the spread, keep the max
			lxMax = max(lxMax, fabsf(vert2.x / vert2.z));
			lyMax = max(lyMax, fabsf(vert2.y / vert2.z));
		}
	}
}

const Model& AnimationSequence::getFrame(float milliseconds) const
{
	ASSERT(!keyFrames.empty(), "no keyframes in the animation sequence");
	ASSERT(milliseconds>=0.0f, "Given time is before the beginning of the animation");

	/*

	TODO: Find out why this assert trips up so much in the Linux build:

	ASSERT(milliseconds<=getLength(), "Given time is past the end of the animation");

	*/


	if(keyFrames.size() == 1)
		return(keyFrames[0].getMeshes());

	if(milliseconds > getLength())
		milliseconds = getLength();

	const float frameOfAnimation = (milliseconds / getLength()) * (keyFrames.size() - 1);
	const size_t lowerFrame = (size_t)floor(frameOfAnimation);
	const size_t upperFrame = (size_t)ceil(frameOfAnimation);
	const float bias = frameOfAnimation - lowerFrame;

	return getFrame(lowerFrame, upperFrame, bias);
}

const Model& AnimationSequence::getFrame(size_t lowerFrame, size_t upperFrame, float bias) const
{
	ASSERT(!keyFrames.empty(), "no keyframes in the animation sequence");
	ASSERT(lowerFrame < keyFrames.size(), "lower keyframe out of range:" + itoa((int)lowerFrame));
	ASSERT(upperFrame < keyFrames.size(), "upper keyframe out of range: "+ itoa((int)upperFrame));
	ASSERT(bias >= 0.0f && bias <= 1.0f, "bias is out of range: " + ftoa(bias));

	if(lowerFrame == upperFrame)
		return(keyFrames[lowerFrame].getMeshes());

	const Model &a = keyFrames[lowerFrame].getMeshes();
	const Model &b = keyFrames[upperFrame].getMeshes();

	for(size_t i=0; i<meshes.size(); ++i)
		meshes[i]->interpolate(bias, a[i], b[i]);

	return(meshes);
}

}; // namespace
