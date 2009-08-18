/*
Modified by Andrew Fox: Uses my own ASSERT() macro
E-Mail: mailto:andrewfox@cmu.edu

Author: Richard "superpig" Fine
Source: www.gamedev.net "Enginuity, Part V"
*/

#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

#include "logstring.h"
#include "myassert.h"


namespace Engine {



template<typename T>
class Singleton
{
	static T* ms_singleton;
	protected:

		Singleton()
		{
			ms_singleton = static_cast<T*>(this);
		}

		~Singleton()
		{
			ms_singleton=0;
		}

	public:
		static inline void Create()
		{
			if(ms_singleton)return;
			new T();
		}
		static inline T& GetSingleton()
		{
			ASSERT(ms_singleton!=0, "ms_singleton was NULL");

			return(*ms_singleton);
		}
		static inline T* GetSingletonPtr()
		{
			return ms_singleton;
		}
		static inline void Destroy()
		{
			if(ms_singleton)delete(ms_singleton);
			ms_singleton=0;
		}
};

template <typename T> T* Singleton <T>::ms_singleton = 0;

} // namespace Engine

#endif
