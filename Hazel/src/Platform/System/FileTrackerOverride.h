#pragma once

#include <stdio.h>

#include "FileTrackerFunctions.h"

#ifdef HZ_DEBUG
	#define _wfopen(path, access) Log_fopen(_wfopen(path, access), path, access, __FILE__, __LINE__)
	//#define _wfopen_s(stream, path, access) _wfopen_s(stream, path, access); Log_fopen(*stream, __FILE__, __LINE__)
	//#define fopen_s(stream, path, access) fopen_s(stream, path, access); Log_fopen(*stream, __FILE__, __LINE__)
	#define fopen(path, access) Log_fopen(fopen(path, access), path, access, __FILE__, __LINE__)
	#define fclose(file) Log_fclose(file, fclose(file), __FILE__, __LINE__)
#elif defined(HZ_RELEASE)
	#define _wfopen(path, access) Log_fopen(_wfopen(path, access), path)
	//#define _wfopen_s(stream, path, access) _wfopen_s(stream, path, access); Log_fopen(*stream, "", "")
	//#define fopen_s(stream, path, access) fopen_s(stream, path, access); Log_fopen(*stream, "", "")
	#define fopen(path, access) Log_fopen(fopen(path, access), path)
	#define fclose(file) Log_fclose(file, fclose(file))
#else
	//Dont track file open/closes
#endif
