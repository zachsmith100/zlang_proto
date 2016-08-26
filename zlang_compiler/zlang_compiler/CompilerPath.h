
#pragma once

#include <string>

class CCompilerPath
{
public:
	static std::string Get();
	static void InitInstance(int argc, char* argv[]);	

private:
	static CCompilerPath* GetInstance();

	void Init(const std::string& path);

	CCompilerPath();
	CCompilerPath(const CCompilerPath& other);
	void operator =(const CCompilerPath& other);
	~CCompilerPath();

	std::string m_path;
};