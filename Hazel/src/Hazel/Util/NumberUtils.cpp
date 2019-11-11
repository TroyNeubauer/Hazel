#include "hzpch.h"
#include "NumberUtils.h"

namespace Hazel {
#ifdef HZ_PLATFORM_WINDOWS
	#include <intrin.h>
	#include <Windows.h>
#else

#endif

	uint32_t NumberUtils::GetMinBitPosition(uint64_t value) {
#ifdef HZ_PLATFORM_WINDOWS
		DWORD result;
		BitScanForward64(&result, value);//TODO provide other implementation for non Windows platforms
		return result;
#else
		for (int i = 0; i < 8 * sizeof(uint64_t); i++)
			if ((value >> i) & 0x01)
				return i;
		return -1;
#endif
	}

	uint64_t NumberUtils::CountBits(uint64_t value) {
#ifdef HZ_PLATFORM_WINDOWS
		return _mm_popcnt_u64(value);//TODO other implementation for non X86
#else
		int count = 0;
		for (int i = 0; i < 8 * sizeof(uint64_t); i++)
			if ((value >> i) & 0x01)
				count++;
		return count;
#endif
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

