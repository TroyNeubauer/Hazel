
#include "FileEnums.h"



namespace Hazel {

	static const char* fileErrorNames[] = { "No Error", "File Not Found", "Access Denied", "Too Many Files Open", "Not Enough Memory", "Is Directory", "Invalid Parameter", "Unknown Error" };
	static const int s_ErrorNamesLength = sizeof(fileErrorNames) / sizeof(const char*);


	inline const char* FileErrorToString(FileError error)
	{
		if ((uint64_t) error >= s_ErrorNamesLength)
			return "Invalid Error! Out of bounds";
		else
			return fileErrorNames[(uint64_t) error];
	}


}
