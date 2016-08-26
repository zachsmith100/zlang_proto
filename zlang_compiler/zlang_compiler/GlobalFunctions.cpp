
#include "GlobalFunctions.h"
#include <assert.h>
#include <stdio.h>
#include "Utils.h"
#include "CodeTemplates.h"

CGlobalFunctions::CGlobalFunctions():
	m_currentFunctionDeclarationLine(0)
{
}

CGlobalFunctions::CGlobalFunctions(const CGlobalFunctions& other):
	m_currentFunctionDeclarationLine(0)
{
	assert(false);
}

CGlobalFunctions* CGlobalFunctions::GetInstance()
{
	static CGlobalFunctions* obj = NULL;

	if (!obj)
	{
		obj = new CGlobalFunctions();
	}

	return obj;
}

void CGlobalFunctions::SetCurrentFunction(const std::string& name, const unsigned int line)
{
	m_currentFunction = name;
	m_currentFunctionDeclarationLine = line;
}

std::string CGlobalFunctions::GetCurrentFunction()const
{
	return m_currentFunction;
}

unsigned int CGlobalFunctions::GetCurrentFunctionDeclarationLine()const
{
	return m_currentFunctionDeclarationLine;
}

std::string CGlobalFunctions::GetFunctionDefaultResultField(const std::string& name)const
{
	GlobalFunctionsMap::const_iterator it = m_map.find(name);
	if (it == m_map.end())
	{
		return "";
	}

	return (*it).second.defaultResultField;
}

void CGlobalFunctions::AddFunction(const std::string& name, const unsigned int line, const std::vector<bool>& isOutParameter, 
		const std::vector<MethodParameter>& parameters, const std::string& defaultResultField)
{
	GlobalFunctionsMap::iterator it = m_map.find(name);
	if (it != m_map.end())
	{
		m_map.erase(it);
	}

	FunctionInfo info(line, isOutParameter, parameters, defaultResultField);
	m_map.insert(GlobalFunctionsMap::value_type(name, info));
}

bool CGlobalFunctions::CheckSecondFunctionDeclaration(const std::string& name, const unsigned int line)
{
	GlobalFunctionsMap::iterator it = m_map.find(name);
	if (it == m_map.end())
	{
		return false;
	}

	return ((*it).second.line != line);
}

void CGlobalFunctions::operator = (const CGlobalFunctions& other)
{
	assert(false);
}

bool CGlobalFunctions::IsFunctionDeclared(const std::string& name)const
{
	GlobalFunctionsMap::const_iterator it = m_map.find(name);
	return (it != m_map.end());
}

bool CGlobalFunctions::ValidateFunctionParametersCount(const std::string& name, const unsigned int parametersCount)const
{
	GlobalFunctionsMap::const_iterator it = m_map.find(name);
	if (it == m_map.end())
	{
		return false;
	}

	return ((*it).second.isOutParameter.size() == parametersCount);
}

std::string CGlobalFunctions::GetExpectedParameters(const std::string& name)const
{
	GlobalFunctionsMap::const_iterator it = m_map.find(name);
	if (it == m_map.end())
	{
		return "";
	}

	int paramCount = (*it).second.parameters.size();
	return IntToStr(paramCount);
}

std::string CGlobalFunctions::GetParametersDeclaration(const unsigned int parameterCount)const
{
	std::string code = "";
	for (unsigned int i = 0; i < parameterCount; i++)
	{
		ADD_FROM_TPL(functions, forwardDeclarationParameter, code);
	}	
	return code;
}

std::string CGlobalFunctions::GetProxyParametersDeclaration(const unsigned int parameterCount)const
{
	std::string code = "";
	for (unsigned int i = 0; i < parameterCount; i++)
	{
		ADD_FROM_TPL(functions, forwardDeclarationProxyParameter, code);
	}	
	return code;
}

std::string CGlobalFunctions::GetForwardDeclarationCode()const
{
	std::string code = "";

	GlobalFunctionsMap::const_iterator it = m_map.begin();
	GlobalFunctionsMap::const_iterator itEnd = m_map.end();

	for ( ; it != itEnd; it++)
	{
		USE_TPL(functions, forwardDeclaration);
		USE_INDEX(forwardDeclaration, functionName);
		USE_INDEX(forwardDeclaration, parameters);
		USE_INDEX(forwardDeclaration, proxyParameters);

		forwardDeclarationTpl->SetVariableValue(functionNameIndex, (*it).first);
		forwardDeclarationTpl->SetVariableValue(parametersIndex, GetParametersDeclaration((*it).second.isOutParameter.size()));
		forwardDeclarationTpl->SetVariableValue(proxyParametersIndex, GetProxyParametersDeclaration((*it).second.isOutParameter.size()));
	
		code += forwardDeclarationTpl->GetText();
	}

	return code;
}

bool CGlobalFunctions::IsOutFunctionParameter(const std::string& name, const unsigned int parameterNum)const
{
	GlobalFunctionsMap::const_iterator it = m_map.find(name);
	if (it == m_map.end())
	{
		return false;
	}
	
	if (parameterNum >= (*it).second.isOutParameter.size())
	{
		return false;

	}
	
	return (*it).second.isOutParameter[parameterNum];
}

Json::Value CGlobalFunctions::GetFunctionInfoAsJSONObject(GlobalFunctionsMap::const_iterator it)const
{
	Json::Value functionObject;
	functionObject["name"] = (*it).first;
	
	Json::Value paramsObject;
	paramsObject.resize((*it).second.parameters.size());

	std::vector<MethodParameter>::const_iterator pit = (*it).second.parameters.begin();
	std::vector<MethodParameter>::const_iterator pitEnd = (*it).second.parameters.end();

	for (int i = 0; pit != pitEnd; pit++, i++)	
	{
		paramsObject[i] = CClassInfo::GetParameterInfoAsJSONValue(pit);
	}

	functionObject["parameters"] = paramsObject;
	
	return functionObject;
}

Json::Value CGlobalFunctions::GetInfoAsJSONObject()const
{
	Json::Value functionsArray;	
	functionsArray.resize(m_map.size());

	GlobalFunctionsMap::const_iterator mit = m_map.begin();
	GlobalFunctionsMap::const_iterator mitEnd = m_map.end();

	for (int i = 0; mit != mitEnd; mit++, i++)	
	{
		functionsArray[i] = GetFunctionInfoAsJSONObject(mit);
	}

	return functionsArray;
}

std::string CGlobalFunctions::GetNamedGlobalFunctionsSupportCode()const
{
	std::string code = "";

	ValuesList functions;
	GlobalFunctionsMap::const_iterator it = m_map.begin();
	GlobalFunctionsMap::const_iterator itEnd = m_map.end();

	for ( ; it != itEnd; it++)
	{
		functions.push_back((*it).first);
	}

	USE_TPL(functions, functionExistFunc);
	USE_INDEX(functionExistFunc, names);
	functionExistFuncTpl->SetVariableValue(namesIndex, functions); 
	code += functionExistFuncTpl->GetText();

	ADD_FROM_TPL(functions, functionCallByNameHeader, code);		
			
	it = m_map.begin();

	std::vector<bool> isOutParameter;
	std::vector<MethodParameter> parameters;

	for ( ; it != itEnd; it++)
	{
		unsigned int paramCount = (*it).second.isOutParameter.size();

		USE_TPL(functions, functionCallByNameItem);
		USE_INDEX(functionCallByNameItem, paramCount);
		USE_INDEX(functionCallByNameItem, initVar);
		USE_INDEX(functionCallByNameItem, param);
		USE_INDEX(functionCallByNameItem, methodName);
		
		functionCallByNameItemTpl->SetVariableValue(paramCountIndex, paramCount);
		functionCallByNameItemTpl->SetVariableValue(methodNameIndex, (*it).first);
		functionCallByNameItemTpl->SetVariableRange(initVarIndex, 0, paramCount - 1); 
		functionCallByNameItemTpl->SetVariableRange(paramIndex, 0, paramCount - 1, false); 
		
		code += functionCallByNameItemTpl->GetText();
	}

	ADD_FROM_TPL(functions, functionCallByNameFooter, code);	

	return code;
}

void CGlobalFunctions::AddGlobalFunctionsToList(std::vector<std::string>& list)
{
	GlobalFunctionsMap::const_iterator it = m_map.begin();
	GlobalFunctionsMap::const_iterator itEnd = m_map.end();

	for ( ; it != itEnd; it++)
	{
		std::string name = (*it).first;

		std::vector<std::string>::iterator itCheck = find(list.begin(), list.end(), name);
		if (itCheck == list.end())
		{
			list.push_back(name);
		}
	}
}

CGlobalFunctions::~CGlobalFunctions()
{
}