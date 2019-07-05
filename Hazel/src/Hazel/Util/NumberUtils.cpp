#include "hzpch.h"
#include "NumberUtils.h"

namespace Hazel {
#ifdef HZ_PLATFORM_WINDOWS
	#include <intrin.h>
	#include <Windows.h>
#else
	#error Need includes for other platforms
#endif

	uint32_t NumberUtils::GetMinBitPosition(uint64_t value) {
		DWORD result;
		BitScanForward64(&result, value);//TODO provide other implementation for non Windows platforms
		return result;
	}

	uint64_t NumberUtils::CountBits(uint64_t value) {
		return _mm_popcnt_u64(value);//TODO other implementation for non X86
	}

	uint64_t NumberUtils::RoundUp(uint64_t value, uint64_t multiple) {
		if (multiple == 0)
			return value;

		uint64_t remainder = value % multiple;
		if (remainder == 0)
			return value;

		return value + multiple - remainder;
	}
}

