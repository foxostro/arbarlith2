/*
Modified in 2003-2006 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
Modified to decouple with windowing and delete ControlData by Tom Cauchois Feb 2006
E-Mail: mailto:tcauchoi@andrew.cmu.edu
*/

/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		OpenGL Control class

	Version History:
		v1.00		Original class	(Daniel Vivas 03/08/02)
		v1.10		Rewritten for inclusion in SDK	(GA 29/10/02)
		v1.20       Modified and adapted by Andrew Fox's Graduation Project

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
// Modified by Andrew Fox


#ifndef OPENGL_H
#define OPENGL_H

#include "timer.h"		// Timing functions
#include "singleton.h"	// OpenGL will be a Singleton (we will never have more than one :)



namespace Engine {


class OpenGL : public Singleton<OpenGL>
{
public:
	float		m_NearClip;       // Near clipping plane
	float		m_FarClip;        // Far clipping plane
	float		m_AspectRatio;    // Aspect ratio of the viewport
	int			stencil;		  // stencil buffer bits

	// Helper functions
    bool			InitGL();			// Setup OpenGL
	unsigned char ComponentFromIndex(int i,unsigned int nbits,unsigned int shift); // for palette function
	void			SetPalette();		// Setup the palette in 8 bit mode

public:
	OpenGL();
	OpenGL(int width, int height);
	~OpenGL();

	int	GetStencil() const { return stencil; }
	float GetNearClip() const { return m_NearClip; }
	float GetFarClip() const { return m_FarClip; }
	void SetClippingPlanes(float Near, float Far);
	void ReSizeGLScene(int width, int height);

	// Setup all extensions supported by hardware
	void SetupExtensions(void);
	bool CheckExtension(const char *str);
};




/** Indicates support for GL_ARB_multitexture */
extern bool g_bUseMultitexture;

/** Indicates the number of available hardware multitexture units */
extern int g_MultitextureUnits;

/** Indicates support for GL_EXT_framebuffer_object */
extern bool g_bUseFramebufferObjects;

/** Indicates support for GL_ARB_depth_texture */
extern bool g_bUseDepthTexture;

/** Indicates support for shadows */
extern bool supportsShadow;

/** Indicates support for anisotropic texture filter */
extern bool supportsAniostropy;




#ifdef _DEBUG
#define FLUSH_GL_ERROR() {while(glGetError()!=GL_NO_ERROR);}
#define CHECK_GL_ERROR()                                                                                                                  \
{                                                                                                                                         \
	GLenum errorCode;                                                                                                                 \
	while( (errorCode=glGetError()) != GL_NO_ERROR)                                                                                   \
		FAIL(_tstring(_T("Line #")) + itoa(__LINE__) + _tstring(_T(": ")) + toTString((const char*)gluErrorString(errorCode))  ); \
}
#else
#define FLUSH_GL_ERROR()
#define CHECK_GL_ERROR()
#endif

} // namespace Engine

#endif

