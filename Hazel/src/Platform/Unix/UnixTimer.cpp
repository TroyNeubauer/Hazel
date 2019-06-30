#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX

#include "Hazel/System/Timer.h"
#include "Hazel/System/System.h"

namespace Hazel {
	Timer::Timer()
	{
		Start();
	}

	Timer* Timer::Start()
	{
		clock_gettime(CLOCK_REALTIME, &start);
		return this;
	}

	Timer* Timer::Stop()
	{
		clock_gettime(CLOCK_REALTIME, &end);
		return this;
	}

	uint64_t Timer::Nanos()
	{
		uint64_t seconds = end.tv_sec - start.tv_sec;
		return 1000000000ULL * seconds + end.tv_nsec - start.tv_nsec;
	}

}

#endif
