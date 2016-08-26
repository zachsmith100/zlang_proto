
#include "Test.h"
#include "CompileExecutor.h"
#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define OUTPUT_DIR "zlang_output"

CTest::CTest(const std::string& name):
	m_name(name),
	m_testPath(""),
	m_outputPath(""),
	m_filesHandleMode(COMPARE_OUTPUT_FILES)
{
	static CFolderPath currentFolder(GetCurrentFolder());
	static unsigned int globalNum = 0;
	globalNum++;

	m_testPath = currentFolder;
	m_testPath.Add(m_name);
	m_isOk = IsDirectoryExists(m_testPath.GetShortPath().c_str());		
	m_num = globalNum;
}

CTest::CTest(const std::string& relativePath, const std::string& outputPath):
	m_name(relativePath),
	m_testPath(""),	
	m_outputPath(""),
	m_filesHandleMode(COMPARE_OUTPUT_FILES)
{
	static CFolderPath currentFolder(GetCurrentFolder());
	static unsigned int globalNum = 0;
	globalNum++;

	m_testPath = currentFolder;
	m_testPath.Add(m_name);
	m_isOk = IsDirectoryExists(m_testPath.GetShortPath().c_str());		
	m_outputPath = currentFolder;
	m_outputPath.Add(outputPath);
	m_num = globalNum;

	printf("test path %s\n", m_testPath.ToStdString().c_str());
	printf("out path %s\n", m_outputPath.ToStdString().c_str());
}

void CTest::PrintError(const std::string& errorDesc, const std::string& correctFilePath, const std::string& incorrectFilePath, const int diffLine)
{
	printf("Test %s is failed! %s...\n", m_name.c_str(), errorDesc.c_str());
	
	if ((correctFilePath.size() == 0) || (incorrectFilePath.size() == 0))
	{
		return;
	}

	printf("Correct output(%s):\n", correctFilePath.c_str());

	PrintFile(correctFilePath, diffLine);

	printf("\nIncorrect output(%s):\n", incorrectFilePath.c_str());

	PrintFile(incorrectFilePath, diffLine);

	printf("\n\n");

	if (diffLine != 0)
	{
		printf("First difference was at line %d\n", diffLine);
	}
}

void CTest::PrintFile(const CFilePath& filePath, const int diffLine)
{
	FILE* fl = OpenFile(filePath, "r");
	if (!fl)
	{
		printf("FILE NOT FOUND\n");
		return;
	}
	int i = 0;
	while (!feof(fl))
	{
		i++;
		char tmp[1024];
		if (fgets(tmp, sizeof(tmp), fl))
		{
			if (i == diffLine)
			{
				printf(">>> ");
			}
			printf("%s", tmp);
		}
	}
	fclose(fl);
}

void ProcessString(std::string& str)
{
	while ((str.size() > 0) && ((str[str.size() - 1] == '\n') ||
		(str[str.size() - 1] == '\r')))
	{
		str.erase(str.begin() + str.size() - 1);
	}
}

bool CTest::CompareOutputFiles(const CFilePath& firstFilePath, const CFilePath& secondFilePath, int& differLine)
{
	FILE* firstFile = OpenFile(firstFilePath, "r");
	if (!firstFile) 
	{
		return false;
	}

	FILE* secondFile = OpenFile(secondFilePath, "r");
	if (!secondFile)
	{
		fclose(firstFile);
		return false;
	}

	differLine = 0;

	while ((!feof(firstFile)) && (!feof(secondFile)))
	{
		differLine++;
		char tmp1[1024], tmp2[1024];
		if (!fgets(tmp1, sizeof(tmp1), firstFile))
		{
			tmp1[0] = 0;
		}
		if (!fgets(tmp2, sizeof(tmp2), secondFile))
		{
			tmp2[0] = 0;			
		}

		std::string s1 = tmp1;
		ProcessString(s1);
		std::string s2 = tmp2;
		ProcessString(s2);

		if (s1 != s2)
		{
			fclose(secondFile);
			fclose(firstFile);
			return false;
		}
	}

	bool bothFilesComeToEnd = (feof(firstFile)) && (feof(secondFile));
	
	fclose(secondFile);
	fclose(firstFile);

	return bothFilesComeToEnd;
}

std::string CTest::GetExeName()const
{
#ifndef PLATFORM_UNIX
	return "project.exe";
#else
	return "project";
#endif
}

bool CTest::Execute()
{
	if (!m_isOk)
	{
		PrintError("Test data is not found");
		return false;
	}

	CFolderPath programPath(m_testPath);
	programPath.Add("program");
	CFilePath programInputPath = m_testPath;
	programInputPath.Add("program_input");
	
	CFilePath correctCompilerOutputPath = m_testPath;
	correctCompilerOutputPath.Add("compiler_output");
	CFilePath correctCompilerErrorOutputPath = m_testPath;
	correctCompilerErrorOutputPath.Add("compiler_error_output");
	CFilePath actualCompilerOutputPath = GetOutputFilePath();
	CFilePath actualCompilerErrorOutputPath = GetOutputErrorFilePath();

	CFolderPath outputProjectDir = GetOutputProjectDir();
	ForceDirectory(outputProjectDir);
	
	if (!CCompileExecutor::GetInstance()->Execute(programPath, outputProjectDir, actualCompilerOutputPath, actualCompilerErrorOutputPath))
	{
		PrintError("Compiler execute error");		
		return false;
	}

	int diffLine = 0;
	if (!CompareOutputFiles(correctCompilerOutputPath, actualCompilerOutputPath, diffLine))
	{
		PrintError("Compiler output is wrong", correctCompilerOutputPath, actualCompilerOutputPath, diffLine);
		return false;
	}

	if (!CompareOutputFiles(correctCompilerErrorOutputPath, actualCompilerErrorOutputPath, diffLine))
	{
		PrintError("Compiler output is wrong", correctCompilerErrorOutputPath, actualCompilerErrorOutputPath, diffLine);
		return false;
	}

	CFilePath computerFailedFlagPath = m_testPath;
	computerFailedFlagPath.Add("compiler_failed");

	if (IsFileExists(computerFailedFlagPath))
	{
		OnTestPassed();
		return true;
	}

	CopyInputFilesIfNeeded(outputProjectDir);
	DeleteOutputFilesIfNeeded();

	CFilePath actualProgramOutputPath = outputProjectDir;
	actualProgramOutputPath.Add("program_output");
	
    if (!CCompileExecutor::GetInstance()->SafeExecute(CCompileExecutor::GetQuotedString(outputProjectDir.ToStdString() + GetExeName()), "> \"" + actualProgramOutputPath.ToStdString() + "\" < \"" + programInputPath.ToStdString() + "\"", outputProjectDir.ToString(), true))
	{
		PrintError("Program cannot be run");
		return false;
	}
	
	CFilePath correctProgramOutputPath = m_testPath;
	correctProgramOutputPath.Add("program_output");	
	
	if (!CompareOutputFiles(correctProgramOutputPath, actualProgramOutputPath, diffLine))
	{
		PrintError("Program output is wrong", correctProgramOutputPath, actualProgramOutputPath, diffLine);
		return false;
	}

	const unsigned int STREAM_COUNT = 4;
	const std::string streams[STREAM_COUNT] = {"info", "error", "warnings", "trace"};
	for (int k = 0; k < STREAM_COUNT; k++)
	{
		correctProgramOutputPath = m_testPath;
		correctProgramOutputPath.Add("program_" + streams[k]);			

		actualProgramOutputPath = outputProjectDir;
		actualProgramOutputPath.Add(GetExeName() + "." + streams[k]);

		if (!CompareOutputFiles(correctProgramOutputPath, actualProgramOutputPath, diffLine))
		{
			PrintError("Program " + streams[k] + " is wrong", correctProgramOutputPath, actualProgramOutputPath, diffLine);
			return false;
		}
	}

	if (!CheckOutputFilesIfNeeded())
	{
		return false;
	}

	OnTestPassed();
	return true;
}

void CTest::OnTestPassed()
{
	printf("%d. Test %s. OK!\n", m_num, m_name.c_str());
}

CFolderPath CTest::GetOutputProjectDir()
{
	if (!m_outputPath.IsEmpty())
	{
		return m_outputPath;		
	}
	
	CFolderPath path(GetTempFolder());
	path.Add(OUTPUT_DIR);
	return path;
}

CFilePath CTest::GetOutputFilePath()
{
	CFilePath path(GetTempFolder().GetShortPath());
	path.Add(m_name + ".out");
	return path;
}

CFilePath CTest::GetOutputErrorFilePath()
{
	CFilePath path(GetTempFolder().GetShortPath());
	path.Add(m_name + "_err.out");
	return path;
}

bool CTest::ProcessOutputFiles()
{
	std::string originalFilesPath = m_testPath;
	originalFilesPath += "output";

	if (!IsDirectoryExists(originalFilesPath.c_str()))
	{
		return true;
	}

	originalFilesPath += PATH_SEPARATOR;

	return FindFiles(originalFilesPath);
}

bool CTest::DeleteOutputFilesIfNeeded()
{
	m_filesHandleMode = DELETE_OUTPUT_FILES;

	return ProcessOutputFiles();
}

bool CTest::CheckOutputFilesIfNeeded()
{
	m_filesHandleMode = COMPARE_OUTPUT_FILES;

	return ProcessOutputFiles();
}

bool CTest::HandleFile(const std::string& path, const std::string& name)
{
	if (m_filesHandleMode == DELETE_OUTPUT_FILES)
	{
		CFilePath path = GetOutputProjectDir().ToStdString() + name;
		remove(path);
		return true;
	}
	else if (m_filesHandleMode == COMPARE_OUTPUT_FILES)
	{
		CFilePath actualProgramOutputPath = GetOutputProjectDir().ToStdString() + name;
		CFilePath correctProgramOutputPath = path + name;
	
		int diffLine = 0;
		if (!CompareOutputFiles(correctProgramOutputPath, actualProgramOutputPath, diffLine))
		{
			PrintError("Output file " + correctProgramOutputPath.ToStdString() + " is wrong", correctProgramOutputPath, actualProgramOutputPath, diffLine);
			return false;
		}

		return true;
	}
	else
	{
		assert(false);
		return true;
	}
}

void CTest::CopyInputFilesIfNeeded(const CFolderPath& outputProjectDir)
{
	std::string inputFilesPath = m_testPath;
	inputFilesPath += "input";

	if (!IsDirectoryExists(inputFilesPath.c_str()))
	{
		return;
	}

	inputFilesPath += PATH_SEPARATOR;

#ifdef PLATFORM_UNIX
	std::string cmd  = "cp -r ";	
#else
	std::string cmd  = "copy ";
#endif
    
	cmd += "\"" + inputFilesPath;

#ifdef PLATFORM_UNIX
	cmd += ".";
#endif

	cmd += "\" \"";
	cmd += outputProjectDir.ToStdString();
	cmd += "\"";
	system(cmd.c_str());
}

CTest::~CTest()
{
}
