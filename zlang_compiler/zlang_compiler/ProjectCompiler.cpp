
#include "Common.h"
#include "ProjectCompiler.h"
#include <stdio.h>
#include <string.h>
#include "Utils.h"
#include "ErrorReporter.h"

CProjectCompiler::CProjectCompiler()
{
	
}

CProjectCompiler* CProjectCompiler::GetInstance()
{
	static CProjectCompiler* obj = NULL;

	if (!obj)
	{
		obj = new CProjectCompiler();
	}

	return obj;
}

void CProjectCompiler::SetSaveMode(const std::string& saveFileName)
{
	m_saveFileName = saveFileName;
}

void CProjectCompiler::LoadFromMode(const std::string& loadFileName)
{
	m_loadFileName = loadFileName;
}

void CProjectCompiler::SetFolderPath(const std::string& folderPath)
{
	m_folderPath = folderPath;
}

bool CProjectCompiler::GetClearedFileContent(const std::string& path, std::string& content, unsigned int& linesCount)const
{
	FILE* fl = OpenFile(path, "r");
	if (!fl)
	{
		CErrorReporter::GetInstance()->OnFatalError(CANNOT_OPEN_SOURCE_FILE, 0xFFFFFFFF, path);
		return false;
	}

	char tmp[4096];	
	while ((!feof(fl)) && (fgets(tmp, sizeof(tmp) - 1, fl)))
	{
		std::string line = tmp;
		TrimTabsAndSpaces(line);
		content += line;
		if ((line.size() > 0) && (line[line.size() - 1] == '\n'))
		{
			linesCount++;
		}
	}

	fclose(fl);

	return true;
}

std::string CProjectCompiler::GetRootFolder()
{
	return "root";
}

std::string CProjectCompiler::GetPlatformRootFolder(const std::string platform)
{
	std::string fileName = GetRootFolder();
	fileName += "_";
	fileName += (platform == "") ? PLATFORM_NAME : platform;
	return fileName;
}

bool CProjectCompiler::Process()
{
	AddExcludeFolder(GetRootFolder());
	AddExcludeFolder(GetPlatformRootFolder("windows"));
	AddExcludeFolder(GetPlatformRootFolder("macosx"));
	AddExcludeFolder(GetPlatformRootFolder("linux"));

	if (!FindFiles(m_folderPath))
	{
		return false;
	}

	if (m_files.size() == 0)
	{
		CErrorReporter::GetInstance()->OnFatalError(INPUT_FILE_NOT_FOUND, 0xFFFFFFFF);		
		return false;	
	}

	CErrorReporter::GetInstance()->SetProjectFilesInfo(m_files);

	return true;
}

unsigned int CProjectCompiler::GetFileSize(const std::string& filePath)const
{
	FILE* fl = OpenFile(filePath, "rb");
	if (!fl)
	{
		return 0xFFFFFFFF;
	}

	fseek(fl, 0, 2);
	unsigned int size = ftell(fl);
	fclose(fl);

	return size;
}

bool CProjectCompiler::ReadFileInfo(const std::string& filePath, std::string& content, unsigned int& linesCount)const
{
	unsigned int size = GetFileSize(filePath);
	if (0xFFFFFFFF == size)
	{
		CErrorReporter::GetInstance()->OnFatalError(CANNOT_OPEN_SOURCE_FILE, 0xFFFFFFFF, filePath);
		return false;
	}

	// + 2 because we maybe will add newline to the end
	content.reserve(size + 2);
	linesCount = 0;

	bool res = GetClearedFileContent(filePath, content, linesCount);
	content += "\n";
	linesCount++;

	return res;
}

bool CProjectCompiler::HandleFile(const std::string& path, const std::string& name)
{
	SProjectFile* newFile = new SProjectFile();
	newFile->name = name;
	newFile->fullPath = path;

	std::string content;
	if (!ReadFileInfo(path + name, content, newFile->linesCount))
	{
		return false;
	}

	m_singleFileContent.reserve(m_singleFileContent.size() + content.size());
	m_singleFileContent += content;

	newFile->lineStart = 0;

	if (m_files.size() != 0)
	{
		unsigned int lastIndex = m_files.size() - 1;
		newFile->lineStart = m_files[lastIndex]->linesCount + m_files[lastIndex]->lineStart;
	}

	m_files.push_back(newFile);
	return true;
}

FILE* CProjectCompiler::GetSingleSourceFile()
{
	if (m_saveFileName.size() > 0)
	{
		FILE* fl = OpenFile(m_saveFileName, "w");
		if (fl)
		{
			fprintf(fl, "%s", m_singleFileContent.c_str());
			fclose(fl);	
		}		
	}

	if (m_loadFileName.size() > 0)
	{
		FILE* fl = OpenFile(m_loadFileName, "r");
		return fl;
	}

	m_singleFile = GetTempFileFromString(m_singleFileContent, m_singleFilePath);
	if (!m_singleFile)
	{
		CErrorReporter::GetInstance()->OnFatalError(CANNOT_WRITE_TO_TEMPORARY_FILE, 0xFFFFFFFF, m_singleFilePath);
	}	
	return m_singleFile;
}

CProjectCompiler::~CProjectCompiler()
{
	remove(m_singleFilePath.c_str());

	std::vector<SProjectFile *>::iterator it = m_files.begin();
	std::vector<SProjectFile *>::iterator itEnd = m_files.end();

	for ( ; it != itEnd; it++)
	{
		delete *it;
	}
}
