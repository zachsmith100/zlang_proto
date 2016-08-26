
#include "Common.h"
#include "Utils.h"
#include <sys/types.h>
#include <sys/stat.h>

#if defined(PLATFORM_WINDOWS)
	#include <direct.h>
	#include <shlobj.h>
	#include <winreg.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

bool IsDirectoryExists(const char* path)
{
	struct stat info;

	if (stat( path, &info ) != 0)
	{
		return false;
	}

	return ((info.st_mode & S_IFDIR) != 0);	
}

bool ForceDirectories(const CFolderPath& startPath, const CFolderPath& endPath)
{
	std::string path = endPath.GetShortPath();
	std::vector<std::string> folders = SplitStringIntoVector(path, PATH_SEPARATOR_CHAR);

	std::vector<std::string>::iterator it = folders.begin();
	std::vector<std::string>::iterator itEnd = folders.end();	
	std::string currentFolder = startPath;
	for ( ; it != itEnd; it++)
	{		
		currentFolder += *it;
		currentFolder += PATH_SEPARATOR;
		if (!ForceDirectory(currentFolder))
		{
			return false;
		}
	}

	return true;
}

bool ForceDirectory(const CFolderPath& _path)
{
	std::string path = _path.GetShortPath();
	
	if (!IsDirectoryExists(path.c_str()))
	{		
#if defined(PLATFORM_WINDOWS)
		_mkdir(path.c_str());
#elif defined(PLATFORM_UNIX)
		mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
		if (!IsDirectoryExists(path.c_str()))
		{
			return false;
		}
	}
	
	return true;
}

bool FileCopy(const CPath& inputPath, const CPath& outputPath)
{
	FILE* inputFile = OpenFile(inputPath, "rb");
	if (!inputFile)
	{
		return false;
	}

	FILE* outputFile = OpenFile(outputPath, "wb");
	if (!outputFile)
	{
		fclose(inputFile);
		return false;
	}

	fseek(inputFile, 0, 2);
	unsigned int size = ftell(inputFile);
	fseek(inputFile, 0, 0);

	char* buffer = new char[size];
	if (fread(buffer, 1, size, inputFile) != size)
	{
		return false;
	}
	if (fwrite(buffer, 1, size, outputFile) != size)
	{
		return false;
	}

	fclose(inputFile);
	fclose(outputFile);
	return true;
}

bool IsFileExists(const CPath& path)
{
	FILE* fl = OpenFile(path, "r");
	if (!fl)
	{
		return false;
	}

	fclose(fl);
	return true;
}

std::string IntToStr(int num)
{
	char tmp[20];
#if defined(PLATFORM_UNIX)
	sprintf(tmp, "%d", num);
#elif defined(PLATFORM_WINDOWS)
	sprintf_s(tmp, sizeof(tmp) - 1, "%d", num);
#else
  #error Unsupported OS
#endif
	return tmp;
}

FILE* OpenFile(const std::string& fileName, const std::string& mode)
{
#if defined(PLATFORM_WINDOWS)
	FILE* fl;
	fopen_s(&fl, fileName.c_str(), mode.c_str());
	return fl;
#elif defined( PLATFORM_UNIX)
	return fopen(fileName.c_str(), mode.c_str());
#else
  #error Unsupported OS
#endif
}

void CharReplace(std::string& str, const char from, const char to)
{
	std::string::iterator it = str.begin();
	std::string::iterator itEnd = str.end();
	for ( ; it != itEnd; it++)
	if (*it == from)
	{
		*it = to;
	}
}

void TrimTabsAndSpaces(std::string& str)
{
	if (str.size() == 0)
	{
		return;
	}

	int i = str.size() - 1;
	while (((str[i] == 10) || (str[i] == 13)) && (i > 0))
	{
		i--;
	}

	while ((i >= 0) && ((str[i] == '\t') || (str[i] == ' ')))
	{
		str.erase(str.begin() + i);
		i--;
	}
}

std::vector<std::string> SplitStringIntoVector(const std::string& str, const char delim)
{
	std::vector<std::string> result;

	std::string element = "";
	for (unsigned int i = 0; i < str.size(); i++)
	if (str[i] == delim)
	{
		result.push_back(element);
		element = "";
	}
	else
	{
		element += str[i];
	}

	if (element.size() != 0)	
	{
		result.push_back(element);
	}

	return result;
}

bool ExecuteExternalCommand(const std::string& path, const std::string& params)
{
#if defined(PLATFORM_WINDOWS)
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = path.c_str();
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;
	ShExecInfo.hInstApp = NULL;
	bool res = (ShellExecuteEx(&ShExecInfo) == TRUE);
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
	return res;
#elif defined(PLATFORM_UNIX)
	std::string shellCmd = path;
	shellCmd += " ";
	shellCmd += params;
	system(shellCmd.c_str());
	return true;
#else
  #error Unsupported OS
#endif
}

std::string GetCurrentFolder()
{
	char cCurrentPath[FILENAME_MAX];

#if defined(PLATFORM_UNIX)
	getcwd
#elif defined(PLATFORM_WINDOWS)
	_getcwd
#else
  #error Unsupported OS
#endif
		(cCurrentPath, sizeof(cCurrentPath));

	return cCurrentPath;
}

#ifndef PLATFORM_UNIX
std::string GetApplicationDataDirectory()
{
	char tmp[MAX_PATH];
	SHGetSpecialFolderPathA(NULL, tmp, CSIDL_APPDATA, true);
	return tmp;
}

std::string GetTempFilePath()
{
	CFolderPath basePath(GetApplicationDataDirectory());	
	int i = 0;
	while (IsFileExists(CPath(basePath, IntToStr(i))))
	{
		i++;
	}
	CPath path(basePath, IntToStr(i));
	return path;
}
#else
std::string GetTempFilePath()
{
	return ""; // not needed for UNIX right now
}
#endif

std::string GetCppCompilerPath()
{
#if defined(PLATFORM_WINDOWS)
	HKEY key;
	LSTATUS status = RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\4.0\\", &key);
	if (ERROR_SUCCESS != status)
	{	
		printf("error %d getting key #1\n", (int)status);
		return "";
	}

	char tmp[MAX_PATH];
	DWORD size = MAX_PATH;
	status = RegQueryValueEx(key, "MSBuildToolsPath", NULL, NULL, (LPBYTE)&tmp, &size);
	if (ERROR_SUCCESS != status)
	{
		printf("error %d getting key #2\n", (int)status);
		return "";
	}	

	RegCloseKey(key);
		
	return tmp;
#else
	return "";
#endif
}

CFolderPath GetTempFolder()
{
#if defined(PLATFORM_WINDOWS)
	char tmp[MAX_PATH];
	SHGetSpecialFolderPathA(NULL, tmp, CSIDL_COMMON_APPDATA, true);	
	return tmp;
#elif defined(PLATFORM_UNIX)	
	return P_tmpdir;
#else
  #error Unsupported OS
#endif
}

std::string GetPathAsStringConst(const std::string& path)
{
#if defined(PLATFORM_UNIX)
	return path;
#elif defined(PLATFORM_WINDOWS)
	std::vector<std::string> arr = SplitStringIntoVector(path, PATH_SEPARATOR_CHAR);
	if (arr.size() == 0)
	{
		return "";
	}

	std::string res = arr[0];
	for (unsigned int i = 1; i < arr.size(); i++)
	{
		res += PATH_SEPARATOR;
		res += PATH_SEPARATOR;
		res += arr[i];
	}

	return res;
#else
  #error Unsupported OS
#endif
}

FILE* GetTempFileFromString(std::string& fileContent, std::string& filePath)
{
#ifdef PLATFORM_UNIX
	FILE* singleFile = fmemopen((void*)fileContent.c_str(), fileContent.size(), "r");
	return singleFile;
#else
	filePath = GetTempFilePath();
	FILE* singleFile = OpenFile(filePath, "w");
	if (!singleFile)
	{		
		return NULL;
	}
	fprintf(singleFile, "%s", fileContent.c_str());
	fileContent = "";
	fclose(singleFile);
	singleFile = OpenFile(filePath, "r");
	if (!singleFile)
	{		
		return NULL;
	}
	return singleFile;
#endif	
}

void StringReplace(std::string& str, std::string from, std::string to)
{
	int p = str.find(from);
	
	while (p != std::string::npos) 
	{ 
		str.replace(p, from.length(), to); 
		p += to.length();
		p = str.find(from, p);
	}
}

void StringReplaceOnce(std::string& str, std::string from, std::string to)
{
	int p = str.find(from);
	
	if (p != std::string::npos) 
	{ 
		str.replace(p, from.length(), to); 
		p += to.length();
		p = str.find(from, p);
	}
}

std::string UnquoteString(const std::string& str)
{
	std::string res = str;
	if ((res.size() > 0) && (res[0] == '"'))
	{
		res.erase(res.begin());
	}
	if ((res.size() > 0) && (res[res.size() - 1] == '"'))
	{
		res.erase(res.begin() + res.size() - 1);
	}
	StringReplace(res, "\\\\", "\\");

	return res;
}