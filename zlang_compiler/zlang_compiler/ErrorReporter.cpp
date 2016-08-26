
#include "Common.h"
#include "ErrorReporter.h"
#include "CodeTemplates.h"
#include "Utils.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h>

CCodeTemplate* LoadTpl(const std::string& name)
{
	CPath path("errors", name + ".tpl");
	CCodeTemplate* tpl = CCodeTemplates::GetInstance()->LoadTemplate(path);
	assert(tpl);
	return tpl;
}

CCodeTemplate* LoadWarningTpl(const std::string& name)
{
	CPath nameWithPath("warnings", name);
	return LoadTpl(nameWithPath);
}

CErrorReporter::CErrorReporter():
	m_disableShowFileNames(false),
	m_fatalErrorCount(0),
	m_isEnabled(true)
{
	m_commonError = LoadTpl("common");
	m_globalError = LoadTpl("global");
	m_commonWarning = LoadTpl("commonWarning");

	memset(m_errors, 0, sizeof(m_errors[0]) * COMPILER_ERROR_COUNT);

	m_errors[CANNOT_OPEN_SOURCE_FILE] = LoadTpl("cannotOpenSourceFile");
	m_errors[CANNOT_MODIFY_SOURCE_FILE] = LoadTpl("cannotModifySourceFile");
	m_errors[INPUT_FILE_NOT_FOUND] = LoadTpl("inputFileNotFound");
	m_errors[LEXEMS_WERE_UNRECOGNIZED] = LoadTpl("lexemsWereUnrecognized");
	m_errors[SYNTAX_TREE_BUILD_ERROR] = LoadTpl("syntaxTreeBuildError");
	m_errors[LEX_ERROR] = LoadTpl("lexError");
	m_errors[YACC_ERROR] = LoadTpl("yaccError");
	m_errors[OUTPUT_FILE_ERROR] = LoadTpl("outputFileError");
	m_errors[OUTPUT_DIR_NOT_EXISTS] = LoadTpl("outputDirNotExists");
	m_errors[NO_TEMPLATES] = LoadTpl("noTemplates");	
	m_errors[OUTPUT_DIR_NOT_WRITEABLE] = LoadTpl("outputDirNotWriteable");	
	m_errors[FILE_WRITE_ERROR] = LoadTpl("fileWriteError");	
	m_errors[CANNOT_WRITE_TO_TEMPORARY_FILE] = LoadTpl("cannotWriteToTempFile");	
	m_errors[CANNOT_READ_FROM_TEMPORARY_FILE] = LoadTpl("cannotReadFromTempFile");	

	m_errors[UNINITIALIZED_VARIABLE] = LoadTpl("uninitializedVariable");
	m_errors[VARIABLE_WITH_METHOD_NAME] = LoadTpl("variableWithMethodName");
	m_errors[CALL_NON_STANDARD_FUNCTION] = LoadTpl("callNonStandardFunction");
	m_errors[CREATE_SINGLETON_INSTANCE] = LoadTpl("createSingletonInstance");
	m_errors[DIVISION_BY_ZERO] = LoadTpl("divisionByZero");
	m_errors[INVALID_PARAMETER_COUNT] = LoadTpl("invalidParameterCount");
	m_errors[METHOD_WITH_CLASS_NAME] = LoadTpl("methodWithClassName");
	m_errors[MULTIPLE_METHODS_WITH_ONE_NAME] = LoadTpl("multipleMethodsWithOneName");
	m_errors[MULTIPLE_FUNCTIONS_WITH_ONE_NAME] = LoadTpl("multipleFunctionsWithOneName");
	m_errors[MULTIPLE_PROPERTIES_WITH_ONE_NAME] = LoadTpl("multiplePropertiesWithOneName");
	m_errors[NO_ENTRY_POINT] = LoadTpl("noEntryPoint");
	m_errors[INVALID_ENTRY_POINT] = LoadTpl("invalidEntryPoint");
	m_errors[UNKNOWN_METHOD_CALL] = LoadTpl("unknownMethodCall");
	m_errors[UNKNOWN_CLASS] = LoadTpl("unknownClass");
	m_errors[VARIABLE_WITH_CLASS_NAME] = LoadTpl("variableWithClassName");
	m_errors[VARIABLE_WITH_STANDARD_FUNCTION_NAME] = LoadTpl("variableWithStandardFunctionName");
	m_errors[PARAMETER_WITH_CLASS_PROPERTY_NAME] = LoadTpl("parameterWithClassPropertyName");	
	m_errors[UNKNOWN_METHOD_CALL_IN_SINGLETON] = LoadTpl("unknownMethodCallInSingleton");
	m_errors[PRIVATE_METHOD_CALL] = LoadTpl("privateMethodCall");
	m_errors[INVALID_PARAMETERS_METHOD_CALL] = LoadTpl("invalidParametersMethodCall");
	m_errors[UNKNOWN_METHOD_CALL_INSIDE_CLASS] = LoadTpl("unknownMethodCallInsideClass");
	m_errors[INVALID_PARAMETERS_PUBLIC_METHOD_CALL] = LoadTpl("invalidParametersPublicMethodCall");
	m_errors[ATTEMPT_TO_USE_PRIVATE_PROP_SINGLETON] = LoadTpl("attemptToUsePrivatePropSingleton");	
	m_errors[ATTEMPT_TO_USE_METHOD_AS_PROPERTY] = LoadTpl("attemptToUseMethodAsProperty");
	m_errors[ATTEMPT_TO_USE_CONST_AS_OUT_PARAMETER] = LoadTpl("attemptToUseConstAsOutParameter");
	m_errors[BASE_CLASS_UNDEFINED] = LoadTpl("baseClassUndefined");
	m_errors[SECOND_CLASS_DECLARATION] = LoadTpl("secondClassDeclaration");
	m_errors[CALL_TO_NON_EXIST_BASE_CLASS] = LoadTpl("callToNonExistBaseClass");
	m_errors[EXTERNAL_CPP_COMPILER_EXECUTE_ERROR] = LoadTpl("externalCppCompilerExecuteError");
	m_errors[MSBUILD_NOT_FOUND] = LoadTpl("MSBuildNotFound");

	m_errors[WARNING_USE_POSSIBLE_UNINITIALIZED_OUT_PARAMETER] = LoadWarningTpl("usePossibleUninitializedOutParameter");
	m_errors[WRONG_ATTEMPT_TO_ADD_TO_ARRAY] = LoadTpl("wrongAttemptToAddToArray");
	m_errors[INCORRECT_ELSE] = LoadTpl("incorrectElse");
	m_errors[INCORRECT_IF] = LoadTpl("incorrectIf");
	m_errors[INCORRECT_BREAK] = LoadTpl("incorrectBreak");
	m_errors[INCORRECT_CONTINUE] = LoadTpl("incorrectContinue");
	m_errors[USE_CYCLE_VAR_IN_SECOND_FOREACH] = LoadTpl("useCycleVarInSecondForeach");
	m_errors[USE_EQUAL_HASHTABLE_AND_CYCLE_VAR] = LoadTpl("useEqualHashtableAndCycleVar");
	m_errors[ATTEMPT_TO_USE_RESERVED_KEYWORD] = LoadTpl("attemptToUseReservedKeyword");
	m_errors[ATTEMPT_TO_RETURN_UNKNOWN_VARIABLE] = LoadTpl("attemptToReturnUnknownVariable");

	m_errors[ANNOTATION_FOR_NON_USED_PARAMETER] = LoadTpl("annotationForNonUsedParameter");
	m_errors[ANNOTATION_INVALID_PARAMETER] = LoadTpl("annotationInvalidParameter");
	m_errors[ANNOTATION_INVALID_VALIDATOR] = LoadTpl("annotationInvalidValidator");

	m_errors[BASE_CLASS_CANNOT_BE_USED] = LoadTpl("baseClassCanNotBeUsed");
	m_errors[GLOBAL_FUNCTION_CANNOT_BE_REPLACED_BY_FUNCTION_POINTER] = LoadTpl("globalFunctionCannotBeReplacedByFunctionPointer");

	m_errors[PRECISION_MUST_BE_POSITIVE_INT] = LoadTpl("precisionMustBePositiveInt");
	
	for (unsigned int i = 0; i < COMPILER_ERROR_COUNT; i++)
	{		
		assert(m_errors[i]);
	}	
}

CErrorReporter::CErrorReporter(const CErrorReporter& other):
	m_fatalErrorCount(0),
	m_isEnabled(true)
{
	assert(false);
}

CErrorReporter* CErrorReporter::GetInstance()
{
	static CErrorReporter* obj = NULL;

	if (!obj)
	{
		obj = new CErrorReporter();
	}

	return obj;
}

std::string CErrorReporter::PatchYaccError(const std::string& txt)
{
	std::string result = txt;

	StringReplace(result, "COMMA", "','");
	StringReplace(result, "COLON", "':'");
	StringReplace(result, "END_PART", "';'");
	StringReplace(result, "FP_NUMBER", "FLOATING POINT NUMBER");
	StringReplace(result, "OUT_SPEC", "OUT PARAMETER QUALIFIER");
	StringReplace(result, "PARENS_START", "'('");
	StringReplace(result, "PARENS_END", "')'");
	StringReplace(result, "VAL_TRUE", "TRUE");
	StringReplace(result, "VAL_FALSE", "FALSE");
	StringReplace(result, "ID_FUNC", "NAME OF FUNCTION");
	StringReplace(result, "ID_CLASS", "NAME OF CLASS");
	StringReplace(result, "CODE_LEVEL3", "THREE TABS");
	StringReplace(result, "CODE_LEVEL", "TAB");
	StringReplace(result, "SQUARE_BRACKET_START", "'['");
	StringReplace(result, "SQUARE_BRACKET_END", "']'");

	StringReplace(result, "_", " ");

	return result;
}

bool CErrorReporter::GetErrorLineInfo(unsigned int& line, std::string* sourceFilePath)
{
	std::vector<SProjectFile *>::iterator it = m_projectFiles.begin();
	std::vector<SProjectFile *>::iterator itEnd = m_projectFiles.end();

	for ( ; it != itEnd; it++)
	if ((line >= (*it)->lineStart) && (line < (*it)->lineStart + (*it)->linesCount))
	{
		line = line - (*it)->lineStart;
		if (sourceFilePath)
		{
			*sourceFilePath = (*it)->fullPath + (*it)->name;
		}
		break;
	}

	if ((m_disableShowFileNames) && (sourceFilePath))
	{
		*sourceFilePath = "<some file>";
	}

	return true;
}

void CErrorReporter::OnWarning(const CompilerError error, const unsigned int line, const std::string& variableName, const std::string& otherName)
{
	if (!m_isEnabled)
	{
		return;
	}

	static unsigned int lineIndex = m_commonWarning->GetVariableIndex("line");
	static unsigned int filePathIndex = m_commonWarning->GetVariableIndex("filePath");
	m_commonWarning->SetVariableValue(0, line);

	std::string filePath = "";
	unsigned int lineInFile = line;
	if (!GetErrorLineInfo(lineInFile, &filePath))
	{	
		assert(false);
	}
	m_commonWarning->SetVariableValue(lineIndex, lineInFile);
	m_commonWarning->SetVariableValue(filePathIndex, filePath);
	printf("%s", m_commonWarning->GetText().c_str());
	
	unsigned int variableNameIndex = m_errors[error]->GetVariableIndex("variableName");
	if (variableNameIndex != 0xFFFFFFFF)
	{
		m_errors[error]->SetVariableValue("variableName", variableName);
	}
	unsigned int otherNameIndex = m_errors[error]->GetVariableIndex("secondName");
	if (otherNameIndex != 0xFFFFFFFF)
	{
		m_errors[error]->SetVariableValue("secondName", otherName);
	}
	printf("%s", m_errors[error]->GetText().c_str());
}

void CErrorReporter::DisableShowFileNames()
{
	m_disableShowFileNames = true;
}

void CErrorReporter::OnFatalError(const CompilerError error, const unsigned int line, const std::string& variableName, const std::string& otherName)
{
	if (!m_isEnabled)
	{
		return;
	} 

	m_fatalErrorCount++;	

	if (line != 0xFFFFFFFF)
	{
		static unsigned int lineIndex = m_commonError->GetVariableIndex("line");
		static unsigned int filePathIndex = m_commonError->GetVariableIndex("filePath");
		m_commonError->SetVariableValue(0, line);

		std::string filePath = "";
		unsigned int lineInFile = line;
		if (!GetErrorLineInfo(lineInFile, &filePath))
		{	
			assert(false);
		}		
		m_commonError->SetVariableValue(lineIndex, lineInFile);
		m_commonError->SetVariableValue(filePathIndex, filePath);
		printf("%s", m_commonError->GetText().c_str());
	}
	else
	{		
		printf("%s", m_globalError->GetText().c_str());
	}

	unsigned int variableNameIndex = m_errors[error]->GetVariableIndex("variableName");
	if (variableNameIndex != 0xFFFFFFFF)
	{
		m_errors[error]->SetVariableValue("variableName", variableName);
	}
	unsigned int otherNameIndex = m_errors[error]->GetVariableIndex("secondName");
	if (otherNameIndex != 0xFFFFFFFF)
	{
		m_errors[error]->SetVariableValue("secondName", otherName);
	}
	printf("%s", m_errors[error]->GetText().c_str());
}

void CErrorReporter::SetProjectFilesInfo(const std::vector<SProjectFile *>& info)
{
	m_projectFiles = info;
}

bool CErrorReporter::IsAnyFatalErrorExists()const
{
	return (m_fatalErrorCount > 0);
}

void CErrorReporter::EnableErrorsHandling(const bool isEnabled)
{
	m_isEnabled = isEnabled;
}

void CErrorReporter::PrintCompileStatus()
{
	if (m_fatalErrorCount > 0)
	{
		printf("Code generation was FAILED with fatal errors: %d\n\n", m_fatalErrorCount);
	}
	else
	{
		printf("Compilation was ended successfully\n\n");
	}
}

void CErrorReporter::operator =(const CErrorReporter& other)
{
	assert(false);
}

CErrorReporter::~CErrorReporter()
{
}
