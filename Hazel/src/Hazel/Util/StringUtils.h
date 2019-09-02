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

		inline static size_t Length(const char* string) { return strlen(string); }
		inline static size_t Capacity(const char* string) { return Length(string) + 1; }

		inline static void Copy(char* dest, const char* source) { strcpy(dest, source); }

		static void LTrim(std::string& s);// trim from start
		static void RTrim(std::string& s);// trim from end
		static void Trim(std::string& s);// trim from both ends

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

		//Advances the pointer until end returns true for the current character.
		template<typename T, typename F>//Template to allow for both const char* and char*
		inline static void Until(T*& string, F end)
		{
			while (*string && !end(*string))
				string++;
		}
		template<typename T>
		inline static void FirstOf(T*& string, char target) { Until(string, [target](char current) { return current == target; }); }
		template<typename T>
		inline static void FirstOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current == a || current == b; }); }

		template<typename T>
		inline static void FirstNotOf(T*& string, char target) { Until(string, [target](char current) { return current != target; }); }
		template<typename T>
		inline static void FirstNotOf(T*& string, char a, char b) { Until(string, [a, b](char current) { return current != a || current != b; }); }


		//Advances string to the null termination character or to the n+1th character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> " fun"
		template<typename T>
		static void Find(T*& string, const char* target)
		{
			const char* targetStart = target;
			while (*string && *target)
			{
				if (*string == *target) target++;//Advance. If we reach the end of the string target[0] will be null and we will return
				else target = targetStart;
				string++;
			}
		}

		//Advances string to the null termination character or to the first character of the matched target
		//string will be modified to be an empty string if the target is not found
		//Find("Fast string finding is fun", "is") -> "is fun"
		template<typename T>
		static void FindBegin(T*& string, const char* target)
		{
			if (*string == 0x00 || *target == 0x00) return;
			const char* targetStart = target;
			const char* start;//Guaranteed to be assigned the first time a character is matched
			while (*string)
			{
				if (*string == *target) {//A character matches
					if (target == targetStart)//We are starting the beginning of a new match so save the place in the string
						start = string;
					target++;
					if (*target == 0x00)//We matched the entire target
					{
						string = start;//Reset string to point to the start of the target
						return;
					}
				}
				else
				{
					target = targetStart;//Bring back target since we lost the old one
				}
				string++;
			}
		}

	};
}
