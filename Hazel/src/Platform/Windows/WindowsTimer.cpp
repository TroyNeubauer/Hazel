#include "hzpch.h"
#ifdef HZ_PLATFORM_WINDOWS

#include "Hazel/System/Timer.h"
#include "Hazel/System/System.h"

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

	uint64_t Timer::Nanos()
	{
		return (end.QuadPart - start.QuadPart) * 1000000000ULL / System::PerformanceCounterResulution();
	}

}

#endif
