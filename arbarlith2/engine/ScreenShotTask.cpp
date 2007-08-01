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

/*
Include the DevIL headers
*/
#include <IL/il.h>
#include <IL/ilut.h>

#include "ScreenShotTask.h"
#include "searchfile.h"

namespace Engine {

int stoi(const string &s); // stdafx.cpp

ScreenShotTask::ScreenShotTask(void)
:screenShotDebounce(false)
{}

void ScreenShotTask::update(float)
{
	// Take a screen shot and save it to file
	if(g_Keys.isKeyDown(KEY_SCREENSHOT) )
	{
		if(screenShotDebounce == false)
		{
			screenShotDebounce = true;

			takeScreenShot();
		}
	}
	else
	{
		screenShotDebounce = false;
	}
}

void ScreenShotTask::takeScreenShot(void)
{
	ILuint handle=0;
	ilGenImages(1, &handle);

	ilBindImage(handle);
	ilutGLScreen();

	createDirectory("sshots/");

	char *pszScreenShotFileName = strdup(getScreenShotFileName());
	ilSaveImage(pszScreenShotFileName);
	delete[] pszScreenShotFileName;

	ilDeleteImages(1, &handle);
}

string ScreenShotTask::getScreenShotFileName(void)
{
	int highestNumber = 0;

	const string screen = string("screen");
	const string ext = string(".jpg");

	SearchFile files("sshots/*.jpg");

	for(vector<string>::const_iterator iter=files.m_Files.begin();
	    iter!=files.m_Files.end();
	    ++iter)
	{
		const string fileName =(*iter);
		const string strNum = fileName.substr(screen.length(),
		fileName.length() - screen.length() - ext.length());
		const int number = stoi(strNum);

		if(number>highestNumber)
		{
			highestNumber = number;
		}
	}

	return string("sshots/screen") + itoa(++highestNumber) + ext;
}

} //namespace Engine
