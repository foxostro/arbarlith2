/* Written by Tom Cauchois, February 2006, for Parasomnia: Decadence of Darkness

   Andrew Fox, July 2006: SetFullscreen sets a flag to activate the setting on restart.
   Andrew Fox, August 2006: Release and reaquire assets so that changing resolutions does not corrupt video

   Email: mailto:tcauchoi@andrew.cmu.edu,
          mailto:foxostro@gmail.com
*/

#include "stdafx.h"
#include "Application.h"
#include "SDLwindow.h"


namespace Engine {

//Color-channel depths for the given color formats
//Color formats are:
//R8G8B8A8 (32-bit)
unsigned int redBits[SDLWindow::NUM_COLORFORMATS] = { 8 };
unsigned int greenBits[SDLWindow::NUM_COLORFORMATS] = { 8 };
unsigned int blueBits[SDLWindow::NUM_COLORFORMATS] = { 8 };
unsigned int alphaBits[SDLWindow::NUM_COLORFORMATS] = { 8 };
unsigned int depth[SDLWindow::NUM_COLORFORMATS] = { 32 };

SDLWindow::SDLWindow(Application &app)
: application(app),
  windowSurface(0),
  width(800),
  height(600),
  zdepth(16),
  fullscreen(false),
  format(R8G8B8A8),
  title("SDL Window")
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);
}

SDLWindow::~SDLWindow()
{
	SDL_Quit();
}

unsigned int SDLWindow::GetColorDepth() const
{
	return depth[format];
}

void SDLWindow::Create
(
	const string &title,
	unsigned int width,
	unsigned int height,
	const ColorFormat &format,
	unsigned int zdepth,
	bool fullscreen
)
{
	//kill any pre-existing window
	Kill();

	this->title = title;
	this->width = width;
	this->height = height;
	this->format = format;
	this->zdepth = zdepth;
	this->fullscreen = fullscreen;

	//set the GL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, zdepth);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,redBits[format]);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,greenBits[format]);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,blueBits[format]);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,alphaBits[format]);

	SDL_WM_SetCaption(title.c_str(), title.c_str());

	//note: uncomment SDL_RESIZEABLE if you want to be able to resize the window,
	//although that might hurt performance
	windowSurface = SDL_SetVideoMode(width,height,depth[format],
		SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF |
		(fullscreen?SDL_FULLSCREEN:0) /* | SDL_RESIZEABLE */);
	ASSERT(windowSurface != NULL, "Couldn't create window!"); //SDL_GetError() for more info

	//check that the GL attributes were correctly set
	{
		int attrib;

		SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER,	&attrib);
		SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE,		&attrib);
		SDL_GL_GetAttribute(SDL_GL_RED_SIZE,		&attrib);
		SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE,		&attrib);
		SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE,		&attrib);
		SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE,		&attrib);
	}
}

void SDLWindow::Kill()
{
	SDL_FreeSurface(windowSurface);
}

void SDLWindow::Flip()
{
	SDL_GL_SwapBuffers();
}

void SDLWindow::SetFullscreen(bool fullscreen)
{
	if(this->fullscreen != fullscreen)
	{
		this->fullscreen = fullscreen;

		application.release();

		Create(title,width,height,format,zdepth,fullscreen);

		OpenGL::GetSingleton().InitGL();
		OpenGL::GetSingleton().ReSizeGLScene(width,height);

		application.reaquire();
	}
}

void SDLWindow::Resize(const int width, const int height)
{
	if(this->width != (unsigned int)width || this->height != (unsigned int)height)
	{
		this->width = width;
		this->height = height;

		application.release();

		Create(title,width,height,format,zdepth,fullscreen);

		OpenGL::GetSingleton().InitGL();
		OpenGL::GetSingleton().ReSizeGLScene(width,height);

		application.reaquire();
	}
}

void SDLWindow::SetTitle(const string &title)
{
	this->title = title;
	SDL_WM_SetCaption(title.c_str(), 0);
}

SDLInput::SDLInput()
{
	MouseX = MouseY = MouseWheelDelta = 0;
	MouseLeft = MouseMiddle = MouseRight = resizePending = Quit = false;
	for(size_t s = 0; s < SDLK_LAST; ++s)
		Keys[s] = false;

	LastMouseX = 0;
	LastMouseY = 0;
	DeltaMouseX = 0;
	DeltaMouseY = 0;
}

void SDLInput::Pump()
{
	SDL_Event event; event.type = SDL_NOEVENT;

	DeltaMouseX = 0;
	DeltaMouseY = 0;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			Keys[event.key.keysym.sym] = event.key.state == SDL_PRESSED;
			break;
		case SDL_MOUSEMOTION:
			MouseX = event.motion.x;
			MouseY = event.motion.y;
			DeltaMouseX = event.motion.xrel;
			DeltaMouseY = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			switch(event.button.button)
			{
			case SDL_BUTTON_LEFT:
				MouseLeft = event.button.state == SDL_PRESSED;
				break;
			case SDL_BUTTON_MIDDLE:
				MouseMiddle = event.button.state == SDL_PRESSED;
				break;
			case SDL_BUTTON_RIGHT:
				MouseRight = event.button.state == SDL_PRESSED;
				break;
			case SDL_BUTTON_WHEELUP:
				MouseWheelDelta += event.button.state == SDL_PRESSED;
				break;
			case SDL_BUTTON_WHEELDOWN:
				MouseWheelDelta -= event.button.state == SDL_PRESSED;
				break;
			}
			break;
		case SDL_VIDEORESIZE:
			resizePending = true;
			resizeW = event.resize.w;
			resizeH = event.resize.h;
			break;
		case SDL_QUIT:
		Quit = true;
			break;
		}
	}

	// Save last mouse positions
	LastMouseX = MouseX;
	LastMouseY = MouseY;
}

} // namespace Engine
