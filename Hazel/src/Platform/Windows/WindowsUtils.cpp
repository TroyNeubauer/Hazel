#include "hzpch.h"
#ifdef HZ_PLATFORM_WINDOWS

#include "WindowsUtils.h"

namespace Hazel {
	void WindowsUtils::GetLastErrorMessage(char* buf, unsigned int capacity) {
		DWORD error = GetLastError();
		DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, capacity, NULL);
		if (!result) {
			HZ_CORE_CRITICAL("Failed to get information log from error code: {0}, the error from FormatMessageA was {1}", error, GetLastError());
			if (capacity)
				buf[0] = 0x00;//Make sure to terminate the string
			else
				HZ_CORE_ASSERT(false, "Capacity is 0!");
		}
	}
}

#endif