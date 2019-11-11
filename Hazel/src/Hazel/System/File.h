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

	//A class that uses fast, low-level API's quiclky access files
	//Be warned, a file handle will be opened upon constructing an instance of this object
	class File
	{
	public:
		static File* Open(Path& path, FileOpenOptions options = DEFAULT_OPTIONS, FileError* error = nullptr);

		File(const Path& path, FileOpenOptions options) : m_Path(path), m_Length((uint64_t) -1),
			m_Read(options & FileOpenOptions::READ), m_Write(options& FileOpenOptions::WRITE), m_Append(options & FileOpenOptions::APPEND) {}

		//No copying
		File(File& other) = delete;
		File(File&& other);

		//Returns the length of the file in bytes
		inline uint64_t Length() { return m_Length; }

		inline bool CanRead() { return m_Read; }
		inline bool CanWrite() { return m_Write; }

		virtual void Save() = 0;

		template<typename T>
		inline void Write(T* data, uint64_t elements, uint64_t offset = 0) { Write(static_cast<void*>(data), sizeof(T) * elements, offset); }
		virtual void Write(void* data, uint64_t bytes, uint64_t offset = 0) = 0;

		template<typename T>
		inline void Read(T* data, uint64_t elements, uint64_t offset = 0) { Read(static_cast<void*>(data), sizeof(T) * elements, offset); }
		virtual void Read(void* data, uint64_t bytes, uint64_t offset = 0) = 0;


		//Returns true if calling Data() is allowed. This is the case for memory mapped files or files backed by a block of memory
		//If this returns false then Calling Data et all. will return null
		virtual bool HasDirectAccess() = 0;

		//Returns a pointer to the first byte of the file
		//If CanWrite if false and the given pointer is written to, the results are undefined
		virtual void* Data() = 0;

		//Identical to Data()
		inline uint8_t* const Begin() { return (uint8_t*) Data(); }
		//Returns the first invalid byte of the file (the byte where the null terminator sits for AsString). For use in iterators
		inline uint8_t* const End() { return Begin() + Length(); }

		//Returns the data of this file with a null termination character at the end
		virtual const char* AsString() = 0;

		//Returns the mutable copy of this file with a null termination character at the end
		//Calling this will destroy any modifications made to the string returned from the last call to AsMutableString()
		virtual char* AsMutableString() = 0;

		inline const Path& GetPath() { return m_Path; }

		virtual ~File() {}

	protected:
		const Path m_Path;
		uint64_t m_Length;
		const bool m_Read, m_Write, m_Append;
	};

	//ArchivedFile represents a file within an archive using the location of the archive on disk as well as path specifying the file relative to the archive
	class ArchivedFile : public File
	{
	public:
		ArchivedFile(File* parent, const Path& path, FileOpenOptions options, FileError* error);

		virtual void Save();

		virtual void Write(void* data, uint64_t bytes, uint64_t offset = 0);
		virtual void Read(void* data, uint64_t bytes, uint64_t offset = 0);

		inline virtual bool HasDirectAccess() { return true; }
		inline virtual void* Data() { return m_Data; }
		inline virtual const char* AsString() { return (const char*) m_Data; }
		
		virtual char* AsMutableString();

		~ArchivedFile();
	private:
		File* m_Parent;
		uint8_t* m_Data = nullptr;
		char* m_MutableString = nullptr;
	};

	class MemoryMappedFile : public File
	{
	public:
		MemoryMappedFile(const Path& path, FileOpenOptions options, FileError* error, uint8_t* data, uint64_t length);

		virtual void Save();

		virtual void Write(void* data, uint64_t bytes, uint64_t offset = 0);
		virtual void Read(void* data, uint64_t bytes, uint64_t offset = 0);

		inline virtual bool HasDirectAccess() { return true; }
		inline virtual void* Data() { return m_Data; }
		virtual const char* AsString();

		virtual char* AsMutableString();

		~MemoryMappedFile();
	private:
		uint8_t* m_Data;
		char* m_StringData = nullptr;
		char* m_MutableString = nullptr;
	};
}
