/*
2004-2007 Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
Modified to use SDL windowing by Tom Cauchois, Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu
*/

#include "stdafx.h"
#include "gl.h"
#include "SDLwindow.h"
#include "image.h"
#include "TextureHandle.h"
#include "WaitScreen.h"

namespace Engine {

WaitScreen::WaitScreen(void)
{
	const _tstring nill = _T("");
	const _tstring waitScreenImage = _T("data/sprites/backdrops/wait.jpg");

	backDrop.create(nill,
					waitScreenImage,
					nill,
					nill);
}

WaitScreen::~WaitScreen(void)
{}

void WaitScreen::Render(void)
{
	CHECK_GL_ERROR();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	effect_Begin(effect_GUI);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	CHECK_GL_ERROR();

	// Set up the model view matrix
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.0f);

	// Select the splash screen or wait screen
	backDrop.getTexture().bind();

	// Render a textured quad over the screen
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1024.0f,   0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1024.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(   0.0f, 768.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(   0.0f,   0.0f, 0.0f);
	glEnd();

	CHECK_GL_ERROR();

	glPopAttrib();
	effect_End();


	// Swap buffers
	glFlush();

	g_Window.Flip();


// Message Pumping
	g_Input.Pump();

	// check for sizing
	if(g_Input.ResizePending())
	{
		int width = g_Input.ResizeWidth(),
			height = g_Input.ResizeHeight(); // screen dimensions

		// Resize the window
		g_Window.Resize(width,height);
		OpenGL::GetSingleton().ReSizeGLScene(width,height);
	}

	CHECK_GL_ERROR();
}

}; // namespace
