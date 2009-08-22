/*
Original Author: Andrew Fox
E-Mail: mailto:foxostro@gmail.com

Copyright (c) 2003-2007,2009 Game Creation Society
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

#ifndef _WIDGET_H_
#define _WIDGET_H_

namespace Engine {

/** a GUI widget */
class Widget
{
public:
	/** Destructor */
	virtual ~Widget(void);

	/** Constructor */
	Widget(void);

	/**
	Adds a child widget
	@param widget Ownership of the given widget passes to this object
	*/
	void AddChild(Widget *widget);

	/**
	Renders the widget and its children
	@param base Position of the parent
	*/
	virtual void RenderTree(const vec2 &base) const;

	/**
	Updates the widget and its children
	@param deltaTime Millseconds since the last update
	*/
	virtual void ProcessTree(float deltaTime);

	/** Handle our and our children's events */
	virtual void handleEventsTree(void);

	/**	Deletes all child widgets */
	virtual void destroy(void);

	/**
	Gets the x-coord of the widget in absolute coords
	@return X
	*/
	inline float getBaseX(void) const
	{
		return absPos.x;
	}

	/**
	Gets the y-coord of the widget in absolute coords
	@return Y
	*/
	inline float getBaseY(void) const
	{
		return absPos.y;
	}

	/**
	Gets the x-coord of the widget in relative coords
	@return X
	*/
	inline float getRelativeX(void) const
	{
		return relPos.x;
	}

	/**
	Gets the y-coord of the widget in relative coords
	@return Y
	*/
	inline float getRelativeY(void) const
	{
		return relPos.y;
	}

	/**
	Gets the width of the widget
	@return width
	*/
	inline float getWidth(void) const
	{
		return dim.x;
	}

	/**
	Sets the position of the widget in relative coords
	@return Y
	*/
	virtual void setRelativePos(const vec2 &pos)
	{
		relPos = pos;
	}

	/**
	Sets the position of the widget in relative coords
	@return Y
	*/
	inline void setRelativePos(float x, float y)
	{
		setRelativePos(vec2(x,y));
	}

	/**
	Gets the height of the widget
	@return Y
	*/
	inline float getHeight(void) const
	{
		return dim.y;
	}

	/**
	Sets the width of the widget
	@param w The new width
	*/
	virtual void setWidth(float w)
	{
		dim.x = w;
	}

	/**
	Sets the height of the widget
	@param h The new height
	*/
	virtual void setHeight(float h)
	{
		dim.y = h;
	}

	/**
	Indicates that the widget is visible
	@return true if the widget is visible, false otherwise
	*/
	inline bool isVisible(void) const
	{
		return m_bVisible;
	}

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void) {}

	/** Called in the even of a mouse down event */
	virtual void onMouseDown(void) {}

	/** Called in the even of a mouse up event */
	virtual void onMouseUp(void) {}

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void) {}

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void) {}

protected:
	/** Draws the widget */
	virtual void draw(void) const {}

	/**
	Updates the widget
	@param deltaTime Milliseconds since the last tick
	*/
	virtual void update(float deltaTime);

	/** Handles events from user input */
	virtual void handleEvents(void);

	/** Removes and deletes all dead children */
	virtual void pruneDeadChildren(void);

	/**
	Determines whether the mouse is within the widget
	@return true if the mouse is within the widget
	*/
	bool isWithin(void) const;

public:
	/** Indicate that the widget is visible */
	bool m_bVisible;

	/** Indicates that another widget will be passing us event messages. */
	bool dumb;

	/** Identifying name of the widget */
	string widgetName;

	/** indicates to the parent widget that they are free to delete this widget */
	bool dead;

protected:
	/** indicates that the widget is depressed */
	bool depressed;

	/** indicates that the mouse is within the bounds of the widget */
	bool within;

	/** indicates that the mouse is over the widget */
	bool hover;

	/** The mouse was down in the last tick */
	bool mouseDown;

	/** Points to the parent widget */
	Widget *parent;

private:
	/** Position of the widget relative to the parent */
	vec2 relPos;

	/** Position of the widget relative to the window */
	mutable vec2 absPos;

	/** Dimensions of the widget */
	vec2 dim;

	/** children */
	list<Widget*> m_Children;

	float getMouseX(void) const;
	float getMouseY(void) const;
};



} // namespace Engine



#endif
