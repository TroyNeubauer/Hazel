#pragma once

#include "Hazel/Core.h"

namespace Hazel {

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

	enum class FileError
	{
		NONE = 0, FILE_NOT_FOUND, ACCESS_DENIED, TOO_MANY_FILES, NOT_ENOUGH_MEMORY, IS_DIRECTORY, IS_FILE, INVALID_PARAMETER, UNKNOWN
	};

	static const uint32_t APPEND_MASK = (uint32_t) FileOpenOptions::APPEND, MODE_MASK = (uint32_t) FileOpenOptions::READ_WRITE;
	static const FileOpenOptions DEFAULT_OPTIONS = (FileOpenOptions) (FileOpenOptions::READ_WRITE | FileOpenOptions::APPEND);


	const char* FileErrorToString(FileError error);


}

