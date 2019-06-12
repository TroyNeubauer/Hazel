#pragma once

#include <Windows.h>

namespace Hazel {
	class WindowsUtils {
	public:
		static void GetLastErrorMessage(char* buf, unsigned int capacity);
	};
}
