/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"
 * 
 * Modified by Andrew Fox in 2007
 */

#ifndef _PROFILE_H
#define _PROFILE_H

//#define ENABLE_PROFILER

#include "tstring.h"

namespace Engine {
namespace Profiler {

void ProfileInit( void );
void ProfileBegin( char* name );
void ProfileEnd( const char* name );
void ProfileDumpOutputToBuffer( void );
const _tstring& ProfileGetBuffer( void );
void StoreProfileInHistory( char* name, float percent );
void GetProfileFromHistory( char* name, float* ave, float* min, float* max );

#ifdef ENABLE_PROFILER
class Profile
{
public:
	string name;

	Profile(char *name)
	{
		this->name = name;
		ProfileBegin(name);
	}

	~Profile()
	{
		ProfileEnd(name.c_str());
	}
};
#endif

}; // namespace Profiler
}; // namespace Engine

#ifdef ENABLE_PROFILER
#define PROFILE Engine::Profiler::Profile ____profile____ (__FUNCTION__);
#else
#define PROFILE // compile it out
#endif

#endif
