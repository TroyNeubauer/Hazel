#pragma once
#include "FileSystem.h"
#include "System.h"
#include "Hazel/Util/StringUtils.h"

namespace Hazel {
	//Advances the pointer until end returns true for the current character.
	template<typename T>//Template to allow for both const char* and char*
	HZ_ALWAYS_INLINE void Until(T*& string, std::function<bool(char)> end)
	{
		while (*string && !end(*string))
			string++;
	}

	HZ_ALWAYS_INLINE void Until(const char*& string, char target)
	{
		Until(string, [target](char current) { return current == target; });
	}

	void FileSystem::PathNameIterator(const char* path, std::function<bool(char*, char*)> onPath)
	{
		size_t bytes = StringUtils::Capacity(path);
		char* mutablePath = System::FastNew<char>(bytes);
		memcpy(mutablePath, path, bytes);

		PathNameIterator(mutablePath, onPath);
		System::FastDelete(mutablePath);
	}

	inline bool FileSystem::PathsEqual(const char* a, const char* b)
	{
		char fullA[512], fullB[512];
		GetFullPathNameA(a, sizeof(fullA), fullA, nullptr);
		GetFullPathNameA(b, sizeof(fullB), fullB, nullptr);
		return StringUtils::Equal(fullA, fullB);
	}

	void FileSystem::PathNameIterator(char* path, std::function<bool(char*, char*)> onPath)
	{
		Until(path, IsSlash);
		if (*path == 0x00)
			return;//Return for empty strings
		char* start = path + 1;//Skip the first slash and have start point to the first char of this file/dir name
		char* end = path + 1;//Begin searching for the end starting at the first character of this file/dir name
		while (*end)
		{
			Until(end, IsSlash);//Find the ending slash of this file/dir name
			if (*end == 0x00) break;
			if (start != end)//We found somthing
			{
				char origional = *end;//Put in a temporary null byte so they we dont have to copy it
				*end = 0x00;
				if (onPath(start, origional ? (end + 1) : end)) {
					*end = origional;
					return;
				}
				*end = origional;
			}
			start = ++end;//Have the next start be the char after this trailing slash
		}
		if (start != end)
			onPath(start, end);
	}

}
