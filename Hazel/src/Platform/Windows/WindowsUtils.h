#pragma once
#ifdef HZ_PLATFORM_WINDOWS

#include <Windows.h>

namespace Hazel {
	class WindowsUtils {
	public:
		static void GetLastErrorMessage(char* buf, unsigned int capacity);
	};
}

#endif