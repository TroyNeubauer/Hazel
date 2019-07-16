#pragma once

#include <Str.h>
#include <string.h>

namespace Hazel {

	//A nicer looking api for C-strings
	class StringUtils
	{
	public:
		inline static bool Equal(const char* a, const char* b) { return strcmp(a, b) == 0; }
		inline static bool Contains(const char* string, const char* part) { return strstr(string, part) != NULL; }

		template<class none = void>
		constexpr static bool ContainsAny(const char* string, const char* first)
		{
			return Contains(string, first);
		}

		template<class ... Types>
		static bool ContainsAny(const char* string, const char* first, Types... args)
		{
			return Contains(string, first) || ContainsAny(string, args...);
		}
	};

}
