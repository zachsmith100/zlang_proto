
#include "Common.h"
#include "StandardFunctions.h"
#include "ErrorReporter.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"

CStandardFunctions::CStandardFunctions()
{	
	Register("unset", 1, false);
	Register("typeof", 1, false);
	Register("parseInt", 1, false);
	Register("parseFloat", 1, false);
	Register("parseBoolean", 1, false);
	Register("print", 1, false);
	FunctionDesc* printDesc = Register("print", 2, false);
	printDesc->SetAdditionalParameters(false, false, false, true);
	printDesc = Register("print", 3, false);
	printDesc->SetAdditionalParameters(false, false, false, true);
	Register("println", 1, false);
	printDesc = Register("println", 2, false);
	printDesc->SetAdditionalParameters(false, false, false, true);
	printDesc = Register("println", 3, false);
	printDesc->SetAdditionalParameters(false, false, false, true);
	Register("read", 1, true);
	Register("read", 2, true);
	Register("read", 3, true);
	Register("append", 1, false);
	Register("append", 2, false);
	Register("openStream", 2, false);
	Register("createStream", 2, false);
	Register("clearStream", 1, false);
	Register("seekStream", 2, false);
	FunctionDesc* assertDesc = Register("ASSERT", 1, false);
	assertDesc->SetAdditionalParameters(true, true, true, false);
	assertDesc = Register("ASSERT_EXT", 2, false);
	assertDesc->SetAdditionalParameters(true, true, true, false);
}

CStandardFunctions::CStandardFunctions(const CStandardFunctions&)
{
	assert(false);
}

CStandardFunctions* CStandardFunctions::GetInstance()
{
	static CStandardFunctions* obj = NULL;
	if (!obj)
	{
		obj = new CStandardFunctions();
	}
	return obj;
}

FunctionDesc* CStandardFunctions::Register(const std::string& id, const int parametersCount, const bool isOutParametersUsed, const bool isGlobalNamespaceNeeded, const bool isArgumentCountAdditionNeeded)
{
	FunctionsMap::iterator it = m_functions.find(id);
	if (it != m_functions.end())
	{
		(*it).second.parameters.push_back(parametersCount);		
	}
	else
	{
		std::vector<unsigned int> parametersCountArr;
		parametersCountArr.push_back(parametersCount);
		FunctionDesc desc(parametersCountArr, isOutParametersUsed);
		m_functions.insert(FunctionsMap::value_type(id, desc));
		it = m_functions.find(id);
	}
	return &(*it).second;
}

bool CStandardFunctions::IsFunctionExists(const std::string& id)const
{
	FunctionsMap::const_iterator it = m_functions.find(id);
	return (it != m_functions.end());
}

bool CStandardFunctions::ValidateFunctionParametersCount(const std::string& id, const unsigned int parametersCount)const
{
	FunctionsMap::const_iterator it = m_functions.find(id);
	if (it == m_functions.end())
	{
		return false;
	}

	std::vector<unsigned int>::const_iterator paramsIt = find((*it).second.parameters.begin(), (*it).second.parameters.end(), parametersCount);
	
	return (paramsIt != (*it).second.parameters.end());
}

void CStandardFunctions::operator =(const CStandardFunctions&)
{
	assert(false);
}

std::string CStandardFunctions::GetExpectedParameters(const std::string& id)const
{
	FunctionsMap::const_iterator it = m_functions.find(id);
	if (it == m_functions.end())
	{
		return "";
	}

	std::string result = "";

	std::vector<unsigned int>::const_iterator pit = (*it).second.parameters.begin();
	std::vector<unsigned int>::const_iterator pitEnd = (*it).second.parameters.end();

	for ( ; pit != pitEnd; pit++)
	{
		result += IntToStr(*pit)+",";
	}

	result.erase(result.begin() + result.size() - 1);

	return result;
}

bool CStandardFunctions::IsOutParametersUsed(const std::string& id)const
{
	FunctionsMap::const_iterator it = m_functions.find(id);
	if (it == m_functions.end())
	{
		return false;
	}

	return (*it).second.isOutParametersUsed;
}

std::string CStandardFunctions::GetAdditionalParameters(const std::string& id, node* nd)const
{
	std::string code = "";

	FunctionsMap::const_iterator it = m_functions.find(id);
	if (it == m_functions.end())
	{
		return "";
	}	 

	if (((*it).second.isLineParamNeeded) || ((*it).second.isFileParamNeeded))
	{
		std::string filePath;
		unsigned int line = nd->line;
		CErrorReporter::GetInstance()->GetErrorLineInfo(line, &filePath);

		if ((*it).second.isFileParamNeeded)
		{
			code += ", \"" + GetPathAsStringConst(filePath) + "\"";
		}

		if ((*it).second.isFileParamNeeded)
		{
			code += ", " + IntToStr(line);
		}
	}

	return code;
}

std::string CStandardFunctions::PatchFunctionName(const std::string& name, const unsigned int argumentCount)const
{
	FunctionsMap::const_iterator it = m_functions.find(name);
	if (it == m_functions.end())
	{
		return name;
	}

	std::string result = name;
	if (it->second.isGlobalNamespaceNeeded)
	{
		result = "::" + result;
	}

	if ((it->second.isArgumentCountAdditionNeeded) && (argumentCount != 1))
	{
		result = result + IntToStr(argumentCount);
	}

	return result;
}

CStandardFunctions::~CStandardFunctions()
{
}