#pragma once

#include "FileTrackerFunctions.h"

#include "Hazel/Util/CountTracker.h"
#include "Hazel/Core.h"

#include <map>
#include <set>
#include <string>

namespace Hazel {
	class FileTracker
	{
	public:
		static unsigned long long GetCurrentlyOpenFilesCount();//The number of active file handles
		static unsigned long long GetOpenedFilesCount();//Total ever opened / ever closed
		static unsigned long long GetClosedFilesCount();

		static unsigned long long GetOpenedFilesSec();//The number of files opened/closed in the last second
		static unsigned long long GetClosedFilesSec();
		static inline CountTracker& GetTracker() { return m_FileTracker; }
		static inline std::map<FILE*, std::string>& GetOpenFilePaths() { return m_OpenFiles; }
		static inline std::set<std::string>& GetClosedFilePaths() { return m_ClosedFiles; }
	private:
		static std::map<FILE*, std::string> m_OpenFiles;
		static std::set<std::string> m_ClosedFiles;
		static CountTracker m_FileTracker;
	};
}

