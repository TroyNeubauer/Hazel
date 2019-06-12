#pragma once
#include <stdio.h>
#ifdef HZ_DEBUG
	__declspec(dllexport) extern FILE* Log_fopen(FILE* result, const char* path, const char* mode, const char* sourceFile, int line);
	__declspec(dllexport) extern int Log_fclose(FILE* file, int result, const char* sourceFile, int line);
#elif defined(HZ_RELEASE)
	__declspec(dllexport) extern FILE* Log_fopen(FILE* result);
	__declspec(dllexport) extern int Log_fclose(int result);
#endif