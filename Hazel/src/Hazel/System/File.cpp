#include "hzpch.h"
#include "File.h"

static const char* fileErrorNames[] = { "No Error", "File Not Found", "Access Denied", "Too Many Files Open", "Unknown Error" };

const int s_ErrorNamesLength = sizeof(fileErrorNames) / sizeof(const char*);

const char * Hazel::FileErrorToString(FileError error)
{
	if (error >= s_ErrorNamesLength)
		return "Invalid Error! Out of bounds";
	else
		return fileErrorNames[error];
}
