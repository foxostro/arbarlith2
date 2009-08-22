/*
Modified in 2003-2006 by Andrew Fox
E-Mail: mailto:foxostro@gmail.com
*/

/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Timer class

	Version History:
		v1.00			New file (5/11/02)


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

#ifndef TIMER_H
#define TIMER_H

namespace NeHe {

/** Tracks time between frames and frame FPS stats */
class Frame
{
	/** Tick count of the last frame we were updated */
	unsigned int lasttime;

	/** Most recent measurement of frames per second */
	unsigned int fps;

	/** Length of the last frame */
	unsigned int length;

public:
	/** Defalt Constructor */
	Frame(void);

	/** Update frame counter. Call once per frame! */
	void Update(void);

	/** Get frames per second */
	inline unsigned int getFPS(void) const
	{
		return fps;
	}

	/** get the time of the last frame */
	inline unsigned int getLength(void) const
	{
		return length;
	}

private:
	/** Counts milliseconds since the app started */
	static unsigned int getTicks(void);
};

} // namespace NeHe

#endif
