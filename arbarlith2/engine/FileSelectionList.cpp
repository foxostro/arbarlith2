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

#include "stdafx.h"
#include "searchfile.h"
#include "FileSelectionList.h"
#include "ListElementLabel.h"


namespace Engine {



FileSelectionList::FileSelectionList(const string &pattern, float x, float y)
:SelectionList(x, y)
{
	SearchFile files(pattern);
	for(vector<string>::const_iterator iter=files.m_Files.begin(); iter!=files.m_Files.end(); ++iter)
	{
		string fileName =(*iter);

		ListElementLabel *fileWidget = new ListElementLabel(fileName,
															"data/sprites/list/listwidget_depressed.png",
															"data/sprites/list/listwidget_depressed_over.png",
															"data/sprites/list/listwidget.png",
															"data/sprites/list/listwidget_hover.png");

		selected = fileWidget; // the last widget added will ende up being the selected widget

		addSelectableElement(fileWidget);
	}
}

string FileSelectionList::getFilename(void)
{
	ListElementWidget *selected = getSelected();

	ListElementLabel *label = dynamic_cast<ListElementLabel*>(selected);

	if(label)
	{
		return label->getLabel();
	}
	else
	{
		return string();
	}
}

void FileSelectionList::update(float)
{
	// Has a selectable element been clicked?
	for(list<ListElementWidget*>::iterator iter=selectableElements.begin(); iter!=selectableElements.end(); ++iter)
	{
		ListElementLabel *label = dynamic_cast<ListElementLabel*>(*iter);

		if(label)
		{
			label->isActive=false;
		}
	}

	// Only highlight the selected filename
	ListElementLabel *label = dynamic_cast<ListElementLabel*>(selected);
	if(label)
		label->isActive=true;
}

}; // namespace
