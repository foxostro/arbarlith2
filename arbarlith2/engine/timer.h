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


#ifndef TIMER_H
#define TIMER_H

namespace NeHe {

class Timer
{
	bool	performance;		// whether the performance timer is available
	float	resolution;			// timer resolution
	unsigned long	mm_start;	// multimedia timer start
	__int64	perf_start;			// performance timer start

public:
	Timer();

	// get time in milliseconds
	float GetTime();
};

class Frame
{
	Timer	*time;		// a timer object
	float	lasttime;	// last time called
	int		fps;		// frames per second
	float	length;		// length of frame

	double  avgFps;     // Average FPS
	double  numFrames;

public:
	Frame();

	// Update frame counter *call once a frame*
	void Update();

	// Get frames per second
	int GetFPS() const { return fps; };

	// Get average frames per second
	int GetAvgFPS() const { return (int)avgFps; };

	// get the time of the last frame
	float GetLength() const { return length; };

	// Get the timer object
	Timer* GetTimer() { return time; };
};

}; // namespace NeHe

#endif
