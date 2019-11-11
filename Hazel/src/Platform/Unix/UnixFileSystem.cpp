#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX
#include "FileSystem.h"

namespace Hazel {


}
#endif



#include "Hazel/System/FileSystem.h"
#include "Hazel/System/Path.h"
#include "Hazel/System/Timer.h"
#include "WindowsUtils.h"

namespace Hazel {

	bool FileSystem::Exists(const char* path)
	{
		return false;
	}

	bool FileSystem::PathsEqual(const char* a, const char* b)
	{
		return false;
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		return false;
	}

	bool FileSystem::CreateFile(const char* path)
	{
		return true;
	}

	bool FileSystem::CreateFileWithParents(const char* path)
	{
		return true;
	}

	bool FileSystem::CreateDirectory(const char* path)
	{
		return false;
	}

	bool FileSystem::CreateDirectories(const char* path)
	{
		return true;
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		return true;
	}

	void FileSystem::AbsloutePath(const char* file, char* buf, uint32_t bufLength)
	{
	}

	bool FileSystem::Delete(const char* path)
	{
		return false;
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, uint64_t& p_FileLength, FileError* p_Error, uint64_t p_Offset, uint64_t p_Bytes)
	{
		return nullptr;
	}

	void FileSystem::UnmapFile(void* file)
	{

	}

	/*File::File(File&& other) : m_Path(other.GetPath())
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
			Log_fclose((FILE*)m_FileHandle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
			Log_fclose((FILE*)m_FileHandle, 0);
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

	}*/

}
