
#include "Common.h"
#include <stdio.h>
#include "Compiler.h"
#include "ErrorReporter.h"
#include "CompilerPath.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Usage: compiler [input folder] [output folder] [target (optional)]\n");
		return -1;
	}

	CCompilerPath::InitInstance(argc, argv);

	// This is a step to give ability to check if some error templates are missing
	// We must know about it in compiler develop time :)
	CErrorReporter::GetInstance();
	
	CCompiler compiler(argc, argv);
	compiler.Compile();

 	return 0;
}

