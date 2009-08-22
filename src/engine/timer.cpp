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

#include <cmath>
#include <SDL/SDL.h>
#include "timer.h"

namespace NeHe {

Frame::Frame(void)
: lasttime(0),
  fps(0),
  length(0)
{
	lasttime = getTicks();
}

void Frame::Update(void)
{
	const unsigned int now = getTicks();

	length = now - lasttime;

	fps = (unsigned int)ceilf(1000.0f / length);

	lasttime = now; // save for next time
}

unsigned int Frame::getTicks(void)
{
	return (unsigned int)SDL_GetTicks();
}

} // namespace NeHe
