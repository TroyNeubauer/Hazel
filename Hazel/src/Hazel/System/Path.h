#pragma once

#include <string>

namespace Hazel {

	class File;
	
	enum FileError
	{
		NONE = 0, FILE_NOT_FOUND, ACCESS_DENIED, TOO_MANY_FILES, NOT_ENOUGH_MEMORY, OTHER
	};
	//A class that represents a path to a file or directory on disk
	//Evuantaly it will be able to read from archives
	class Path
	{
	public:
		inline Path(const char* path) : m_Path(path) {}
		inline Path(std::string path) : m_Path(path.c_str()) {}

		inline operator const char* () { return m_Path; }
		inline const char* ToString() { return m_Path; }

		File Open();
		File Open(bool sequential, FileError* error);

	private:
		const char* m_Path;
	};
}