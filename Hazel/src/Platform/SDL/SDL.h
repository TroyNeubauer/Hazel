#pragma once


#ifdef HZ_PLATFORM_EMSCRIPTEN
	#include <SDL/SDL.h>

#elif defined(HZ_PLATFORM_WINDOWS) && defined(HZ_DEBUG)
	#define DECLSPEC __declspec(dllimport)//Tell the IDE that the definitions are in a DLL file so we dont get intellisense errors
	#include <SDL/SDL.h>

#else
	#error Only emscripten can use SDL at the moment!
#endif

