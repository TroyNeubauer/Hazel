#pragma once

#include <stdint.h>
#include <string>

#include "Hazel/System/Path.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include <Windows.h>
	typedef HANDLE FILE_HANDLE;
	const FILE_HANDLE INVALID_FILE_HANDLE = INVALID_HANDLE_VALUE;
#elif defined(HZ_PLATFORM_UNIX)
	typedef int FILE_HANDLE;
	const FILE_HANDLE INVALID_FILE_HANDLE = 0;
#else
	#error
#endif

namespace Hazel {

	const char* FileErrorToString(FileError error);

	//A class that uses fast, low-level API's to memory map a file for read only access and provide useful access to the data
	//Be warned, a file handle will be opened upon constructing an instance of this object
	class File
	{
	public:
		File(const char* path, bool sequential, FileError* error);
		inline File(const char* path) : File(path, true, nullptr) {}
		
		inline File(std::string path, bool sequential, FileError* error) : File(path.c_str(), sequential, error) {}
		inline File(std::string path) : File(path, true, nullptr) {}

		//No copying
		File(File& other) = delete;
		File(File&& other);

		inline bool IsValid() { return m_FileHandle != INVALID_FILE_HANDLE; }

		//Returns the amount of bytes the file consumes
		inline uint64_t Length() { return m_Length; }

		//Returns a pointer to the first byte of the file
		inline void* const Data() { return m_Data; }
		//Identical to Data()
		inline uint8_t* const Begin() { return (uint8_t*) Data(); }
		//Returns the first invalid byte of the file (the byte where the null terminator sits). For use in iterators
		inline uint8_t* const End() { return Begin() + Length(); }

		//Returns the first byte of this file with a null termination character at the end
		inline char* const AsString() { return (char*) Data(); }

		inline Path GetPath() { return m_Path; }

		~File();

	private:
		Path m_Path;

		uint64_t m_Length;
		//The platform dependent location of the file
		FILE_HANDLE m_FileHandle = INVALID_FILE_HANDLE;
		//Holds a pointer to the data
		void* m_Data;
		//If mapping another page to write the null termination character fails, this
		// will be true indicating that m_Data must be freed instead of closed
		bool m_FreeData = false, m_CompletedDeInit = false;
		void* m_OtherPage = nullptr;
	};
}
