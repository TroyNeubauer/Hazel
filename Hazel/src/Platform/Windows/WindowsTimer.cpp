#include "hzpch.h"
#ifdef HZ_PLATFORM_WINDOWS

#include "Hazel/System/Timer.h"
#include "Hazel/System/System.h"

namespace Hazel {
	Timer::Timer()
	{
		Start();
	}

	Timer& Timer::Start()
	{
		QueryPerformanceCounter(&start);
		return *this;
	}

	Timer& Timer::Stop()
	{
		QueryPerformanceCounter(&end);
		return *this;
	}

	uint64_t Timer::Nanos()
	{
		return (end.QuadPart - start.QuadPart) * 1000000000ULL / System::PerformanceCounterResulution();
	}

}

#endif
