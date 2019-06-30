#pragma once

#include <stdint.h>
#include <spdlog/logger.h>

#include "Hazel/Log.h"


#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
	typedef LARGE_INTEGER TimeType;
#elif defined(HZ_PLATFORM_UNIX)
	#include <time.h>
	typedef timespec TimeType;
#else
	#error
#endif

namespace Hazel {
	class Timer {
	public:
		Timer();

		Timer* Start();
		Timer* Stop();
		uint64_t Nanos();
		
		double Seconds()
		{
			return Nanos() / 1000000000.0;
		}
		double Millis()
		{
			return Nanos() / 1000000.0;
		}
		uint64_t Micros()
		{
			return Nanos() / 1000;
		}
		
		inline Timer* Print(const char* message, spdlog::level::level_enum level = spdlog::level::info)
		{
			uint64_t nanos = Nanos();
			if (nanos < 1000) {
				Log::GetCoreLogger()->log(level, "{0} {1} nanoseconds",			message, nanos);
			} else if (nanos < 1000000) {
				Log::GetCoreLogger()->log(level, "{0} {1:.1f} microseconds",	message, (double) nanos / 1000.0);
			} else if (nanos < (1000ULL * 1000ULL * 1000ULL)) {
				Log::GetCoreLogger()->log(level, "{0} {1:.2f} milliseconds",	message, (double) nanos / 1000000.0);
			} else {
				Log::GetCoreLogger()->log(level, "{0} {1:.3f} seconds",			message, (double) nanos / 1000000000.0);
			}
			
			return this;
		}
		
	private:
		TimeType start, end;
	};

}
