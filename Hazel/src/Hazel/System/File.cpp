#include "hzpch.h"
#include "File.h"

#include <archive.h>
#include <archive_entry.h>

namespace Hazel {

	static File* OpenRegularFile(const char* path, FileOpenOptions options, FileError* error, uint64_t& length)
	{
		void* memory = FileSystem::MapFile(path, options, length, error);
		if (*error != FileError::NONE)
			return nullptr;
		return new MemoryMappedFile(Path(path), options, error, (uint8_t*) memory, length);
	}


	File* Hazel::File::Open(Path& p_Path, FileOpenOptions p_Options, FileError* p_Error)
	{
		uint64_t length;
		if (p_Path.m_InArchive)
		{
			char* end = nullptr;
			FileSystem::PathNameIterator(p_Path, [&end](char* fileName, char* rest) -> bool {
				
				if (FileSystem::HasArchiveExtension(fileName))
				{
					end = rest;
					return true;
				}
				return false;
			});
			ArchivedFile* result = nullptr;
			end[-1] = 0x00;//Change the seperator to a null byte to make the fileOnDisk a seperate string from internalFile
			File* parent = OpenRegularFile(Path(p_Path), p_Options, p_Error, length);
 			if (*p_Error != FileError::NONE)
			{
				delete parent;
				return nullptr;
			}
			while (end && *end)
			{
				char* start = end;
				FileSystem::PathNameIterator(end, [&end](char* fileName, char* rest) -> bool {

					if (FileSystem::HasArchiveExtension(fileName))
					{
						end = rest;
						return true;
					}
					return false;
				});
				if (start == end)
				{
					end = nullptr;//There are no more recursive archives so make this be the last iteration
				}
				else
				{
					end[-1] = 0x00;
				}
				result = new ArchivedFile(parent, p_Path, p_Options, p_Error);
			}
			HZ_CORE_ASSERT(false, "Unable to find seperation between archive file and internal file!");
			return result;
		}
		else
		{
			return OpenRegularFile(p_Path, p_Options, p_Error, length);
		}
	}
	

	ArchivedFile::ArchivedFile(File* parent, const Path& path, FileOpenOptions options, FileError* error)
		: File(path, options), m_Parent(parent)
	{
		archive_entry* entry;
		archive* a = archive_read_new();
		archive_read_support_filter_all(a);
		archive_read_support_format_all(a);
		uint64_t r = archive_read_open_memory(a, parent->Data(), parent->Length());
		if (r != ARCHIVE_OK)
			printf("Invalid file %s\n", path.ToString());
		printf("archive %s contains: \n", path.ToString());
		while ((r = archive_read_next_header(a, &entry)) == ARCHIVE_OK)
		{
			const char* pathName = archive_entry_pathname(entry);
			printf("\tentry name %s\n", pathName);
			if (FileSystem::PathsEqual(pathName, path))
			{
				m_Length = archive_entry_size(entry);
				m_Data = new uint8_t[m_Length + 1];
				r = archive_read_data(a, m_Data, m_Length);
				if (r < 0)// Error
					goto finish;
				HZ_CORE_ASSERT(r == m_Length, "Did not read entire entry! Expected length: {}, Bytes actually read {}", m_Length, r);
				m_Data[m_Length] = 0x00;

				goto finish;
			}
			else
			{
				archive_read_data_skip(a);
			}
		}
		*error = FileError::FILE_NOT_FOUND;
	finish:
		if (r < 0)
		{
			printf("Error reading archive! Error: %s\n", archive_error_string(a));
			*error = FileError::UNKNOWN;
		}
		r = archive_read_free(a);
	}

	void ArchivedFile::Save()
	{
	}

	void ArchivedFile::Write(void* data, uint64_t bytes, uint64_t offset)
	{
		if (offset + bytes > m_Length)
		{
			uint8_t* oldData = m_Data;
			m_Length = offset + bytes;
			m_Data = new uint8_t[m_Length + 1];
			if (m_MutableString)
			{
				delete m_MutableString;
				m_MutableString = new char[m_Length + 1];
			}
			memcpy(m_Data, oldData, offset);
			m_Data[m_Length] = 0x00;
			delete[] oldData;
		}
		memcpy(m_Data + offset, data, bytes);
	}

	void ArchivedFile::Read(void* data, uint64_t bytes, uint64_t offset)
	{
		HZ_CORE_ASSERT(offset + bytes <= m_Length, "Attempt to read past the end of the buffer");
		memcpy(data, m_Data + offset, bytes);
	}

	char* ArchivedFile::AsMutableString()
	{
		if (m_MutableString == nullptr)
		{
			m_MutableString = new char[m_Length + 1];
		}
		memcpy(m_MutableString, m_Data, m_Length);
		m_MutableString[m_Length] = 0x00;
		return m_MutableString;
	}

	ArchivedFile::~ArchivedFile()
	{
		if (m_Data)
			delete[] m_Data;
	}


	MemoryMappedFile::MemoryMappedFile(const Path& path, FileOpenOptions options, FileError* error, uint8_t* data, uint64_t length)
		: File(path, options)
	{
		this->m_Length = length;
		this->m_Data = data;
	}

	void MemoryMappedFile::Save()
	{
		//Nop. Since we are using memory mapping, the OS and TLB handle the heavy lifting
	}

	void MemoryMappedFile::Write(void* data, uint64_t bytes, uint64_t offset)
	{
		if (offset + bytes > m_Length)
		{
			if (m_StringData)
			{
				delete[] m_StringData;
			}
			m_Length = offset + bytes;
			m_StringData = new char[m_Length + 1];

		}
		memcpy(m_Data + offset, data, bytes);
	}

	void MemoryMappedFile::Read(void* data, uint64_t bytes, uint64_t offset)
	{
		HZ_CORE_ASSERT(offset + bytes <= m_Length, "Attempt to read past the end of the buffer");
		memcpy(data, m_Data + offset, bytes);
	}

	const char* MemoryMappedFile::AsString()
	{
		if (m_Write)
		{
			if (m_StringData == nullptr)
			{
				m_StringData = new char[m_Length + 1];
			}
			memcpy(m_StringData, m_Data, m_Length);
			m_StringData[m_Length] = 0x00;
			return m_StringData;
		}
		else
		{
			return (const char*) m_Data;//We know there is a null byte at the end
		}
	}

	char* MemoryMappedFile::AsMutableString()
	{
		if (m_MutableString == nullptr)
		{
			m_MutableString = new char[m_Length + 1];
		}
		memcpy(m_MutableString, m_Data, m_Length);
		m_MutableString[m_Length] = 0x00;
		return m_MutableString;
	}

	MemoryMappedFile::~MemoryMappedFile()
	{
		FileSystem::UnmapFile(m_Data);
		if (m_StringData)
		{
			delete[] m_StringData;
		}
	}

}

