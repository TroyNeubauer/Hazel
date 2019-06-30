#include "hzpch.h"
#ifdef HZ_PLATFORM_WINDOWS

#include "Hazel/System/System.h"
#include "Platform/Windows/WindowsUtils.h"

#include <Windows.h>
#include <intrin.h>
#include <psapi.h>
#include <TCHAR.h>
#include <pdh.h>

namespace Hazel {
	static SYSTEM_INFO info = {};
	static uint64_t timerResulution = 0;

	//total CPU usage
	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	//Process CPU time
	static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	static int numProcessors;
	static HANDLE self;

	void System::Init()
	{
		PdhOpenQuery(NULL, NULL, &cpuQuery);
		PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
		PdhCollectQueryData(cpuQuery);

		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}

	void GetMemInfo(MEMORYSTATUSEX* memInfo)
	{
		memInfo->dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(memInfo);
	}

	uint64_t System::GetTotalMachineVirtualMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
		return totalVirtualMem;
	}

	uint64_t System::GetSystemVirtualMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		return virtualMemUsed;
	}

	uint64_t System::GetProcessVirtualMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
		return virtualMemUsedByMe;
	}

	//stats for physical memory
	uint64_t System::GetTotalMachinePhysicalMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
		return totalPhysMem;
	}

	uint64_t System::GetSystemPhysicalMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		return physMemUsed;
	}

	uint64_t System::GetProcessPhysicalMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
		return physMemUsedByMe;
	}

	float System::GetSystemCPUUsagePercent() {
		PDH_FMT_COUNTERVALUE counterVal;

		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
		return (float)counterVal.doubleValue;
	}

	float System::GetProcessCPUUsagePercent() {
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));
		memcpy(&user, &fuser, sizeof(FILETIME));
		percent = (double) ((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
		percent /= (now.QuadPart - lastCPU.QuadPart);
		percent /= numProcessors;
		lastCPU = now;
		lastUserCPU = user;
		lastSysCPU = sys;

		return (float)(percent * 100.0);
	}

	uint32_t PlatformUtils::GetProcessorCount() {
		return numProcessors;
	}


	static inline void GetSystemInfo() {
		if (info.dwPageSize == 0) {
			GetSystemInfo(&info);
		}
	}

	uint64_t System::PageSize() {
		GetSystemInfo();
		return info.dwPageSize;
	}

	uint64_t System::AllocationGranularity() {
		GetSystemInfo();
		return info.dwAllocationGranularity;
	}

	uint64_t System::PerformanceCounterResulution()
	{
		if (timerResulution == 0) {
			LARGE_INTEGER result;
			if (QueryPerformanceFrequency(&result) == 0) {
				char buf[1024];
				WindowsUtils::GetLastErrorMessage(buf, sizeof(buf));
				HZ_CORE_WARN("QueryPerformanceFrequency Returned 0! Error: {0}", buf);
			}
			return timerResulution = result.QuadPart;
		} else
			return timerResulution;
	}

	uint32_t System::GetMinBitPosition(uint64_t value) {
		DWORD result;
		_BitScanForward64(&result, value);
		return result;
	}

	uint64_t System::CountBits(uint64_t value) {
		return _mm_popcnt_u64(value);
	}

	void System::RoundUp(uint64_t& value, uint64_t multiple) {
		if (multiple == 0)
			return;

		uint64_t remainder = value % multiple;
		if (remainder != 0)
			value += multiple - remainder;
	}

	uint64_t System::LogFloor(uint64_t value) {
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
