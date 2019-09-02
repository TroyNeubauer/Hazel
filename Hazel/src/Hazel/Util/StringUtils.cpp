#include "hzpch.h"
#include "StringUtils.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace Hazel {
	bool StringUtils::StartsWith(const char* string, const char* target)
	{
		if (*target == 0x00) return false;
		while (*string != 0x00)
		{
			if (*string == *target)
			{
				string++;
				target++;
				if (*target == 0x00) return true;
			}
			else
				return false;
		}
	}

}
