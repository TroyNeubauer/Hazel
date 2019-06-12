#pragma once

#include <string>

#include "File.h"

namespace Hazel {
	//A class that represents a path to a file or directory on disk
	//Evuantaly it will be able to read from archives
	class Path
	{
	public:
		inline Path(const char* path) : m_Path(path) {}
		inline Path(std::string path) : m_Path(path.c_str()) {}

		inline operator const char* () { return m_Path; }
		inline const char* ToString() { return m_Path; }

		File Open() { return File(m_Path, false, nullptr); }
		File Open(bool sequential, FileError* error) { return File(m_Path, sequential, error); }

	private:
		const char* m_Path;
	};
}