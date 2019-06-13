#pragma once

#include <stdint.h>

namespace Hazel {
	class System
	{
	public:
		static uint64_t PageSize();
		static uint64_t AllocationGranularity();
		static uint64_t PerformanceCounterResulution();

		static uint32_t GetMinBitPosition(uint64_t value);
		static uint64_t CountBits(uint64_t value);
		static inline uint64_t CountZeroBits(uint64_t value) { return CountBits(~value); }

		static void RoundUp(uint64_t& value, uint64_t multiple);
		template<typename T>
		static T CeilDivide(T a, T b) {
			if (a == 0)
				return 0;
			return (a - 1) / b + 1;
		}

		static uint64_t LogFloor(uint64_t value);
		//Computes the log base 2 for the value and rounds up. Ex 2->1, 4->2, 5->3, 8->3
		//We need to make powers of two produce the largest value for a given input.
		//because LogFloor(5)->2, LogFloor(4) is still 2 and with a +1 we get the right anwser
		//LogFloor(4)->2, Log2Celi(3)->1+1->2
		static inline uint64_t Log2Celi(uint64_t value) { return LogFloor(value - 1) + 1; }
	};
}
