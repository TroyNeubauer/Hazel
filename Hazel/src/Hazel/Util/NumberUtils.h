#pragma once

#include <stdint.h>

namespace Hazel {
	
	class NumberUtils
	{
	public:
		static uint32_t GetMinBitPosition(uint64_t value);
		static uint64_t CountBits(uint64_t value);
		static uint64_t RoundUp(uint64_t value, uint64_t multiple);
	};

}

