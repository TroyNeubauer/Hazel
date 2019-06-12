
#include "hzpch.h"
#include "PlatformUtils.h"

#include <Windows.h>
#include <psapi.h>
#include <TCHAR.h>
#include <pdh.h>

namespace Hazel {

	//total CPU usage
	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	//Process CPU time
	static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	static int numProcessors;
	static HANDLE self;

	void PlatformUtils::Init()
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

	unsigned long long PlatformUtils::GetTotalMachineVirtualMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
		return totalVirtualMem;
	}

	unsigned long long PlatformUtils::GetSystemVirtualMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		return virtualMemUsed;
	}

	unsigned long long PlatformUtils::GetProcessVirtualMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
		return virtualMemUsedByMe;
	}

	//stats for physical memory
	unsigned long long PlatformUtils::GetTotalMachinePhysicalMemory() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
		return totalPhysMem;
	}

	unsigned long long PlatformUtils::GetSystemPhysicalMemoryUsage() {
		MEMORYSTATUSEX memInfo;
		GetMemInfo(&memInfo);

		DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		return physMemUsed;
	}

	unsigned long long PlatformUtils::GetProcessPhysicalMemoryUsage() {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

		SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
		return physMemUsedByMe;
	}

	float PlatformUtils::GetSystemCPUUsagePercent() {
		PDH_FMT_COUNTERVALUE counterVal;

		PdhCollectQueryData(cpuQuery);
		PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
		return (float)counterVal.doubleValue;
	}

	float PlatformUtils::GetProcessCPUUsagePercent() {
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

	int PlatformUtils::GetProcessorCount() {
		return numProcessors;
	}

}
