#include "hzpch.h"

#include "Path.h"
#include "File.h"

namespace Hazel {
	File Path::Open() 
	{
		return File(m_Path);
	}
	
	File Path::Open(bool sequential, FileError* error)
	{
		return File(m_Path, sequential, error);
	}


}

