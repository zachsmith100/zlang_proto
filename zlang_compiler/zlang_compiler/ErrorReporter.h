
#pragma once

#include <string>
#include <vector>
#include "CodeTemplate.h"
#include "ProjectCompiler.h"

enum CompilerError
{
	CANNOT_OPEN_SOURCE_FILE = 0,
	CANNOT_MODIFY_SOURCE_FILE,
	INPUT_FILE_NOT_FOUND,
	LEX_ERROR,
	YACC_ERROR,
	LEXEMS_WERE_UNRECOGNIZED,
	SYNTAX_TREE_BUILD_ERROR,
	OUTPUT_FILE_ERROR,
	OUTPUT_DIR_NOT_EXISTS,
	CANNOT_WRITE_TO_TEMPORARY_FILE,
	CANNOT_READ_FROM_TEMPORARY_FILE,
	FILE_WRITE_ERROR,
	NO_TEMPLATES,
	OUTPUT_DIR_NOT_WRITEABLE,
	UNINITIALIZED_VARIABLE,
	VARIABLE_WITH_METHOD_NAME,
	CALL_NON_STANDARD_FUNCTION,
	CREATE_SINGLETON_INSTANCE,
	DIVISION_BY_ZERO,
	INVALID_PARAMETER_COUNT,
	METHOD_WITH_CLASS_NAME,
	MULTIPLE_METHODS_WITH_ONE_NAME,
	MULTIPLE_PROPERTIES_WITH_ONE_NAME,
	NO_ENTRY_POINT,
	INVALID_ENTRY_POINT,
	UNKNOWN_METHOD_CALL,
	VARIABLE_WITH_CLASS_NAME,
	VARIABLE_WITH_STANDARD_FUNCTION_NAME,
	PARAMETER_WITH_CLASS_PROPERTY_NAME,
	UNKNOWN_METHOD_CALL_IN_SINGLETON,
	PRIVATE_METHOD_CALL,
	INVALID_PARAMETERS_METHOD_CALL,
	UNKNOWN_METHOD_CALL_INSIDE_CLASS,
	INVALID_PARAMETERS_PUBLIC_METHOD_CALL,
	ATTEMPT_TO_USE_PRIVATE_PROP_SINGLETON,	
	ATTEMPT_TO_USE_METHOD_AS_PROPERTY,
	ATTEMPT_TO_USE_CONST_AS_OUT_PARAMETER,
	BASE_CLASS_UNDEFINED,
	SECOND_CLASS_DECLARATION,
	CALL_TO_NON_EXIST_BASE_CLASS,
	EXTERNAL_CPP_COMPILER_EXECUTE_ERROR,	
	MSBUILD_NOT_FOUND,
	WARNING_USE_POSSIBLE_UNINITIALIZED_OUT_PARAMETER,
	WRONG_ATTEMPT_TO_ADD_TO_ARRAY,
	INCORRECT_ELSE,
	INCORRECT_IF,
	INCORRECT_BREAK,
	INCORRECT_CONTINUE,
	USE_CYCLE_VAR_IN_SECOND_FOREACH,
	USE_EQUAL_HASHTABLE_AND_CYCLE_VAR,
	ANNOTATION_FOR_NON_USED_PARAMETER,
	ANNOTATION_INVALID_PARAMETER,
	ANNOTATION_INVALID_VALIDATOR,
	MULTIPLE_FUNCTIONS_WITH_ONE_NAME,
	ATTEMPT_TO_USE_RESERVED_KEYWORD,
	ATTEMPT_TO_RETURN_UNKNOWN_VARIABLE,
	UNKNOWN_CLASS,
	BASE_CLASS_CANNOT_BE_USED,
	GLOBAL_FUNCTION_CANNOT_BE_REPLACED_BY_FUNCTION_POINTER,
	PRECISION_MUST_BE_POSITIVE_INT,
	COMPILER_ERROR_COUNT
};

class CErrorReporter
{
public:
	static CErrorReporter* GetInstance();

	void OnFatalError(const CompilerError error, const unsigned int line, const std::string& variableName = "", const std::string& otherName = "");
	void OnWarning(const CompilerError error, const unsigned int line, const std::string& variableName = "", const std::string& otherName = "");

	bool IsAnyFatalErrorExists()const;

	void PrintCompileStatus();
	void DisableShowFileNames();

	void EnableErrorsHandling(const bool isEnabled);

	void SetProjectFilesInfo(const std::vector<SProjectFile *>& info);
	bool GetErrorLineInfo(unsigned int& line, std::string* sourceFilePath);

	std::string PatchYaccError(const std::string& txt);

private:
	CErrorReporter();
	CErrorReporter(const CErrorReporter& other);
	void operator =(const CErrorReporter& other);
	~CErrorReporter();

	CCodeTemplate* m_commonError;
	CCodeTemplate* m_globalError;
	CCodeTemplate* m_commonWarning;
	CCodeTemplate* m_errors[COMPILER_ERROR_COUNT];
	
	unsigned int m_fatalErrorCount;	

	bool m_disableShowFileNames;
	bool m_isEnabled;

	std::vector<SProjectFile *> m_projectFiles;
};




