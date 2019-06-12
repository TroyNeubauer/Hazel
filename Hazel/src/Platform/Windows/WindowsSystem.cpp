#include "hzpch.h"
#include "Hazel/System/System.h"

#include <Windows.h>
#include <intrin.h>

#ifdef HZ_PLATFORM_WINDOWS
namespace Hazel {
	static SYSTEM_INFO info = {};

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
