#include "hzpch.h"
#include "FileTracker.h"

#ifdef HZ_DEBUG
FILE* Log_fopen(FILE* result, const char* path, const char* mode, const char* sourceFile, int line)
{
	auto& map = Hazel::FileTracker::GetOpenFilePaths();
	if (result != NULL) {
		Hazel::FileTracker::GetTracker().IncrementStarted();
		map[result] = path;
	} else {
		HZ_CORE_ERROR("Unable to open file: {} in mode {}, result {:p}, from source file: {}, line {}", path, mode, (void*)result, sourceFile, line);
	}
	return result;
}

int Log_fclose(FILE* file, int result, const char* sourceFile, int line)
{
	auto& map = Hazel::FileTracker::GetOpenFilePaths();
	std::string path = map[file];
	map.erase(file);
	Hazel::FileTracker::GetClosedFilePaths().insert(path);
	if (result != EOF && file != NULL) {
		Hazel::FileTracker::GetTracker().IncrementFinished();
	} else {
		HZ_CORE_ERROR("Unable to close file: {:p}, result: {}, from source file: {}, line {}", (void*)file, result, sourceFile, line);
	}
	return result;
}
#elif defined(HZ_RELEASE)
FILE* Log_fopen(FILE* result, const char* path)
{
	auto& map = Hazel::FileTracker::GetOpenFilePaths();
	if (result != NULL) {
		Hazel::FileTracker::GetTracker().IncrementStarted();
		map[result] = path;
	} else {
		HZ_CORE_ERROR("Unable to open file: {}", (void*) result);
	}
	return result;
}

int Log_fclose(FILE* file, int result)
{
	auto& map = Hazel::FileTracker::GetOpenFilePaths();
	std::string path = map[file];
	map.erase(file);
	Hazel::FileTracker::GetClosedFilePaths().insert(path);
	if (result != EOF) {
		Hazel::FileTracker::GetTracker().IncrementFinished();
	} else {
		HZ_CORE_ERROR("Unable to close file: result {}", result);
	}
	return result;
}
#endif

namespace Hazel {
	CountTracker FileTracker::m_FileTracker;
	std::map<FILE*, std::string> FileTracker::m_OpenFiles;
	std::set<std::string> FileTracker::m_ClosedFiles;

	uint64_t FileTracker::GetCurrentlyOpenFilesCount() { return GetOpenedFilesCount() - GetClosedFilesCount(); }
	
	uint64_t FileTracker::GetOpenedFilesCount()
	{
		return m_FileTracker.GetTotalStarted();
	}
	uint64_t FileTracker::GetClosedFilesCount()
	{
		return m_FileTracker.GetTotalFinished();
	}

	uint64_t FileTracker::GetOpenedFilesSec()
	{
		return m_FileTracker.GetStartedSec();
	}

	uint64_t FileTracker::GetClosedFilesSec()
	{
		return m_FileTracker.GetFinishedSec();
	}
}