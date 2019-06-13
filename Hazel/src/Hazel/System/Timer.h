#pragma once

#include <stdint.h>
#include <spdlog/logger.h>


#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
	typedef LARGE_INTEGER TimeType;
#elif HZ_PLATFORM_UNIX
	#include <time.h>
	typedef timespec TimeType;
#endif

namespace Hazel {
	class Timer {
	public:
		Timer();

		Timer* Start();
		Timer* Stop();
		double Seconds();
		double Millis();
		uint64_t Micros();
		uint64_t Nanos();
		Timer* Print(const char* message, spdlog::level::level_enum level = spdlog::level::info);

		uint64_t Difference();
	private:
		TimeType start, end;
	};

}
