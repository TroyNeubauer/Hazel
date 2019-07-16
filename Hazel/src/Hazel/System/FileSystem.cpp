#include "FileSystem.h"
#include <stdint.h>

bool isSlash(char c)
{
	return (c == '/' || c == '\\');
}


namespace Hazel {

const uint32_t MAX_SLASHES = 32;
	void FileSystem::NormalizePath(const char* src, char* dest, uint64_t destSize)
	{
		bool valid = true;
		uint32_t index = 0;

		uint32_t slashes[MAX_SLASHES];
		uint32_t slashCount = 0;
		while (*src)
		{
			if (isSlash(src[0]))
			{
				if (src[1] == '.' && src[2] == '.')
				{
					if (slashCount)
					{
						index = slashes[slashCount - 1];
						slashCount--;
					}
					else
					{
						dest[index++] = src[0];
						valid = false;
					}
					src += 3;
				}
				else if (src[1] == '.')
				{
					if (slashCount == 0)
					{
						dest[index++] = src[0];
						valid = false;
					}
					src += 2; // just skip \.
				}
				else
				{
					if (slashCount < MAX_SLASHES)
					{
						slashes[slashCount++] = index;
					}
					else
					{
						valid = false;
					}
					dest[index++] = *src++;
				}
			}
			else
			{
				dest[index++] = *src++;
			}
		}
		dest[index] = 0;
	}
}
