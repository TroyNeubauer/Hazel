#pragma once

#include <string>
#include <Str.h>
#include "Hazel/System/FileSystem.h"

namespace Hazel {

	class File;

	enum FileOpenOptions
	{
		READ = BIT(0),
		WRITE = BIT(1),
		APPEND = BIT(2),
		RANDOM_ACCESS = BIT(3),
		READ_WRITE = READ | WRITE,
		TRUNCATE = 0,	//Bit second bit is off
		SCAN = 0,		// the third bit is off
	};

	static const uint32_t APPEND_MASK = (uint32_t) FileOpenOptions::APPEND, MODE_MASK = (uint32_t) FileOpenOptions::READ_WRITE;
	static const FileOpenOptions DEFAULT_OPTIONS = (FileOpenOptions) (FileOpenOptions::READ_WRITE | FileOpenOptions::APPEND);

	enum class FileError
	{
		NONE = 0, FILE_NOT_FOUND, ACCESS_DENIED, TOO_MANY_FILES, NOT_ENOUGH_MEMORY, IS_DIRECTORY, IS_FILE, INVALID_PARAMETER, UNKNOWN
	};
	const char* FileErrorToString(FileError error);
	
	//A class that represents a path to a file or directory on disk
	//Evuantaly it will be able to read from archives
	class Path
	{
	public:
		Path(const char* path);
		Path(std::string path);
		Path(Str& path);

		operator const Str& ();
		operator char* ();
		explicit operator const char* () const;
		const char* ToString() const;
		bool operator==(const Path& other) const;

		//Returns a path that represents the parent of this path. If this path is already the root, this method returns the same path
		Path GetParent() const;

		bool Exists() const;
		bool IsDirectory() const;
		bool IsFile() const;

		bool Delete() const;
		bool Truncate() const;

		//Creates this path as either a path or a directory.
		//If the dir/file already exists, no action is taken and the function returns true.
		//If the dir/file doesnt exist, it created with false being returned only in the cases where the dir/file was unable to be created
		bool CreateFile();
		bool CreateDirectory();
	
		//COpies the absloute path of this file into buf and returns buf
		const char* AbsloutePath(char* buf, size_t bufLength);

		//Returns a new path that has the given name and this path as the parent. This assumes that this path is a directory. 
		//If this path represents an existing file and the platform doesnt support a directory and a file with the same name, 
		//Path::Invalid is returned and FileError is set to FileError::Directory
		Path GetChild(const char* childName) const;
		bool IsInvalid() const;

		File* Open(FileOpenOptions options = DEFAULT_OPTIONS);
		File* Open(FileOpenOptions options, FileError& error);
	private:
		void UpdatePath();

	public:
		static const Path Invalid;

	private:
		Str128 m_Path;
		bool m_InArchive;

		friend class File;
	};
}