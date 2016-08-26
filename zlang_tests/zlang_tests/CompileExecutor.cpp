
#include "Common.h"
#include "CompileExecutor.h"
#include <assert.h>
#include "Utils.h"
#ifdef PLATFORM_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#define _chdir chdir
#endif

CCompileExecutor::CCompileExecutor()	
{
}

CCompileExecutor::CCompileExecutor(const CCompileExecutor& other)
{
	assert(false);
}

CCompileExecutor* CCompileExecutor::GetInstance()
{
	static CCompileExecutor* obj = NULL;

	if (!obj)
	{
		obj = new CCompileExecutor();
	}

	return obj;
}

void CCompileExecutor::SetCompilerPath(const std::string& compilerPath)
{
	m_compilerPath = compilerPath;
}

std::string CCompileExecutor::GetQuotedString(const std::string& str)
{
	std::string res = "\"" + str;
	if (str[str.size() - 1] == '\\')
	{
		res += " \" ";
	} 
	else
	{
		res += "\" ";
	}
	return res;
}

bool CCompileExecutor::Execute(const std::string& programPath, const std::string& outputProjectDir, const std::string& outputFilePath, const std::string& outputErrorFilePath)
{
	std::string platform = "gcc";

#ifdef WIN32
	platform = "msvc";
#endif

#ifdef __APPLE__
	platform = "clang";
#endif

	return SafeExecute(m_compilerPath, GetQuotedString(programPath) + GetQuotedString(outputProjectDir) + GetQuotedString(platform) + GetQuotedString("--nofilenames")  + GetQuotedString("--projecttree") + " 1>" + outputFilePath + " 2>" + outputErrorFilePath, outputProjectDir);
}

bool CCompileExecutor::SafeExecute(const std::string& path, const std::string& params, const std::string& outputProjectDir, bool runFromProgramFolder)
{
#ifdef PLATFORM_UNIX
	std::string cmd;
	if (runFromProgramFolder)
	{
		cmd = "cd \"" + outputProjectDir;
		cmd += "\" && ";
	}
	return ExecuteExternalCommand(cmd + path, params);
#endif

	std::string batPath = outputProjectDir + "temp.bat";

	_chdir(outputProjectDir.c_str());
	FILE* fl = OpenFile(batPath, "wb");
	if (!fl)
	{
		return false;
	}
	fprintf(fl, "cd %s\n", outputProjectDir.c_str());	
	fprintf(fl, "%s %s\n", path.c_str(), params.c_str());	
	fclose(fl);

	return ExecuteExternalCommand(batPath, "");
}

void CCompileExecutor::operator =(const CCompileExecutor& other)
{
	assert(false);
}

CCompileExecutor::~CCompileExecutor()
{
}
