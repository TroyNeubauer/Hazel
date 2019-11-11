#pragma once
#include "FileSystem.h"
#include "System.h"
#include "Hazel/Util/StringUtils.h"

namespace Hazel {

	template<typename F>
	void FileSystem::PathNameIterator(const char* path, F onPath)
	{
		size_t bytes = StringUtils::Capacity(path);
		char* mutablePath = System::FastNew<char>(bytes);
		memcpy(mutablePath, path, bytes);

		PathNameIterator(mutablePath, onPath);
		System::FastDelete(mutablePath);
	}

	template<typename F>
	void FileSystem::PathNameIterator(char* path, F onPath)
	{
		if (*path == 0x00)
			return;//Return for empty strings
		if (IsSlash(*path))
			path++;
		char* start = path;//Skip the first slash and have start point to the first char of this file/dir name
		char* end = path;//Begin searching for the end starting at the first character of this file/dir name
		while (*end)
		{
			StringUtils::Until(end, IsSlash);
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
