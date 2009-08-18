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

#ifndef _TWEAKER_WIDGET_H_
#define _TWEAKER_WIDGET_H_

#include "primitivedatatypes.h"
#include "EditWidget.h"
#include <sstream>
#include <string>


namespace Engine {



class TweakerWidgetString : public EditWidget
{
private:
	string *myValue;
	string myLabel;

public:
	TweakerWidgetString(const string &label, string *value, const vec2 &pos)
	:EditWidget("null", pos)
	{
		myLabel = label;
		myValue = value;
	}

	virtual void update(float deltaTime)
	{
		EditWidget::update(deltaTime);

		if(!hasFocus())
		{
			string label = myLabel + string(": ") + getString();

			// If the text is too long, we have to cut it off
			if(label.size() > 27)
			{
				label = label.substr(0,23) + "...";
			}

			setLabel(label);
		}
	}

	virtual string getString(void)
	{
		return (*myValue);
	}

	virtual string getValue(void)
	{
		(*myValue) = getLabel();

		return(*myValue);
	}

	/**	Called in the event that focus is acquired */
	virtual void onAcquireFocus(void)
	{
		EditWidget::onAcquireFocus();
		setLabel(getString());
	}

	/**	Called in the event that focus is lost */
	virtual void onLostFocus(void)
	{
		EditWidget::onLostFocus();
		getValue();
	}
};

template<class TYPE>
class TweakerWidget : public EditWidget
{
private:
	string myLabel;

	TYPE *myValue;

public:
	TweakerWidget(const string &label, TYPE *value, const vec2 &pos)
	: EditWidget("null", pos),
	  myLabel(label),
	  myValue(value)
	{}

	virtual void update(float deltaTime)
	{
		EditWidget::update(deltaTime);

		if(!hasFocus())
		{
			string label = myLabel + string(": ") + getString();

			// If the text is too long, we have to cut it off
			if(label.size() > 27)
			{
				label = label.substr(0,23) + "...";
			}

			setLabel(label);
		}
	}

	virtual string getString(void)
	{
		stringstream ss;
		string ret;

		ss << (*myValue);
		ss >> ret;

		return ret;
	}

	virtual TYPE getValue(void)
	{
		stringstream ss;

		ss << getLabel().c_str();
		ss >> (*myValue);

		return(*myValue);
	}

	/** Called in the event that focus is acquired */
	virtual void onAcquireFocus(void)
	{
		EditWidget::onAcquireFocus();
		setLabel(getString());
	}

	/** Called in the event that focus is lost */
	virtual void onLostFocus(void)
	{
		EditWidget::onLostFocus();
		getValue();
	}
};

class TweakerWidgetXML : public EditWidget
{
private:
	XmlDataType *myValue;
	string myLabel;

public:
	TweakerWidgetXML(const string &label, XmlDataType *value, const vec2 &pos)
	:EditWidget("null", pos)
	{
		myLabel = label;
		myValue = value;
	}

	virtual void update(float deltaTime)
	{
		EditWidget::update(deltaTime);

		if(!hasFocus())
		{
			string label = myLabel + string(": ") + getString();

			// If the text is too long, we have to cut it off
			if(label.size() > 27)
			{
				label = label.substr(0,23) + "...";
			}

			setLabel(label);
		}
	}

	virtual string getString(void)
	{
		XmlDataType *value = static_cast<XmlDataType*>(myValue);
		string ret = value->ToString();
		return ret;
	}

	virtual void getValue(void)
	{
		string str = getLabel();

		XmlDataType *value = static_cast<XmlDataType*>(myValue);
		value->FromString(str);
	}

	/**	Called in the event that focus is acquired */
	virtual void onAcquireFocus(void)
	{
		EditWidget::onAcquireFocus();
		setLabel(getString());
	}

	/**	Called in the event that focus is lost */
	virtual void onLostFocus(void)
	{
		EditWidget::onLostFocus();
		getValue();
	}
};

} // namespace Engine

#endif
