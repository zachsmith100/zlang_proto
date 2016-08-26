
#pragma once

#include <string>

class CCompileExecutor
{
public:
	static CCompileExecutor* GetInstance();

	bool Execute(const std::string& programPath, const std::string& outputProjectDir, const std::string& outputFilePath, 
		const std::string& outputErrorFilePath);

	void SetCompilerPath(const std::string& compilerPath);

	bool SafeExecute(const std::string& path, const std::string& params, const std::string& outputProjectDir, bool runFromProgramFolder = false);
    
	static std::string GetQuotedString(const std::string& str);

private:
	CCompileExecutor();
	CCompileExecutor(const CCompileExecutor& other);
	void operator =(const CCompileExecutor& other);
	~CCompileExecutor();

	std::string m_compilerPath;
};