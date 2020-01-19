#pragma once

#include <stdio.h>

#include "FileTrackerFunctions.h"

#ifdef HZ_DEBUG
	#define fopen(p_Path, p_Access) Log_fopen(fopen(p_Path, p_Access), p_Path, p_Access, __FILE__, __LINE__)
	#define fclose(file) Log_fclose(file, fclose(file), __FILE__, __LINE__)
#elif defined(HZ_RELEASE)
	#define fopen(p_Path, p_Access) Log_fopen(fopen(p_Path, p_Access), p_Path)
	#define fclose(file) Log_fclose(file, fclose(file))
#else
	//Dont track file open/closes
#endif
