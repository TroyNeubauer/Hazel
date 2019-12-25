#pragma once

#include "Hazel/Core/Core.h"

#ifdef HZ_PLATFORM_EMSCRIPTEN
	#include <emscripten.h>
	#include <SDL.h>
	#include <SDL_opengles2.h>

#elif defined(HZ_PLATFORM_WINDOWS) && defined(HZ_DEBUG)
	#define DECLSPEC __declspec(dllimport)//Tell the IDE that the definitions are in a DLL file so we dont get intellisense errors
	#include <emscripten.h>
	#include <SDL.h>
	#include <SDL_opengles2.h>

#else
	#error Only emscripten can use SDL at the moment!
#endif

