#pragma once
namespace Hazel {
	class PlatformUtils {
	public:
		static void Init();
		//Stats for virtual memory
		static unsigned long long GetTotalMachineVirtualMemory();//The total amount of virtual memory avilable to the system in bytes
		static unsigned long long GetSystemVirtualMemoryUsage();//The amount of virtual memory being currently used on this machine in bytes
		static unsigned long long GetProcessVirtualMemoryUsage();//The amount of virtual memory used by this process in bytes

		//stats for physical memory
		static unsigned long long GetTotalMachinePhysicalMemory();//The amount of physical ram avilable to this machine in bytes
		static unsigned long long GetSystemPhysicalMemoryUsage();//The amount of physical ram used by this machine in bytes
		static unsigned long long GetProcessPhysicalMemoryUsage();//The amount of physical ram used by this process in bytes

		static float GetSystemCPUUsagePercent();//from [0,100] indicates the total CPU usage across all cores
		static float GetProcessCPUUsagePercent();//from [0,100] indicates the total CPU usage across all cores by this process

		static int GetProcessorCount();
	};
}