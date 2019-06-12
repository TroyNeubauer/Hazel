#pragma once

#include "FileTrackerFunctions.h"

#include "Hazel/Util/CountTracker.h"
#include "Hazel/Core.h"


namespace Hazel {
	class FileTracker
	{
	public:
		static unsigned long long GetCurrentlyOpenFilesCount();//The number of active file handles
		static unsigned long long GetOpenedFilesCount();//Total ever opened / ever closed
		static unsigned long long GetClosedFilesCount();

		static unsigned long long GetOpenedFilesSec();//The number of files opened/closed in the last second
		static unsigned long long GetClosedFilesSec();
		static CountTracker& GetTracker();
	};
}

