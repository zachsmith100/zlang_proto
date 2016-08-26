
#include "CppInsertCodeGenerator.h"
#include "Common.h"
#include "CodeTemplates.h"
#include "ImportManager.h"
#include "Classes.h"
#include "Utils.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CCppInsertCodeGenerator)

PROCESS_TOKENS_START(CCppInsertCodeGenerator)
	PROCESS_TOKEN(OP_CPP, ProcessCppInsert);
	PROCESS_TOKEN(OP_CPP_DECL, ProcessCppDeclaration);
	PROCESS_TOKEN(OP_INCLUDE, ProcessInclude);
	PROCESS_TOKEN(OP_INCLUDE_WINDOWS, ProcessWindowsInclude);
	PROCESS_TOKEN(OP_INCLUDE_LINUX, ProcessLinuxInclude);
	PROCESS_TOKEN(OP_INCLUDE_MACOSX, ProcessMacOsXInclude);		
	PROCESS_TOKEN(OP_INCLUDE_UNIX, ProcessUnixInclude);		
	PROCESS_TOKEN(OP_IMPORT_WINDOWS, ProcessWindowsImport);
	PROCESS_TOKEN(OP_IMPORT_UNIX, ProcessUnixImport);	
	PROCESS_TOKEN(OP_IMPORT_LINUX, ProcessLinuxImport);
	PROCESS_TOKEN(OP_IMPORT_MACOSX, ProcessMacOsXImport);	
	PROCESS_TOKEN(OP_INCLUDE_WINDOWS_PATH, ProcessWindowsIncludePath);	
	PROCESS_TOKEN(OP_INCLUDE_UNIX_PATH, ProcessUnixIncludePath);		
	PROCESS_TOKEN(OP_INCLUDE_LINUX_PATH, ProcessLinuxIncludePath);	
	PROCESS_TOKEN(OP_INCLUDE_MACOSX_PATH, ProcessMacOsXIncludePath);		
	PROCESS_TOKEN(OP_INCLUDE_PATH, ProcessIncludePath);		
	PROCESS_TOKEN(OP_IMPORT_WINDOWS_PATH, ProcessWindowsImportPath);
	PROCESS_TOKEN(OP_IMPORT_UNIX_PATH, ProcessUnixImportPath);	
	PROCESS_TOKEN(OP_IMPORT_LINUX_PATH, ProcessLinuxImportPath);
	PROCESS_TOKEN(OP_IMPORT_MACOSX_PATH, ProcessMacOsXImportPath);	
	PROCESS_TOKEN(OP_COMPILER_FLAGS_UNIX, ProcessUnixCompilerFlags);
	PROCESS_TOKEN(OP_COMPILER_FLAGS_LINUX, ProcessLinuxCompilerFlags);
	PROCESS_TOKEN(OP_COMPILER_FLAGS_MACOSX, ProcessMacOsXCompilerFlags);
	PROCESS_TOKEN(OP_ADD_CPP, ProcessAddCppFile);
	PROCESS_TOKEN(OP_CPP_ENTRY_POINT, ProcessCppEntryPoint);
PROCESS_TOKENS_END

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessCppInsert)
{
	CHANGE_STATE(newVariablesProhibited, true);
	std::string code = nd->left->token;
	CHANGE_STATE(newVariablesProhibited, false);
	StringReplace(code, "cpp@", "");
	StringReplace(code, "@cpp", "");
	StringReplace(code, "\n\t\t\t", "\n\t\t");
	TrimTabsAndSpaces(code);
	if (nd->tabLevel != 0)
	{
		code = "{" + code;
	}
	for (int i = 0; i < nd->tabLevel - 2; i++)
		code += "\t";
	if (nd->tabLevel != 0)
	{
		code += "}";
	}
	return code;
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessCppDeclaration)
{
	CHANGE_STATE(newVariablesProhibited, true);
	std::string code = nd->left->token;
	CHANGE_STATE(newVariablesProhibited, false);	
	StringReplace(code, "cpp@", "");
	StringReplace(code, "@cpp", "");	
	StringReplace(code, "\n\t\t\t", "\n\t");
	TrimTabsAndSpaces(code);	
	for (int i = 0; i < nd->tabLevel - 3; i++)
		code += "\t";	
	Classes::GetInstance()->GetCurrentClass()->AddCppDeclaration(code);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessInclude)
{
	CImportManager::GetInstance()->AddInclude(nd->left->token, OS_ALL);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessCppEntryPoint)
{
	CImportManager::GetInstance()->SetEntryPoint(nd->left->token);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessWindowsInclude)
{
	CImportManager::GetInstance()->AddInclude(nd->left->token, OS_WINDOWS);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessLinuxInclude)
{
	CImportManager::GetInstance()->AddInclude(nd->left->token, OS_LINUX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessMacOsXInclude)
{
	CImportManager::GetInstance()->AddInclude(nd->left->token, OS_MACOSX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessUnixInclude)
{
	CImportManager::GetInstance()->AddInclude(nd->left->token, OS_UNIX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessWindowsIncludePath)
{
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_WINDOWS);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessUnixIncludePath)
{
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_UNIX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessLinuxIncludePath)
{
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_LINUX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessIncludePath)
{
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_UNIX);
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_WINDOWS);	
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessMacOsXIncludePath)
{
	CImportManager::GetInstance()->AddIncludePath(nd->left->token, OS_MACOSX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessWindowsImport)
{
	if (!nd->center)
	{
		CImportManager::GetInstance()->AddImport(nd->left->token, OS_WINDOWS);
	}
	else
	{
		CImportManager::GetInstance()->AddConditionalImport(nd->left->token, nd->center->token, OS_WINDOWS);
	}
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessUnixImport)
{
	CImportManager::GetInstance()->AddImport(nd->left->token, OS_UNIX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessLinuxImport)
{
	CImportManager::GetInstance()->AddImport(nd->left->token, OS_LINUX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessMacOsXImport)
{
	CImportManager::GetInstance()->AddImport(nd->left->token, OS_MACOSX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessWindowsImportPath)
{
	CImportManager::GetInstance()->AddImportPath(nd->left->token, OS_WINDOWS);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessUnixImportPath)
{
	CImportManager::GetInstance()->AddImportPath(nd->left->token, OS_UNIX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessLinuxImportPath)
{
	CImportManager::GetInstance()->AddImportPath(nd->left->token, OS_LINUX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessMacOsXImportPath)
{
	CImportManager::GetInstance()->AddImportPath(nd->left->token, OS_MACOSX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessUnixCompilerFlags)
{
	CImportManager::GetInstance()->AddCompilerFlags(nd->left->token, OS_UNIX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessLinuxCompilerFlags)
{
	CImportManager::GetInstance()->AddCompilerFlags(nd->left->token, OS_LINUX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessMacOsXCompilerFlags)
{
	CImportManager::GetInstance()->AddCompilerFlags(nd->left->token, OS_MACOSX);
	return "";
}

DECLARE_HANDLER(CCppInsertCodeGenerator::ProcessAddCppFile)
{	
	CImportManager::GetInstance()->AddCppFile(nd->left->token);
	return "";
}

std::vector<int> CCppInsertCodeGenerator::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(OP_CPP);
	res.push_back(OP_CPP_DECL);
	res.push_back(OP_INCLUDE);
	res.push_back(OP_INCLUDE_LINUX);
	res.push_back(OP_INCLUDE_WINDOWS);
	res.push_back(OP_INCLUDE_MACOSX);
	res.push_back(OP_INCLUDE_UNIX);
	res.push_back(OP_IMPORT_UNIX);
	res.push_back(OP_IMPORT_WINDOWS);	
	res.push_back(OP_IMPORT_LINUX);
	res.push_back(OP_IMPORT_MACOSX);	
	res.push_back(OP_INCLUDE_WINDOWS_PATH);	
	res.push_back(OP_INCLUDE_UNIX_PATH);
	res.push_back(OP_INCLUDE_LINUX_PATH);	
	res.push_back(OP_INCLUDE_MACOSX_PATH);
	res.push_back(OP_INCLUDE_PATH);	
	res.push_back(OP_IMPORT_UNIX_PATH);
	res.push_back(OP_IMPORT_WINDOWS_PATH);	
	res.push_back(OP_IMPORT_LINUX_PATH);
	res.push_back(OP_IMPORT_MACOSX_PATH);
	res.push_back(OP_COMPILER_FLAGS_UNIX);
	res.push_back(OP_COMPILER_FLAGS_LINUX);
	res.push_back(OP_COMPILER_FLAGS_MACOSX);
	res.push_back(OP_ADD_CPP);
	res.push_back(OP_CPP_ENTRY_POINT);	
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CCppInsertCodeGenerator)


