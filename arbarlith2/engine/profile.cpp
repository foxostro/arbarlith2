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


#include "stdafx.h"
#include "misc.h"
#include "LabelWidget.h"
#include "profile.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#define strcpy strcpy_s // avoid deprecation warnings and keep portable syntax
#endif

namespace Engine {
namespace Profiler {

Engine::LabelWidget* g_ProfileText=0;

#ifdef _WIN32
float g_StartTime = -1.0f;
#else
struct timeb   g_StartTimeb;
#endif
float g_CurrentTime = -1.0f;
float g_TimeLastTick = -1.0f;


void InitTime( void )
{
#ifdef _WIN32
	g_StartTime = ((float)timeGetTime()) / 1000.0f;
#else
	ftime(&g_StartTimeb);
#endif
	g_CurrentTime = 0.0f;
	g_TimeLastTick = 0.001f;
}

void MarkTimeThisTick( void )
{
#ifdef _WIN32
	float newTime = (((float)timeGetTime()) / 1000.0f) - g_StartTime;
#else
	struct timeb tp;

	ftime(&tp);

	float newTime = tp.time - g_StartTimeb.time +
	  (tp.millitm/1000.0) - (g_StartTimeb.millitm/1000.0);

#endif

	g_TimeLastTick = newTime - g_CurrentTime;
	g_CurrentTime = newTime;

	if( g_TimeLastTick <= 0.0f ) {
		g_TimeLastTick = 0.001f;
	}

}

float GetElapsedTime( void )
{
	return( g_TimeLastTick );
}


float GetExactTime( void )
{
#ifdef _WIN32
	return( ((float)timeGetTime())/1000.0f );
#else
	struct timeb tp;

	ftime(&tp);

	return tp.time - g_StartTimeb.time +
	  (tp.millitm/1000.0) - (g_StartTimeb.millitm/1000.0);
#endif
}




struct ProfileSample {
   bool bValid;                 //Whether this data is valid
   unsigned int iProfileInstances;      //# of times ProfileBegin called
   int iOpenProfiles;           //# of times ProfileBegin w/o ProfileEnd
   char szName[256];            //Name of sample
   float fStartTime;            //The current open profile start time
   float fAccumulator;          //All samples this frame added together
   float fChildrenSampleTime;   //Time taken by all children
   unsigned int iNumParents;            //Number of profile parents
};

struct ProfileSampleHistory {
   bool bValid;        //Whether the data is valid
   char szName[256];   //Name of the sample
   float fAve;         //Average time per frame (percentage)
   float fMin;         //Minimum time per frame (percentage)
   float fMax;         //Maximum time per frame (percentage)
};



#define NUM_PROFILE_SAMPLES 50
ProfileSample g_samples[NUM_PROFILE_SAMPLES];
ProfileSampleHistory g_history[NUM_PROFILE_SAMPLES];
float g_startProfile = 0.0f;
float g_endProfile = 0.0f;


void ProfileInit( void )
{
   unsigned int i;

   for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) {
      g_samples[i].bValid = false;
      g_history[i].bValid = false;
   }

   g_startProfile = GetExactTime();

   g_ProfileText = new Engine::LabelWidget(_T("blank"), vec2(100.0f, 768.0f / 2));
   g_GUI.addWidget(g_ProfileText);
}




void ProfileBegin( char* name )
{
   unsigned int i = 0;

   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true ) {
      if( strcmp( g_samples[i].szName, name ) == 0 ) {
         //Found the sample
         g_samples[i].iOpenProfiles++;
         g_samples[i].iProfileInstances++;
         g_samples[i].fStartTime = GetExactTime();
         ASSERT( g_samples[i].iOpenProfiles == 1, _T("max 1 open at once") );
         return;
       }
       i++;	
   }

   if( i >= NUM_PROFILE_SAMPLES ) {
      FAIL(_T("Exceeded Max Available Profile Samples"));
      return;
   }

   strcpy( g_samples[i].szName, name );
   g_samples[i].bValid = true;
   g_samples[i].iOpenProfiles = 1;
   g_samples[i].iProfileInstances = 1;
   g_samples[i].fAccumulator = 0.0f;
   g_samples[i].fStartTime = GetExactTime();
   g_samples[i].fChildrenSampleTime = 0.0f;
}


void ProfileEnd( const char* name )
{
   unsigned int i = 0;
   unsigned int numParents = 0;

   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true )
   {
      if( strcmp( g_samples[i].szName, name ) == 0 )
      {  //Found the sample
         unsigned int inner = 0;
         int parent = -1;
         float fEndTime = GetExactTime();
         g_samples[i].iOpenProfiles--;

         //Count all parents and find the immediate parent
         while( g_samples[inner].bValid == true ) {
            if( g_samples[inner].iOpenProfiles > 0 )
            {  //Found a parent (any open profiles are parents)
               numParents++;
               if( parent < 0 )
               {  //Replace invalid parent (index)
                  parent = inner;
               }
               else if( g_samples[inner].fStartTime >=
                        g_samples[parent].fStartTime )
               {  //Replace with more immediate parent
                  parent = inner;
               }
            }
            inner++;
         }

         //Remember the current number of parents of the sample
         g_samples[i].iNumParents = numParents;

         if( parent >= 0 )
         {  //Record this time in fChildrenSampleTime (add it in)
            g_samples[parent].fChildrenSampleTime += fEndTime -
                                                     g_samples[i].fStartTime;
         }

         //Save sample time in accumulator
         g_samples[i].fAccumulator += fEndTime - g_samples[i].fStartTime;
         return;
      }
      i++;	
   }
}



void ProfileDumpOutputToBuffer( void )
{
   unsigned int i = 0;

   g_endProfile = GetExactTime();
   MarkTimeThisTick();

   _tstring output;
   output += _T(" Ave  :  Min  :  Max  :   #   : Profile Name\n");
   output += _T("--------------------------------------------\n");

   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true ) {		
      unsigned int indent = 0;
      float sampleTime, percentTime, aveTime, minTime, maxTime;
      _tstring indentedName;
      _tstring ave, min, max, num;
			
      if( g_samples[i].iOpenProfiles < 0 ) {
         FAIL(_T("ProfileEnd() called without a ProfileBegin()"));	
      }
      else if( g_samples[i].iOpenProfiles > 0 ) {
         FAIL(_T("ProfileBegin() called without a ProfileEnd()"));
      }

      sampleTime = g_samples[i].fAccumulator - g_samples[i].fChildrenSampleTime;
	  float deltaTime = g_endProfile - g_startProfile;
      percentTime = ( sampleTime / deltaTime ) * 100.0f;

      aveTime = minTime = maxTime = percentTime;

      //Add new measurement into the history and get the ave, min, and max
      StoreProfileInHistory( g_samples[i].szName, percentTime );
      GetProfileFromHistory( g_samples[i].szName, &aveTime, &minTime, &maxTime );

      //Format the data
	  const int fieldSize = 5;
	  ave = Engine::ftoa(aveTime);
	  min = Engine::ftoa(minTime);
	  max = Engine::ftoa(maxTime);
	  num = Engine::itoa(g_samples[i].iProfileInstances);

	  ave = Engine::fitToFieldSize(ave, fieldSize, Engine::JUSTIFY_LEFT);
      min = Engine::fitToFieldSize(min, fieldSize, Engine::JUSTIFY_LEFT);
      max = Engine::fitToFieldSize(max, fieldSize, Engine::JUSTIFY_LEFT);
      num = Engine::fitToFieldSize(num, fieldSize, Engine::JUSTIFY_CENTER);

      indentedName.clear();
      for(indent=0; indent<g_samples[i].iNumParents; indent++)
         indentedName += _T("   ");
	  indentedName += toTString(g_samples[i].szName);

	  output += ave + _T(" : ") + min + _T(" : ") + max + _T(" : ") + num + _T(" : ") + indentedName + _T("\n");
      i++;
   }

   {  //Reset samples for next frame
      unsigned int i;
      for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) {
         g_samples[i].bValid = false;
      }
      g_startProfile = GetExactTime();
   }

   if(g_ProfileText)
   {
	   _tstring outputT = toTString(output);

	   g_ProfileText->setLabel(outputT);
	   g_ProfileText->m_bVisible = g_Application.displayDebugData;
   }
}



const _tstring& ProfileGetBuffer(void)
{
	return g_ProfileText->getLabel();
}




void StoreProfileInHistory( char* name, float percent )
{
   unsigned int i = 0;
   float oldRatio;
   float newRatio = 0.8f * GetElapsedTime();
   if( newRatio > 1.0f ) {
      newRatio = 1.0f;
   }
   oldRatio = 1.0f - newRatio;

   while( i < NUM_PROFILE_SAMPLES && g_history[i].bValid == true ) {
      if( strcmp( g_history[i].szName, name ) == 0 )
      {  //Found the sample
         g_history[i].fAve = (g_history[i].fAve*oldRatio) + (percent*newRatio);
         if( percent < g_history[i].fMin ) {
            g_history[i].fMin = percent;
         }
         else {
            g_history[i].fMin = (g_history[i].fMin*oldRatio) + (percent*newRatio);
         }

         if( g_history[i].fMin < 0.0f ) {
            g_history[i].fMin = 0.0f;
		 }


         if( percent > g_history[i].fMax ) {
            g_history[i].fMax = percent;
         }
         else {
            g_history[i].fMax = (g_history[i].fMax*oldRatio) + (percent*newRatio);
         }
         return;
      }
      i++;
   }

   if( i < NUM_PROFILE_SAMPLES )
   {  //Add to history
      strcpy( g_history[i].szName, name );
      g_history[i].bValid = true;
      g_history[i].fAve = g_history[i].fMin = g_history[i].fMax = percent;
   }
   else {
      FAIL(_T("Exceeded Max Available Profile Samples!"));
   }
}

void GetProfileFromHistory( char* name, float* ave, float* min, float* max )
{
   unsigned int i = 0;
   while( i < NUM_PROFILE_SAMPLES && g_history[i].bValid == true ) {
      if( strcmp( g_history[i].szName, name ) == 0 )
	  {  //Found the sample
         *ave = g_history[i].fAve;
         *min = g_history[i].fMin;
         *max = g_history[i].fMax;
         return;
	  }
      i++;
   }	
   *ave = *min = *max = 0.0f;
}

}; //namespace Profiler
}; // namespace Engine
