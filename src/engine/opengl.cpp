/*
Modified by Andrew Fox in 2003,2007,2009
E-Mail: mailto:foxostro@gmail.com
Modified to decouple with windowing by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu
*/

/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		OpenGL Control class

	Version History:
		v1.00		Original class	(Daniel Vivas 03/08/02)
		v1.10		Rewritten for inclusion in SDK	(GA 29/10/02)
		v1.11		Fixed palette significance		(GA 13/01/03)

	Licence:

	This SDK/library is free software, written primarily for
	teaching purposes. You can redistribute and modify it to
	your heart's content. However, any use of this library is
	at the user's own risk. There is no warranty. The authors
	will not be held responsible for ANYTHING that happens as
	a result of the use of this software. All financial/emotional/health
	problems or anything else that happens after using this
	software is therefore the user's own responsibility.
	Nevertheless, if the user finds it useful, an email or a
	mention in any credits would be appreciated, but is not
	necessary.
*/
// Modified by Andrew Fox in 2003-2006

#include "stdafx.h" // Master Header
#include "gl.h"
#include "opengl.h"
#include "COLOR.h"


namespace Engine {



using NeHe::Frame;

/** Indicates support for GL_ARB_multitexture */
bool g_bUseMultitexture = false;

/** Indicates the number of available hardware multitexture units */
int g_MultitextureUnits = 1;

/** Indicates support for GL_EXT_framebuffer_object */
bool g_bUseFramebufferObjects = false;

/** Indicates support for GL_ARB_depth_texture */
bool g_bUseDepthTexture = false;

/** Indicates support for shadows */
bool supportsShadow = false;

/** Indicates support for aniostropic texture filter */
bool supportsAniostropy = false;

// All Setup For OpenGL Goes Here
OpenGL :: OpenGL()
{
   stencil = 0; // stencil buffer bits
   m_NearClip = 0.1f;
   m_FarClip = 2000.0f;
   m_AspectRatio = 1.0f;
   InitGL();
}

OpenGL :: OpenGL(int width, int height)
{
	stencil = 0; // stencil buffer bits
	m_NearClip = 0.1f;
	m_FarClip = 2000.0f;
	m_AspectRatio = float(width) / float(height);
	InitGL();
	ReSizeGLScene(width, height);
}

OpenGL :: ~OpenGL()
{
}

bool OpenGL :: InitGL()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clears the stencil to zero
	glClearStencil(0);

	// Setup the depth buffer
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	// Polygon color should be mixed with texture color
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Set pixel packing to be "tight," that is, with a 1 byte row alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Backface Culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Set up a default colored material
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	// calculation hints for lighting
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// Set up all hardware supported extensions
	SetupExtensions();

	return true;
}

// Resize And Initialize The GL Window
GLvoid OpenGL :: ReSizeGLScene(int width, int height)
{
	ASSERT(height!=0, "ReSizeGLScene  ->  height equals zero");
	ASSERT(width!=0, "ReSizeGLScene  ->  width equals zero");

	// Calculate window aspect ratio
	m_AspectRatio = (GLfloat)width/(GLfloat)height;

	// Reset viewport
	glViewport(0, 0, width, height);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, m_AspectRatio, 0.01f, 5000.0f);

	// Reset moelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OpenGL::SetClippingPlanes(float Near, float Far)
{
	ASSERT(Near>0.0f, "OpenGL::SetClippingPlanes  ->  Near plane cannot be negative");
	ASSERT(Far>Near,  "OpenGL::SetClippingPlanes  ->  Far plane cannot be closer than the near plane");

	m_NearClip = Near;
	m_FarClip  = Far;

	glMatrixMode(GL_PROJECTION);     // Select The Projection mat4
	glLoadIdentity();                // Reset The Projection mat4
	gluPerspective(45.0f, m_AspectRatio, m_NearClip, m_FarClip);
	glMatrixMode(GL_MODELVIEW);
}

bool OpenGL::CheckExtension(const char *str)
{
	// get the extension string
	char *extstr=(char *)glGetString(GL_EXTENSIONS);

	// anything to do?
	if(!extstr)
		return false;
	if(strlen(extstr)==0)
		return false;

	// find the required extension
	return (strstr(extstr,str)!=NULL);
}

void OpenGL::SetupExtensions(void)
{
	// Display information about this system
	TRACE(string("OpenGL Vendor: ") + (const char*)glGetString(GL_VENDOR));
	TRACE(string("OpenGL Renderer: ") + (const char*)glGetString(GL_RENDERER));
	TRACE(string("OpenGL Version: ") + (const char*)glGetString(GL_VERSION));
	TRACE((const char*)glGetString(GL_EXTENSIONS));

	g_bUseMultitexture = GLEE_ARB_multitexture;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &g_MultitextureUnits);

	g_bUseFramebufferObjects = GLEE_EXT_framebuffer_object;

	g_bUseDepthTexture = GLEE_ARB_depth_texture;

	supportsShadow = g_bUseMultitexture && g_bUseDepthTexture && GLEE_ARB_shadow;

	supportsAniostropy = GLEE_EXT_texture_filter_anisotropic;
}


}; // namespace
