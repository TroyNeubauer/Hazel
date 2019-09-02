#include <archive.h>
#include <archive_entry.h>

#include <vector>
#include <map>


#include "Hazel.h"



int main()
{
	Hazel::Log::Init();
	const char* str = "Fast string finding is fun";
	Hazel::StringUtils::FindBegin(str, "is");
	HZ_CORE_INFO("Result: {}", str);

	system("PAUSE");
	Hazel::Log::DisableLogging();
}

#if 0
void ArchiveTest()
{
	Hazel::Path path("./1.zip/1/2.zip/2/3.tar/3/4/test.txt");
	Hazel::FileError error = Hazel::FileError::NONE;
	Hazel::File* file = path.Open(Hazel::FileOpenOptions::READ, error);
	if (error != Hazel::FileError::NONE)
	{
		HZ_CORE_TRACE("Error opening file! {}", FileErrorToString(error));
	}
	else {
		char buf[512];
		HZ_CORE_TRACE("Opened file {}\n", path.AbsloutePath(buf, sizeof(buf)));
	}
	delete file;
}


#elif 0
const char* ARCHIVE_INIT = "Archive Init", * READ_FILE = "Read File", * DECODE_ENTRIES = "Decode Entries", * CLEAN_UP = "Clean Up";

#define ADD_TIME(testName)	\
	timer.Stop();			\
	result[{ sampleName, testName }] += timer.Nanos();\
	timer.Start();

void ArchiveBenchmark()
{
	Hazel::Log::Init();
	const int TESTS = 100;
	std::vector<const char*> sampels = { "archive.tar", "sample1.zip", "sample2.zip" };
	std::vector<const char*> tests = { ARCHIVE_INIT, READ_FILE, DECODE_ENTRIES, CLEAN_UP };
	std::map<std::pair<const char*, const char*>, uint64_t> result;// Maps sample name + test name to time
	for (int i = 0; i < TESTS; i++)
	{
		for (const char* sampleName : sampels)
		{
			for (const char* testName : tests) {
				if (result.find({ sampleName, testName }) == result.end()) {
					result[{ sampleName, testName }] = 0;
				}
			}

			Hazel::Timer timer;
			archive* a;
			archive_entry* entry;
			int r;

			a = archive_read_new();
			archive_read_support_filter_all(a);
			archive_read_support_format_all(a);
			ADD_TIME(ARCHIVE_INIT);
			r = archive_read_open_filename(a, sampleName, 10240); // Note 1
			ADD_TIME(READ_FILE);
			if (r != ARCHIVE_OK)
				printf("Invalid file %s\n", sampleName);
			while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
				//printf("%s\n", archive_entry_pathname(entry));
				archive_read_data_skip(a);  // Note 2
			}
			ADD_TIME(DECODE_ENTRIES);
			r = archive_read_free(a);  // Note 3
			ADD_TIME(CLEAN_UP);
			if (r != ARCHIVE_OK)
				printf("Invalid file %s\n", sampleName);
		}
	}
	printf("Averages:\n");
	const char* lastSample = nullptr;
	for (std::map<std::pair<const char*, const char*>, uint64_t>::iterator it = result.begin(); it != result.end(); it++) {
		const std::pair<const char*, const char*>& entry = (*it).first;
		if (entry.first != lastSample) {
			printf("For sample %s:\n", entry.first);
			lastSample = entry.first;
		}
		char time[128];
		Hazel::Timer::FormatNanos(time, sizeof(time), (*it).second / TESTS);
		printf("\tSample %s, test %s = %s\n", entry.first, entry.second, time);

	}
}
#endif