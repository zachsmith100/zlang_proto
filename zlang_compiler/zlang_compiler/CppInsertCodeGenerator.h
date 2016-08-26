
#pragma once

#include "BaseCodeGenerator.h"

class CCppInsertCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CCppInsertCodeGenerator)

	DECLARE_HANDLER(ProcessCppInsert);
	DECLARE_HANDLER(ProcessCppDeclaration);
	DECLARE_HANDLER(ProcessInclude);
	DECLARE_HANDLER(ProcessWindowsInclude);
	DECLARE_HANDLER(ProcessLinuxInclude);
	DECLARE_HANDLER(ProcessMacOsXInclude);
	DECLARE_HANDLER(ProcessUnixInclude);
	DECLARE_HANDLER(ProcessWindowsImport);	
	DECLARE_HANDLER(ProcessUnixImport);
	DECLARE_HANDLER(ProcessLinuxImport);	
	DECLARE_HANDLER(ProcessMacOsXImport);
	DECLARE_HANDLER(ProcessIncludePath);
	DECLARE_HANDLER(ProcessWindowsIncludePath);
	DECLARE_HANDLER(ProcessUnixIncludePath);
	DECLARE_HANDLER(ProcessLinuxIncludePath);
	DECLARE_HANDLER(ProcessMacOsXIncludePath);
	DECLARE_HANDLER(ProcessWindowsImportPath);	
	DECLARE_HANDLER(ProcessUnixImportPath);
	DECLARE_HANDLER(ProcessLinuxImportPath);	
	DECLARE_HANDLER(ProcessMacOsXImportPath);
	DECLARE_HANDLER(ProcessAddCppFile);
	DECLARE_HANDLER(ProcessUnixCompilerFlags);
	DECLARE_HANDLER(ProcessLinuxCompilerFlags);	
	DECLARE_HANDLER(ProcessMacOsXCompilerFlags);
	DECLARE_HANDLER(ProcessCppEntryPoint);
};