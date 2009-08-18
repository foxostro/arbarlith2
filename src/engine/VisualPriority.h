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

#ifndef _VISUAL_PRIORITY_H_
#define _VISUAL_PRIORITY_H_


namespace Engine {



/**
Rating of an object's importance to the scene.
The object's vidual priority is used to determine
whether the object can be rendered simplistically,
or not at all, and still retain the original detail
visible on the screen.
<br/>
Several factors are relevant to visual priority:
1.	Whether the object is visible to the camera
	* Occluded or off-screen objects need not be rendered
	* Objects that are partially occluded may (or may not
	  be rendered in less detail
2.	The size of the object on the screen (in pixels)
	* Low screen resolutions obscure details, removing the
	  need for high quality textures
	* The distance from the camera and the screen resolution
	  affect how far away an object can be before it may be
	  rendered in decreased detail.
3.	The artistic importance of the object
	* Artist-defined value For example, a chair should
	  downsize its shadow map before the dragon in the room.
4.	How brightly or dimly lit the object is
	* Dimly lit objects or shadows in a dark enviroment may
	use less detailed textures without penalty.
*/
class VisualPriority
{
public:
	/** Indicates that the object has moved in this tick */
	bool hasMoved;

	/**
	Indicates that the object has changed in mesh geometry -- possibly as a result of keyframe animation.
	If this is the case, then the object's appearance on screen will have been altered.
	*/
	bool hasAnimated;

protected:
	/**
	A number to describe the current importance of the object to the
	visual representation of a scene.<br/>
	The value is clamped to the range [0.0, 1.0], where 0.0 indicates
	that an object is completely irrelevant to a scene.  A value of 1.0
	indicates that the object is completely essential in full detail.
	No less detail than the maximum is acceptable in such a case.
	*/
	float visualPriority;

	/** Indicates that the object needs to have its visual priority updated	*/
	bool needsVisualPriorityUpdate;

public:
	/** Default constructor*/
	VisualPriority(void);

	/** Destructor */
	virtual ~VisualPriority(void);

	/**
	Gets the visual priority of the object
	@return visual priority: a number to describe the importance
	of the object to the visual representation of a scene.
	*/
	float getVisualPriority(void) const
	{
		return visualPriority;
	}

	/**
	Recalculates the visual priority of the object
	@return visual priority: a number to describe the importance
	of the object to the visual representation of a scene.
	*/
	virtual float calculateVisualPriority(void)=0;

	/**
	Handles the event where an object has moved within the last tick.
	This event handler is called at most once per tick.
	*/
	virtual void onMoved(void)=0;

	/**
	Handles the event where an object has been animated within the last tick.
	This event handler is called at most once per tick.
	*/
	virtual void onAnimated(void)=0;
};


}; // namespace


#endif
