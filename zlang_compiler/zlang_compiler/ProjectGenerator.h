
#pragma once

#include "Common.h"
#include <string>
#include <vector>
#include "CompileTarget.h"
#include "FileFinder.h"

enum ProjectGeneratorError
{
	SUCCESS = 0,
	CREATE_DIRECTORY_ERROR,
	FILES_COPY_ERROR,
	NO_TEMPLATES_ERROR
};

class CProjectGenerator : public CFileFinder
{
public:
	CProjectGenerator(int argc, char* argv[]);
	~CProjectGenerator();

	ProjectGeneratorError GetErrorCode()const;

	void SetInputPath(const CFolderPath& inputDirectory);
	void SetOutputPath(const CFolderPath& outputDirectory);
	ProjectGeneratorError Generate();

	std::string GetMainFilePath()const;
	
	bool PostProcessProject();

	bool Compile();

private:
	bool HandleFile(const std::string& path, const std::string& name);
	void CopyFolder(const std::string& folderName);
	bool CopyRoot();

	std::string GetFilePath(const std::string& nameFile)const;

	std::string GetProjectTemplatesPath()const;
	std::string GetEntryPointCreationCode()const;

	void CheckProjectFilesTemplates();
	
	bool GenerateCommonConfig();
	std::string NumericTypeToString(const VariableType& varType);
	
	ProjectGeneratorError m_errorCode;
	CFolderPath m_outputDir;
	CFolderPath m_inputDir;
	CFolderPath m_rootPath;

	std::vector<std::string> m_projectFiles;
	std::string m_mainFileFooter;	

	CFolderPath m_projectTemplatesPath;

	bool m_rootCopySuccess;
};