
#include "Common.h"
#include "Path.h"
#include "Utils.h"

CPath::CPath(const CPath& other)
{
    m_path = other.m_path;
}

CPath::CPath(const std::string& path)	
{	
	Init(path.c_str());
}

CPath::CPath(const char* path)
{
	Init(path);
}

void CPath::Init(const char* path)
{
	m_path = path;

#ifdef PLATFORM_UNIX
	CharReplace(m_path, '\\', '/');
#endif
}

CPath::CPath(const std::string& path1, const std::string& path2)
{
	m_path = path1 + PATH_SEPARATOR + path2;
}

CPath::CPath(const std::string& path1, const std::string& path2, const std::string& path3)
{
	m_path = path1 + PATH_SEPARATOR + path2 + PATH_SEPARATOR + path3;
}

CPath::CPath(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4)
{
	m_path = path1 + PATH_SEPARATOR + path2 + PATH_SEPARATOR + path3 + PATH_SEPARATOR + path4;
}

CPath::operator std::string()const
{
	return m_path;
}

CPath::operator const char*()const
{
	return m_path.c_str();
}

const char* CPath::ToString()const
{
    return m_path.c_str();
}

std::string CPath::ToStdString()const
{
	return m_path;
}

bool CPath::IsEmpty()const
{
	return m_path.size() == 0;
}

void CPath::Add(const std::string& path)
{
    Add(path.c_str());
}

void CPath::Add(const char* path)
{
    m_path += PATH_SEPARATOR;
    m_path += path;
}

std::string CPath::GetDirectory()const
{
	int pos = m_path.find_last_of(PATH_SEPARATOR);	
	if (pos == std::string::npos)
	{
		return "";	
	}	

	return m_path.substr(0, pos + 1);
}

CFolderPath::CFolderPath(const CFolderPath& other):
    CPath(other.m_path)
{
   PostProcessPath();
}

CFolderPath::CFolderPath(const std::string& path):
	CPath(path)
{
	PostProcessPath();
}

CFolderPath::CFolderPath(const char* path):
	CPath(path)
{
	PostProcessPath();
}

CFolderPath::CFolderPath(const std::string& path1, const std::string& path2):
	CPath(path1, path2)
{
	PostProcessPath();
}

CFolderPath::CFolderPath(const std::string& path1, const std::string& path2, const std::string& path3):
	CPath(path1, path2, path3)
{
	PostProcessPath();
}

CFolderPath::CFolderPath(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4):
	CPath(path1, path2, path3, path4)
{
	PostProcessPath();
}

void CFolderPath::PostProcessPath()
{
	if (m_path.size() > 0)
	{
		if (m_path[m_path.size() - 1] != PATH_SEPARATOR_CHAR)
		{
			m_path += PATH_SEPARATOR;
		}
	}
}

void CFolderPath::Add(const std::string& path)
{
	Add(path.c_str());
}

void CFolderPath::Add(const char* path)
{
	m_path += path;
	PostProcessPath();
}

std::string CFolderPath::GetShortPath()const
{
	return m_path.substr(0, m_path.size() - 1);
}

std::string CFolderPath::GetDirectory()const
{
	return m_path;
}