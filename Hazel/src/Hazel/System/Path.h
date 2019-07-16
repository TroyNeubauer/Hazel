#pragma once

#include <string>
#include <Str.h>
#include "Hazel/System/FileSystem.h"

namespace Hazel {

	class File;
	
	enum FileError
	{
		NONE = 0, FILE_NOT_FOUND, ACCESS_DENIED, TOO_MANY_FILES, NOT_ENOUGH_MEMORY, DIRECTORY, OTHER
	};
	//A class that represents a path to a file or directory on disk
	//Evuantaly it will be able to read from archives
	class Path
	{
	public:
		inline Path(const char* path) : m_Path(path) { CheckPath(); }
		inline Path(std::string path) : m_Path(path.c_str()) { CheckPath(); }
		inline Path(Str& path) : m_Path(path.c_str()) { CheckPath(); }

		inline operator const char* () { return m_Path.c_str(); }
		inline operator const Str& () { return m_Path; }
		inline const char* ToString() { return m_Path.c_str(); }
		inline bool operator==(const Path& other) const;

		//Returns a path that represents the parent of this path. If this path is already the root, this method returns the same path
		Path GetParent() const;
		void Normalize() const;

		//Returns a new path that has the given name and this path as the parent. This assumes that this path is a directory. 
		//If this path represents an existing file and the platform doesnt support a directory and a file with the same name, 
		//Path::Invalid is returned and FileError is set to FileError::Directory
		Path GetChild(const char* childName) const;
		inline Path GetChild(std::string childName) const { return GetChild(childName.c_str()); }
		inline bool IsInvalid() const { return *this == Path::Invalid; }

		File Open();
		File Open(bool sequential, FileError* error);
	private:
		void CheckPath();

	public:
		static const Path Invalid;

	private:
		Str128 m_Path;
		bool normalized, normalFile;
	};
}