/*
Modified in 2003-2006 by Andrew Fox
E-Mail: mailto:andrewfox@cmu.edu
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

#include "stdafx.h" // Master Header
#include "timer.h"
#include "profile.h"

#ifdef _WIN32
#	include <windows.h>   // Master include file for Windows applications.
#	include <mmsystem.h>  // timeGetTime
#else
#	pragma error("timer.cpp has not yet been ported to this operating system.")
#endif

namespace NeHe {

//
//	Timer class
//

Timer :: Timer()
{
#ifdef _WIN32
	__int64 frequency;	// The performance counter frequency

	// check for performance counter
	if(QueryPerformanceFrequency((LARGE_INTEGER *) &frequency))
	{
		// Use the performance counter to get the present time
		QueryPerformanceCounter((LARGE_INTEGER *) &perf_start);
		// say this computer has a performance counter
		performance=true;
		// calculate the resolution from the frequency
		resolution=(float) ( ((double)1.0f)/((double)frequency) );
	}else{
		// No performance counter available
		performance=false;
		// Get the present time
		mm_start=timeGetTime();
		// calculate the resolution
		resolution=1.0f/1000.0f;
	}
#else
	performance=false;       // No performance counter available
	mm_start=0.0f;           // Get the present time
	resolution=1.0f/1000.0f; // calculate the resolution
#endif
}

float Timer :: GetTime()
{
#ifdef _WIN32
	// get the time from the available counter
	if(performance)
	{
		__int64 time;			// the current time

		// get the time
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		// calculate it in milliseconds
		return ( (float)(time-perf_start)*resolution )*1000.0f;
	}else
		return ( (float)(timeGetTime()-mm_start)*resolution )*1000.0f;
#else
	return 0.0f;
#endif
}

//
//	Frame class
//

Frame :: Frame()
{
	// allocate a new timer
	time = new Timer;

	// get the present time
	lasttime=time->GetTime();

	// clear everything else
	fps=0;
	length=0.0f;

	numFrames=0.0;
	avgFps=0.0;
}

void Frame :: Update()
{
	// get the the present time
	float now=time->GetTime();

	// calculate the length of the last frame
	length=now-lasttime;
	// make sure it's sensible
	if(length<=0.0f)
		length=1.0f;

	// and the frames per second
	fps=(int)(1000.0f/length);

	// and save the last time
	lasttime=now;

	avgFps = (avgFps*numFrames + fps) / (numFrames+1.0);
	numFrames += 1.0;
}

} // namespace NeHe
