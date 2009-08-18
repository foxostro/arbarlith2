/*
Original Author: Andrew Fox
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

#include "PropertyBag.h"
#include "mesh.h"

#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_


namespace Engine {

/** Several meshes make up a model at a single key frame */
typedef vector<Mesh*> Model;

/** Stores all data related to a particular key frame in an animation */
class KeyFrame
{
private:
	/** Reference to the meshes involved in the keyframe */
	Model meshes;

public:
	/**
	Constructs the keyframe from a single mesh
	@param model The mesh to use for this keyframe
	*/
	KeyFrame(Mesh* mesh);

	/**
	Constructs the keyframe from a model
	@param model The mesh to use for this keyframe
	*/
	KeyFrame(Model model);

	/**
	Copy costructor
	@param keyframe The key frame to copy
	*/
	KeyFrame(const KeyFrame &keyframe);

	/**
	Assignment operator
	@param keyframe The keyframe to copy
	@return The new value of this keyframe
	*/
	KeyFrame &operator=(const KeyFrame &keyframe);

	/**
	Gets the model associated with the key frame
	@param i Index of the mesh
	@return a pointer to the model associated with this key frame
	*/
	Mesh* getMesh(size_t i) const
	{
		return meshes[i];
	}

	/**
	Gets the models associated with the key frame
	@return a pointer to the model associated with this key frame
	*/
	Model & getMeshes(void)
	{
		return meshes;
	}

	/**
	Gets the models associated with the key frame
	@return a pointer to the model associated with this key frame
	*/
	const Model & getMeshes(void) const
	{
		return meshes;
	}

	/**
	Merges the keyframes with the given keyframes.
	Fails when the number of keyframes is mismatched.
	However, if this object is empty, then this acts to copy the keyframes over and nothing more.
	@param keyFrames extra meshes
	@return true if the merge was successful
	*/
	bool merge(const KeyFrame &o);
};

} //namespace Engine

#endif
