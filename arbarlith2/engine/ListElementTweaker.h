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

#ifndef _LISTELEMENTTWEAKER_H_
#define _LISTELEMENTTWEAKER_H_

#include "misc.h"
#include "TweakerWidget.h"
#include "ListElementWidget.h"
#include "primitivedatatypes.h"


namespace Engine {



/** This is a tweaker widet that may be used as an element in a list widget */
template<class TYPE>
class ListElementTweaker : public ListElementWidget
{
private:
	TweakerWidget<TYPE> *tweaker;

public:
	/**
	Constructs the widget
	@param label The label for the watched value
	@param value The value that will be watched
	*/
	ListElementTweaker(const _tstring &label, TYPE *value)
	{
		tweaker = new TweakerWidget<TYPE>(label, value, 12.0f, 1.0f);
		tweaker->dumb = true;
		AddChild(tweaker);
	}

	/**
	Processes the widget
	@param deltaTime The milliseconds since the last update
	*/
	virtual void update(float deltaTime)
	{
		// Process the widget events
		ListElementWidget::update(deltaTime);
	}

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void)
	{
		tweaker->onMouseEnter();
		ListElementWidget::onMouseEnter();
	}

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void)
	{
		tweaker->onMouseExit();
		ListElementWidget::onMouseExit();
	}

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void)
	{
		tweaker->onMouseClick();
		ListElementWidget::onMouseClick();
	}
};



/** This is a tweaker widet that may be used as an element in a list widget */
class ListElementTweakerString : public ListElementWidget
{
private:
	TweakerWidgetString *tweaker;

public:
	/**
	Constructs the widget
	@param label The label for the watched value
	@param value The value that will be watched
	*/
	ListElementTweakerString(const _tstring &label, _tstring *value)
	{
		tweaker = new TweakerWidgetString(label, value, 12.0f, 1.0f);
		tweaker->dumb = true;
		AddChild(tweaker);
	}

	/**
	Processes the widget
	@param deltaTime The milliseconds since the last update
	*/
	virtual void update(float deltaTime)
	{
		// Process the widget events
		ListElementWidget::update(deltaTime);
	}

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void)
	{
		tweaker->onMouseEnter();
		ListElementWidget::onMouseEnter();
	}

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void)
	{
		tweaker->onMouseExit();
		ListElementWidget::onMouseExit();
	}

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void)
	{
		tweaker->onMouseClick();
		ListElementWidget::onMouseClick();
	}
};


/** This is a tweaker widet that may be used as an element in a list widget */
class ListElementTweakerXML : public ListElementWidget
{
private:
	TweakerWidgetXML *tweaker;

public:
	/**
	Constructs the widget
	@param label The label for the watched value
	@param value The value that will be watched
	*/
	ListElementTweakerXML(const _tstring &label, XmlDataType *value)
	{
		tweaker = new TweakerWidgetXML(label, value, 12.0f, 1.0f);
		tweaker->dumb = true;
		AddChild(tweaker);
	}

	/**
	Processes the widget
	@param deltaTime The milliseconds since the last update
	*/
	virtual void update(float deltaTime)
	{
		// Process the widget events
		ListElementWidget::update(deltaTime);
	}

	/** Called in the even of a mouse enter event */
	virtual void onMouseEnter(void)
	{
		tweaker->onMouseEnter();
		ListElementWidget::onMouseEnter();
	}

	/** Called in the even of a mouse exit event */
	virtual void onMouseExit(void)
	{
		tweaker->onMouseExit();
		ListElementWidget::onMouseExit();
	}

	/** Called in the even of a mouse click */
	virtual void onMouseClick(void)
	{
		tweaker->onMouseClick();
		ListElementWidget::onMouseClick();
	}
};

} // namespace Engine

#endif
