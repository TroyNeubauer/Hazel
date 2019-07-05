#pragma once
#include <stdio.h>
#ifdef HZ_DEBUG
	extern FILE* Log_fopen(FILE* result, const char* path, const char* mode, const char* sourceFile, int line);
	extern int Log_fclose(FILE* file, int result, const char* sourceFile, int line);
#elif defined(HZ_RELEASE)
	extern FILE* Log_fopen(FILE* result, const char* path);
	extern int Log_fclose(FILE* file, int result);
#endif