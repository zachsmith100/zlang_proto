
#pragma once

#include <string>
#include "ProjectCompiler.h"
#include "ProjectGenerator.h"

class CCompiler
{
public:
	CCompiler(int argc, char* argv[]);
	~CCompiler();

	bool Compile();	

private:
	bool RunLexerAndParser();
	bool GenerateCode();
	bool GenerateProject();
	bool CompileGeneratedProject();	

	CProjectGenerator m_projectGenerator;	

	std::string m_inputPath;
	std::string m_outputPath;	
};