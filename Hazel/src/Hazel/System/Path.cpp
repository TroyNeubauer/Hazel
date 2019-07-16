#include "hzpch.h"

#include "Path.h"
#include "File.h"
#include "Hazel/Util/StringUtils.h"
#include "Hazel/System/FileSystem.h"

namespace Hazel {
	const Path Path::Invalid = Path("");

	inline bool Path::operator==(const Path& other) const
	{
		this->Normalize();
		other.Normalize();
		return StringUtils::Equal(this->m_Path.c_str(), other.m_Path.c_str());
	}

	void Path::Normalize() const
	{
		if (normalized) 
			return;
		//Use const cast here since normalizing a path isnt mutating the state of this object from the user's perspective
		Path& _this = const_cast<Path&>(*this);//USE SPARINGLY
		FileSystem::NormalizePath(m_Path, m_Path, m_Path.capacity());
		_this.normalized = true;
	}

	Path Path::GetParent() const
	{
		return "temp_path";
	}


	Path Path::GetChild(const char* childName) const
	{
		return "temp_path";
	}

	File Path::Open()
	{
		return File(m_Path.c_str());
	}

	File Path::Open(bool sequential, FileError* error)
	{
		return File(m_Path.c_str(), sequential, error);
	}

	void Path::CheckPath()
	{
		if (StringUtils::Contains(m_Path, ".zip")) {

		}
	}


}

