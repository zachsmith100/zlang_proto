
#pragma once

#include <string>

#if defined(PLATFORM_WINDOWS)
	#define PATH_SEPARATOR "\\"
	#define PATH_SEPARATOR_CHAR '\\'
#elif defined(PLATFORM_UNIX)
	#define PATH_SEPARATOR "/"
	#define PATH_SEPARATOR_CHAR '/'
#else
  #error Unsupported OS
#endif

class CPath
{
public:
    CPath(const CPath& path);
	CPath(const std::string& path);
	CPath(const char* path);

	CPath(const std::string& path1, const std::string& path2);
	CPath(const std::string& path1, const std::string& path2, const std::string& path3);
	CPath(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4);

	operator std::string()const;
	operator const char*()const;
    
    virtual void Add(const std::string& path);
    virtual void Add(const char* path);
    
    const char* ToString()const;
	std::string ToStdString()const;

	virtual std::string GetDirectory()const;

	bool IsEmpty()const;

private:
	void Init(const char* path);

protected:
	std::string m_path;
};

class CFolderPath : public CPath
{
public:
    CFolderPath(const CFolderPath& path);
	CFolderPath(const std::string& path);
	CFolderPath(const char* path);

	CFolderPath(const std::string& path1, const std::string& path2);
	CFolderPath(const std::string& path1, const std::string& path2, const std::string& path3);
	CFolderPath(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4);

	void PostProcessPath();

	void Add(const std::string& path);
	void Add(const char* path);

	std::string GetShortPath()const;
	std::string GetDirectory()const;
};

typedef CPath CFilePath;
