#include "hzpch.h"
#include "FileTracker.h"

#ifdef HZ_DEBUG
FILE* Log_fopen(FILE* result, const char* path, const char* mode, const char* sourceFile, int line)
{
	if (result != NULL) {
		Hazel::FileTracker::GetTracker().IncrementStarted();
	} else {
		HZ_CORE_ERROR("Unable to open file: {} in mode {}, result {:p}, from source file: {}, line {}", path, mode, (void*)result, sourceFile, line);
	}
	return result;
}

int Log_fclose(FILE* file, int result, const char* sourceFile, int line)
{
	if (result != EOF && file != NULL) {
		Hazel::FileTracker::GetTracker().IncrementFinished();
	} else {
		HZ_CORE_ERROR("Unable to close file: {:p}, result: {}, from source file: {}, line {}", (void*)file, result, sourceFile, line);
	}
	return result;
}
#elif defined(HZ_RELEASE)
FILE* Log_fopen(FILE* result)
{
	if (result != NULL) {
		Hazel::FileTracker::GetTracker().IncrementStarted();
	} else {
		HZ_CORE_ERROR("Unable to open file: {}", (void*) result);
	}
	return result;
}

int Log_fclose(int result)
{
	if (result != EOF) {
		Hazel::FileTracker::GetTracker().IncrementFinished();
	} else {
		HZ_CORE_ERROR("Unable to close file: result {}", result);
	}
	return result;
}
#endif

namespace Hazel {
	static CountTracker m_FileTracker;

	unsigned long long FileTracker::GetCurrentlyOpenFilesCount() { return GetOpenedFilesCount() - GetClosedFilesCount(); }
	
	unsigned long long FileTracker::GetOpenedFilesCount()
	{
		return m_FileTracker.GetTotalStarted();
	}
	unsigned long long FileTracker::GetClosedFilesCount()
	{
		return m_FileTracker.GetTotalFinished();
	}

	unsigned long long FileTracker::GetOpenedFilesSec()
	{
		return m_FileTracker.GetStartedSec();
	}

	unsigned long long FileTracker::GetClosedFilesSec()
	{
		return m_FileTracker.GetFinishedSec();
	}

	CountTracker& FileTracker::GetTracker()
	{
		return m_FileTracker;
	}
}