#pragma once

#include <stdint.h>

namespace Hazel {

	class FileSystem
	{
	public:
		static void NormalizePath(const char* src, char* dest, uint64_t destSize);
	};

}
