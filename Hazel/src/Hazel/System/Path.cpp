#include "hzpch.h"

#include "Path.h"
#include "File.h"
#include "Hazel/Util/StringUtils.h"
#include "Hazel/System/FileSystem.h"

namespace Hazel {
	const Path Path::Invalid = Path("");

	inline bool Path::operator==(const Path& other) const
	{
		return FileSystem::PathsEqual(this->m_Path, other.m_Path);
	}

	Path Path::GetParent() const
	{
		return "temp_path";
	}

	Path Path::GetChild(const char* childName) const
	{
		return "temp_path";
	}

	File* Path::Open(FileOpenOptions options)
	{
		return File::Open(*this, options);
	}

	File* Path::Open(FileOpenOptions options, FileError& error)
	{
		return File::Open(*this, options, &error);
	}

	void Path::UpdatePath()
	{
		bool isLastArchive = false;
		m_InArchive = false;
		FileSystem::PathNameIterator(m_Path, [this, &isLastArchive](const char* fileName, const char* rest) -> bool {
			if (isLastArchive)
			{
				m_InArchive = true;
				return true;//Break
			}
			isLastArchive = FileSystem::HasArchiveExtension(fileName);
			return false;
		});
		//Use const cast here since normalizing a path isnt mutating the state of this object from the user's perspective
		Str128 normalized;
		FileSystem::NormalizePath(m_Path, normalized, m_Path.capacity());
		StringUtils::Copy(m_Path, normalized);
	}
	static const char* fileErrorNames[] = { "No Error", "File Not Found", "Access Denied", "Too Many Files Open", "Not Enough Memory", "Is Directory", "Invalid Parameter", "Unknown Error" };
	static const int s_ErrorNamesLength = sizeof(fileErrorNames) / sizeof(const char*);

	const char* FileErrorToString(FileError error)
	{
		if ((uint64_t) error >= s_ErrorNamesLength)
			return "Invalid Error! Out of bounds";
		else
			return fileErrorNames[(uint64_t) error];
	}

	Path::Path(const char* path) : m_Path(path) { UpdatePath(); }
	Path::Path(const std::string& path) : m_Path(path.c_str()) { UpdatePath(); }
	Path::Path(Str& path) : m_Path(path.c_str()) { UpdatePath(); }

	Path::operator const Str& () { return m_Path; }
	Path::operator char* () { return m_Path; }
	Path::operator const char* () const { return m_Path.c_str(); }
	const char* Path::ToString() const { return m_Path.c_str(); }


	bool Path::Exists() const { return FileSystem::Exists(m_Path); }
	bool Path::IsDirectory() const { return FileSystem::IsDirectory(m_Path); }
	bool Path::IsFile() const { return !IsDirectory(); }

	bool Path::Delete() const { return FileSystem::Delete(m_Path); }
	bool Path::Truncate() const { return FileSystem::TruncateFile(m_Path); }

	bool Path::CreateFile() { FileSystem::CreateFile(m_Path); return Exists(); }
	bool Path::CreateDirectory() { FileSystem::CreateDirectory(m_Path); return Exists(); }

	const char* Path::AbsloutePath(char* buf, size_t bufLength) { FileSystem::AbsloutePath(m_Path, buf, bufLength); return buf; }


	bool Path::IsInvalid() const { return *this == Path::Invalid; }

}

