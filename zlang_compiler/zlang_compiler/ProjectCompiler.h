
#pragma once

#include <vector>
#include <string>
#include "FileFinder.h"

struct SProjectFile
{
	std::string name;
	std::string fullPath;
	unsigned int linesCount;
	unsigned int lineStart;
};

class CProjectCompiler : public CFileFinder
{
public:
	static CProjectCompiler* GetInstance();

	void SetFolderPath(const std::string& folderPath);

	bool Process();
	FILE* GetSingleSourceFile();

	std::string GetRootFolder();
	std::string GetPlatformRootFolder(const std::string platform = "");

	void SetSaveMode(const std::string& saveFileName);
	void LoadFromMode(const std::string& loadFromName);	

private:
	CProjectCompiler();
	CProjectCompiler(const CProjectCompiler& other);
	void operator =(const CProjectCompiler& other);
	~CProjectCompiler();

	bool HandleFile(const std::string& path, const std::string& name);

	unsigned int GetFileSize(const std::string& filePath)const;
	bool ReadFileInfo(const std::string& filePath, std::string& content, unsigned int& linesCount)const;	
	bool GetClearedFileContent(const std::string& path, std::string& content, unsigned int& linesCount)const;

	std::vector<SProjectFile *> m_files;

	std::string m_folderPath;

	std::string m_singleFileContent;
	std::string m_singleFilePath;
	FILE* m_singleFile;

	std::string m_saveFileName;
	std::string m_loadFileName;
};