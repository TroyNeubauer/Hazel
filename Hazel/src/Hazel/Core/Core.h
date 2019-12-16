#pragma once

#include <stdint.h>
#include <memory>

//Common includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "Log.h"
#include "Timestep.h"
#include "HazelExterns.h"

//========== COMPILER detect ==========
#if defined(_MSC_VER)
	#define HZ_COMPILER_MSVC
#elif defined(__GNUC__)
	#define HZ_COMPILER_GCC
#elif defined(__clang__)
	#define HZ_COMPILER_CLANG
#elif defined(__EMSCRIPTEN__ )
	#define HZ_COMPILER_EMSCRIPTEN
#else
	#error Unsupported compiler
#endif

//========== PLATFORM detect ==========
#if defined(__ANDROID__)
	#define HZ_PLATFORM_ANDROID
	#define HZ_PLATFORM_UNIX
	#define HZ_PLATFORM_LINUX
	#error Android is not supported for now!

#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define HZ_PLATFORM_WINDOWS
	#define HZ_LITTLE_ENDIAN

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#define HZ_PLATFORM_APPLE
	#define HZ_PLATFORM_UNIX
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define HZ_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
	#error Apple is not supported for now!


#elif defined(__linux__)//Defined after android to target desktop linux
	#define HZ_PLATFORM_UNIX
	#define HZ_PLATFORM_LINUX
	#define HZ_LITTLE_ENDIAN
#else
	#error Unknown platform
#endif


#ifdef HZ_DEBUG
		// debug break
	#if defined(HZ_PLATFORM_WINDOWS)
		#define HZ_DEBUGBREAK() __debugbreak()
	#elif defined(HZ_PLATFORM_UNIX)
		#include <signal.h>
		#define HZ_DEBUGBREAK() raise(SIGTRAP)

	#else
		#error No debug break!
	#endif

	#define HZ_ASSERT(x, ...) { if(!(x)) {  HZ_ERROR("Assertion Failed: " #x); HZ_ERROR(__VA_ARGS__); HZ_DEBUGBREAK(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_CRITICAL("Assertion Failed: " #x); HZ_CORE_CRITICAL(__VA_ARGS__); HZ_DEBUGBREAK(); } }

#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#ifdef HZ_COMPILER_GCC
	#define HZ_ALWAYS_INLINE __attribute__((always_inline))
#else
	#define HZ_ALWAYS_INLINE inline
#endif


//Enable debug graphics contexts by default in debug mode
#if !defined(HZ_DEBUG_GRAPHICS_CONTEXT) && defined(HZ_DEBUG)
	#define HZ_DEBUG_GRAPHICS_CONTEXT 1
#endif


#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	inline Ref<T> R(T* t) { return Ref<T>(t); }

	template<typename T>
	inline Scope<T> S(T* t) { return Scope<T>(t); }
}


//Conditional defines


#ifdef HZ_COMPILER_EMSCRIPTEN
	#define HZ_USE_JS_AUDIO
#else
	#define HZ_USE_LAB_AUDIO
#endif
