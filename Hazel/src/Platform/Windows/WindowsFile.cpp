#include "hzpch.h"
#include <Windows.h>

#include "WindowsUtils.h"
#include "Hazel/System/File.h"
#include "Hazel/Util/Utils.h"
#include "Hazel/System/System.h"

#ifdef HZ_PLATFORM_WINDOWS
namespace Hazel {
	File::File(const char* path, bool sequential, FileError* error)
	{
		DWORD dwFlagsAndAttributes = sequential ? FILE_FLAG_SEQUENTIAL_SCAN : FILE_FLAG_RANDOM_ACCESS;
		m_FileHandle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
		
		if (m_FileHandle == INVALID_HANDLE_VALUE)
		{
			if (error) {
				DWORD lastError = GetLastError();
				switch (lastError) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					*error = FileError::FILE_NOT_FOUND;
					break;
				case ERROR_ACCESS_DENIED:
					*error = FileError::ACCESS_DENIED;
					break;
				case ERROR_TOO_MANY_OPEN_FILES:
					*error = FileError::TOO_MANY_FILES;
					break;
				default:
					*error = FileError::OTHER;
					break;
				}
			}
			m_Data = nullptr;
			return;
		}
		LARGE_INTEGER size;
		if (!GetFileSizeEx(m_FileHandle, &size))
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			//HZ_CORE_ERROR("Failed to get file length for file \"{0}\", error from GetFileSizeEx is: {1}", path, errorMessage);
			if(error)
				*error = FileError::OTHER;
			CloseHandle(m_FileHandle);
			m_FileHandle = INVALID_FILE_HANDLE;
			return;
		}
		m_Length = size.QuadPart;

		HANDLE mappingHandle = CreateFileMappingA(m_FileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (mappingHandle == INVALID_HANDLE_VALUE)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			HZ_CORE_ERROR("Failed to create file mapping for file: \"{0}\", error from CreateFileMappingA is: {1}", path, errorMessage);
			if (error)
				*error = FileError::OTHER;
			CloseHandle(m_FileHandle);
			m_FileHandle = INVALID_FILE_HANDLE;
			return;
		}

		m_Data = MapViewOfFile2(mappingHandle, GetCurrentProcess(), 0, nullptr, 0, 0, PAGE_READONLY);
		if (m_Data == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			HZ_CORE_ERROR("Failed to map view of file: \"{0}\", error from MapViewOfFile2 is: {1}", path, errorMessage);
			if (error)
				*error = FileError::OTHER;
			CloseHandle(m_FileHandle);
			m_FileHandle = INVALID_FILE_HANDLE;
			return;
		}
	}

	File::File(File&& other)
	{
		other.m_Length = this->m_Length;
		other.m_FileHandle = this->m_FileHandle;
		other.m_Data = this->m_Data;
		other.m_FreeData = this->m_FreeData;

		this->m_FileHandle = INVALID_FILE_HANDLE;
		this->m_Data = nullptr;
	}

	File::~File()
	{
		if (m_Data && m_FreeData) {//In this case m_Data is the block we malloc'ed
			//We only need to free m_Data because the file handle and view were destroyed in the constructor
			free(m_Data);
			m_Data = nullptr;
		} else {
			if (m_FileHandle != INVALID_FILE_HANDLE) {
				CloseHandle(m_FileHandle);
				m_FileHandle = INVALID_FILE_HANDLE;
			}
		}	

	}
}
#endif
