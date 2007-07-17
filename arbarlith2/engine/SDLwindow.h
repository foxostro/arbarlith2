/* Written by Tom Cauchois, February 2006, for Parasomnia: Decadence of Darkness
   Email: mailto:tcauchoi@andrew.cmu.edu
*/

#ifndef SDL_WINDOW
#define SDL_WINDOW

#include <SDL/SDL.h>

 //haven't figured out how to do multi-window with SDL yet
#include "singleton.h"

#include "myassert.h"

#include "Application.h"

namespace Engine {

class SDLInput
{
public:
	SDLInput();

	//Process the message queue
	void Pump();

	//What about a resize?
	bool ResizePending() { bool rp = resizePending; resizePending = false; return rp; }
	//Width/Height parameters corresponding to the most recent resize event
	unsigned int ResizeWidth() const { return resizeW; }
	unsigned int ResizeHeight() const { return resizeH; }

	//Mouse/keystate
	int MouseX, MouseY, MouseWheelDelta;
	bool MouseLeft, MouseMiddle, MouseRight, Keys[SDLK_LAST], Quit;

	int LastMouseX, LastMouseY;
	int DeltaMouseX, DeltaMouseY;

private:
	bool resizePending;
	unsigned int resizeW, resizeH;
};

class SDLWindow : public Singleton<SDLWindow>
{
public:
	//Backbuffer color formats
	enum ColorFormat
	{
		R8G8B8A8, //32-bit, Truecolor with an Alpha channel

		NUM_COLORFORMATS
	};

	/**
	handle linkage of SDL in constructor/destructor
	@param app Application that spawned the window
	*/
	SDLWindow(Application &app);

	~SDLWindow();

	/**
	Create a window with the given properties
	*/
	void Create
	(
		const _tstring &title,
		unsigned int width,
		unsigned int height,
		const ColorFormat &format,
		unsigned int zdepth,
		bool fullscreen
	);

	//destroy the currently active window
	void Kill();

	//Swap the backbuffer and the frontbuffer
	void Flip();

	//mutators for certain window properties; otherwise, just create a new window
	void SetFullscreen(bool fullscreen);
	void SetTitle(const _tstring &title);
	void Resize(int width, int height);

	//accessors for window properties; behavior is undefined/stupid if no window exists
	unsigned int GetWidth() const { return width; }
	unsigned int GetHeight() const { return height; }
	ColorFormat GetColorFormat() const { return format; }
	unsigned int GetColorDepth() const;
	unsigned int GetZDepth() const { return zdepth; }
	bool GetFullscreen() const { return fullscreen; }
	const _tstring& GetTitle() const { return title; }

	//input state and message loop corresponding to this window
	SDLInput input;

private:
	Application &application;
	SDL_Surface *windowSurface;
	unsigned int width, height, zdepth;
	bool fullscreen;
	ColorFormat format;
	_tstring title;
};


} // namespace Engine



#endif /* SDL_WINDOW */
