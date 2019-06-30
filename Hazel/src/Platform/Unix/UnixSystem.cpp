#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX

#include "Hazel/System/System.h"
#include "Hazel/Log.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <stdio.h>
#include <string.h>


namespace Hazel {
	
	struct Pstat {
		uint32_t utime_ticks;
		int32_t cutime_ticks;
		uint32_t stime_ticks;
		int32_t cstime_ticks;
		uint32_t vsize; // virtual memory size in bytes
		uint32_t rss; //Resident Set  Size in bytes
		uint32_t cpu_total_time;
	};
	
	static Pstat lastStat, nowStat;
	static pid_t pid;
	
	/*
	 * reads /proc data into the passed struct Pstat
	 * returns 0 on success, -1 on error
	*/
	/*static int get_usage(Pstat* result) {
		//convert  pid to string
		char stat_filepath[64];
		snprintf(stat_filepath, sizeof(stat_filepath), "/proc/%d/stat", pid);

		FILE* fpstat = fopen(stat_filepath, "r");
		if (fpstat == NULL) {
			HZ_CORE_WARN("[UnixSystem] Unable to open file {}", stat_filepath);
			return -1;
		}

		FILE* fstat = fopen("/proc/stat", "r");
		if (fstat == NULL) {
			HZ_CORE_WARN("[UnixSystem] Unable to open file /proc/stat");
			fclose(fstat);
			return -1;
		}

		//read values from /proc/pid/stat
		bzero(result, sizeof(Pstat));
		int32_t rss;
		if (fscanf(fpstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld %*d %*d %*d %*d %*u %lu %ld",
					&result->utime_ticks, &result->stime_ticks, &result->cutime_ticks, &result->cstime_ticks, &result->vsize, &rss) == EOF)
		{
			fclose(fpstat);
			return -1;
		}
		fclose(fpstat);
		result->rss = rss * System::PageSize();

		//read+calc cpu total time from /proc/stat
		uint32_t cpu_time[10];
		bzero(cpu_time, sizeof(cpu_time));
		if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
					&cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
					&cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
					&cpu_time[8], &cpu_time[9]) == EOF)
		{
			fclose(fstat);
			return -1;
		}

		fclose(fstat);

		for (int i = 0; i < 10; i++)
			result->cpu_total_time += cpu_time[i];

		return 0;
	}*/

	void System::Init()
	{
		pid = getpid();
		//get_usage(&lastStat);
	}

	uint64_t System::GetTotalMachineVirtualMemory() {
		return 0;
	}

	uint64_t System::GetSystemVirtualMemoryUsage() {
		return 0;
	}

	uint64_t System::GetProcessVirtualMemoryUsage() {
		return 0;
	}

	//stats for physical memory
	uint64_t System::GetTotalMachinePhysicalMemory() {
		return 0;
	}

	uint64_t System::GetSystemPhysicalMemoryUsage() {
		return 0;
	}

	uint64_t System::GetProcessPhysicalMemoryUsage() {
		return 0;
	}

	float System::GetSystemCPUUsagePercent() {
		//const uint32_t total_time_diff = nowStat.cpu_total_time - lastStat.cpu_total_time;
		//return 100.0 * ((((nowStat.stime_ticks + nowStat.cstime_ticks) - (lastStat.stime_ticks + lastStat.cstime_ticks))) / (double) total_time_diff);
		return 0.0f;
	}

	float System::GetProcessCPUUsagePercent() {
		//get_usage(&nowStat);
		
		//const uint32_t total_time_diff = nowStat.cpu_total_time - lastStat.cpu_total_time;
		
		//return 100.0 * (((nowStat.utime_ticks + nowStat.cutime_ticks) - (lastStat.utime_ticks + lastStat.cutime_ticks)) / (double) total_time_diff);
		//memcpy(&lastStat, &nowStat, sizeof(nowStat));
		return 0.0f;
	}

	uint32_t System::GetProcessorCount() {
		return sysconf(_SC_NPROCESSORS_ONLN);
	}

	uint64_t System::PageSize() {
		return getpagesize();
	}

	uint64_t System::AllocationGranularity() {
		return 0;
	}

	uint64_t System::PerformanceCounterResulution()
	{
		return CLOCKS_PER_SEC;
	}

	uint32_t System::GetMinBitPosition(uint64_t value)
	{
		return __builtin_ctzll(value);
	}

	uint64_t System::CountBits(uint64_t value)
	{
		return __builtin_popcountll(value);
	}

	void System::RoundUp(uint64_t& value, uint64_t multiple)
	{
		if (multiple == 0)
			return;

		uint64_t remainder = value % multiple;
		if (remainder != 0)
			value += multiple - remainder;
	}

	uint64_t System::LogFloor(uint64_t value)
	{
		const static uint64_t tab64[64] = {
		63,  0, 58,  1, 59, 47, 53,  2,
		60, 39, 48, 27, 54, 33, 42,  3,
		61, 51, 37, 40, 49, 18, 28, 20,
		55, 30, 34, 11, 43, 14, 22,  4,
		62, 57, 46, 52, 38, 26, 32, 41,
		50, 36, 17, 19, 29, 10, 13, 21,
		56, 45, 25, 31, 35, 16,  9, 12,
		44, 24, 15,  8, 23,  7,  6,  5 };

		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value |= value >> 32;
		return tab64[((uint64_t)((value - (value >> 1ULL)) * 0x07EDD5E59A4E28C2)) >> 58ULL];
	}

}

#endif
