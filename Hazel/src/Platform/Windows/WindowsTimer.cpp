#include "hzpch.h"
#include "Hazel/System/Timer.h"
#include "Hazel/System/System.h"

#ifdef HZ_PLATFORM_WINDOWS
namespace Hazel {
	Timer::Timer()
	{
		Start();
	}

	Timer* Timer::Start()
	{
		if (QueryPerformanceCounter(&start) == 0) {
			HZ_CORE_ASSERT(false, "QueryPerformanceCounter failed. Error code {0}", GetLastError());
		}
		return this;
	}

	Timer* Timer::Stop()
	{
		if (QueryPerformanceCounter(&end) == 0) {
			HZ_CORE_ASSERT(false, "QueryPerformanceCounter failed. Error code {0}", GetLastError());
		}
		return this;
	}

	double Timer::Seconds()
	{
		return (double) Difference() / (double) System::PerformanceCounterResulution();
	}

	double Timer::Millis()
	{
		return (double) Difference() * 1000.0 / (double) System::PerformanceCounterResulution();
	}

	uint64_t Timer::Micros()
	{
		return Difference() * 1000000ULL / System::PerformanceCounterResulution();
	}

	uint64_t Timer::Nanos()
	{
		return Difference() * 1000000000ULL / System::PerformanceCounterResulution();
	}

	Timer* Timer::Print(const char* message, spdlog::level::level_enum level)
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

	uint64_t Timer::Difference()
	{
		return end.QuadPart - start.QuadPart;
	}

}

#endif
