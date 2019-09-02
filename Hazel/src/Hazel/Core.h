#pragma once

#include "Hazel/Log.h"
#include <stdint.h>
#include <memory>

#ifdef _GCC
	#define HZ_ALWAYS_INLINE __attribute__((always_inline))
#else
	#define HZ_ALWAYS_INLINE inline
#endif

#if (defined(HZ_DEBUG) || defined(HZ_RELEASE)) && (!defined(HZ_ENABLE_ASSERTS))
	#define HZ_ENABLE_ASSERTS
#endif


#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_ENABLE_ASSERTS
		#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed:"); HZ_ERROR(__VA_ARGS__);__debugbreak(); } }
		#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed:"); HZ_CORE_ERROR(__VA_ARGS__);__debugbreak(); } }
	#else
		#define HZ_ASSERT(x, ...)
		#define HZ_CORE_ASSERT(x, ...)
	#endif
#else
	#ifdef HZ_ENABLE_ASSERTS
		#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed:"); HZ_ERROR(__VA_ARGS__); asm("int $3"); } }
		#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed:"); HZ_CORE_ERROR(__VA_ARGS__); asm("int $3"); } }
	#else
		#define HZ_ASSERT(x, ...)
		#define HZ_CORE_ASSERT(x, ...)
	#endif
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

