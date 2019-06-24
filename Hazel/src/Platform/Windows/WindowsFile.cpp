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
		uint64_t pageSize = System::PageSize();
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
#ifdef HZ_DEBUG
		Log_fopen((FILE*) m_FileHandle, path, "rb", __FILE__, __LINE__);
#elif HZ_RELEASE
		Log_fopen((FILE*) m_FileHandle, path, "rb");
#endif
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

		HANDLE viewHandle = CreateFileMappingA(m_FileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (viewHandle == INVALID_HANDLE_VALUE)
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
		
		//m_Data = MapViewOfFile2(m_FileViewHandle, GetCurrentProcess(), 0, nullptr, 0, 0, PAGE_READONLY);
		m_Data = MapViewOfFile(viewHandle, FILE_MAP_READ, 0, 0, 0);
		if (m_Data == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			HZ_CORE_ERROR("Failed to map view of file: \"{0}\", error from MapViewOfFile2 is: {1}", path, errorMessage);
			if (error)
				*error = FileError::OTHER;
			CloseHandle(m_FileHandle);
			CloseHandle(viewHandle);
			m_FileHandle = INVALID_FILE_HANDLE;
			return;
		}
		CloseHandle(viewHandle);

		void* neededAddress = (char*) m_Data + m_Length;
		if (m_Length % pageSize == 0)//There is no null byte since the string takes up an entire page
		{
			m_OtherPage = VirtualAlloc(neededAddress, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READONLY);
			if (neededAddress != m_OtherPage)
			{	
				if (m_OtherPage == nullptr) {
					char errorMessage[1024];
					WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
					HZ_CORE_ERROR("Failed to allocate page for null terminator: error from VirtualAlloc is: {0}", errorMessage);
				}
				//The page couldn't be allocated where it needed to be
				//So allocate a buffer big enough, release all the file handles, and then give the user the buffer
				m_FreeData = true;
				char* newData = (char*) malloc(m_Length + 1);
				memcpy(newData, m_Data, m_Length);
				newData[m_Length] = 0x00;//Ensure there is a null byte at the end
				
				CloseHandle(m_FileHandle);//We no longer need the file, all the data is safely in newData
				UnmapViewOfFile(m_Data);
				m_Data = newData;//Give the user access to the file
			}
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
		if (!m_CompletedDeInit)
		{
			m_CompletedDeInit = true;
#ifdef HZ_DEBUG
			Log_fclose((FILE*) m_FileHandle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
			Log_fclose((FILE*) m_FileHandle);
#endif
			if (m_FreeData)
			{	//In this case m_Data is the block we malloc'ed
				//We only need to free m_Data because the file handle and view were destroyed in the constructor
				free(m_Data);
			}
			else
			{
				CloseHandle(m_FileHandle);
				UnmapViewOfFile(m_Data);
				if (m_OtherPage)//We allocated another page to get space for the null byte
				{
					VirtualFree(m_OtherPage, 0, MEM_RELEASE);
				}
			}
		}

	}
}
#endif
