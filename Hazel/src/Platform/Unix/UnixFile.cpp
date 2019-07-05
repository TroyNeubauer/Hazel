#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "Hazel/System/File.h"
#include "Hazel/Util/Utils.h"
#include "Platform/System/FileTrackerFunctions.h"
#include "Hazel/System/System.h"

namespace Hazel {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
	File::File(const char* path, bool sequential, FileError* error) : m_Path(path)
	{
		uint64_t pageSize = System::PageSize();
		m_FileHandle = open(path, O_RDONLY);
		if(m_FileHandle == -1)
		{
			*error = FileError::FILE_NOT_FOUND;
			return;
		}
#ifdef HZ_DEBUG
		Log_fopen((FILE*) m_FileHandle, path, "rb", __FILE__, __LINE__);
#elif HZ_RELEASE
		Log_fopen((FILE*) m_FileHandle, path);
#endif
		struct stat stats;
		if(fstat(m_FileHandle, &stats) == -1)
		{
			*error = FileError::OTHER;
			return;
		}
		m_Length = stats.st_size;

		m_Data = mmap(nullptr, m_Length, PROT_READ, MAP_PRIVATE, m_FileHandle, 0);
		if(m_Data == MAP_FAILED)
		{
			switch(errno)
			{
				case EBADF:
					*error = FileError::FILE_NOT_FOUND;
					break;

				case EACCES:
					*error = FileError::ACCESS_DENIED;
					break;
					
				case EAGAIN:
				case ENOMEM:
				case EOVERFLOW:
					*error = FileError::NOT_ENOUGH_MEMORY;
					break;					
				
				case ENFILE:
					*error = FileError::TOO_MANY_FILES;
					break;
				
				default:
					*error = FileError::OTHER;
					break;
			}
			
			return;
		}
		void* neededAddress = (char*) m_Data + m_Length;
		if (m_Length % pageSize == 0)//There is no null byte since the string takes up an entire page
		{
			m_OtherPage = mmap(neededAddress, pageSize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
			if (m_OtherPage == MAP_FAILED)
			{	
				munmap(m_OtherPage, pageSize);
				//The page couldn't be allocated where it needed to be
				//So allocate a buffer big enough, release all the file handles, and then give the user the buffer
				char* newData = (char*) malloc(m_Length + 1);
				memcpy(newData, m_Data, m_Length);
				newData[m_Length] = 0x00;//Ensure there is a null byte at the end
				
				m_FreeData = true;
				munmap(m_Data, m_Length);
				m_Data = newData;//Give the user access to the file
			}
		}
#ifdef HZ_DEBUG
			Log_fclose((FILE*) m_FileHandle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
			Log_fclose((FILE*) m_FileHandle, 0);
#endif
		close(m_FileHandle);
		m_FileHandle = 0;
	}
#pragma GCC diagnostic pop

	File::File(File&& other)
	{
		other.m_Length = this->m_Length;
		other.m_Data = this->m_Data;
		other.m_FreeData = this->m_FreeData;
		this->m_Data = nullptr;
		this->m_CompletedDeInit = true;
	}

	File::~File()
	{
		if (!m_CompletedDeInit)
		{
			m_CompletedDeInit = true;
			if (m_FreeData)
			{	//In this case m_Data is the block we malloc'ed
				//We only need to free m_Data because the file handle and view were destroyed in the constructor
				free(m_Data);
			}
			else
			{
				munmap(m_Data, m_Length);
				if (m_OtherPage)//We allocated another page to get space for the null byte
				{
					munmap(m_OtherPage, System::PageSize());
				}
			}
		}

	}
}


#endif