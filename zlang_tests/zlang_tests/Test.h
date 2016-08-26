
#pragma once

#include "Common.h"
#include <string>
#include "FileFinder.h"

enum FilesHandleMode
{
	DELETE_OUTPUT_FILES = 0,
	COMPARE_OUTPUT_FILES
};

class CTest : public CFileFinder
{
public:
	CTest(const std::string& relativePath, const std::string& outputPath);
	CTest(const std::string& name);
	~CTest();

	bool Execute();	

	bool HandleFile(const std::string& path, const std::string& name);

private:		
	bool CompareOutputFiles(const CFilePath& firstFilePath, const CFilePath& secondFilePath, int& differLine);
	void PrintError(const std::string& errorDesc, const std::string& correctFilePath = "", const std::string& incorrectFilePath = "", const int diffLine = 0);

	void PrintFile(const CFilePath& filePath, const int diffLine = -1);

	void CopyInputFilesIfNeeded(const CFolderPath& outputProjectDir);

	bool CheckOutputFilesIfNeeded();
	bool DeleteOutputFilesIfNeeded();
	bool ProcessOutputFiles();

	std::string GetExeName()const;

	CFolderPath GetOutputProjectDir();
	CFilePath GetOutputFilePath();
	CFilePath GetOutputErrorFilePath();

	void OnTestPassed();

	bool m_isOk;
	unsigned int m_num;
	std::string m_name;
	CFolderPath m_testPath;	
	CFolderPath m_outputPath;

	FilesHandleMode m_filesHandleMode;
};
