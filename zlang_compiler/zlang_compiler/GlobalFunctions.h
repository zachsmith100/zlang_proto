
#pragma once

#include <string>
#include <vector>
#include "ClassInfo.h"

struct FunctionInfo
{
	unsigned int line;	
	std::string defaultResultField;
	std::vector<bool> isOutParameter;
	std::vector<MethodParameter> parameters;

	FunctionInfo(const FunctionInfo& other)
	{		
		line = other.line;
		isOutParameter = other.isOutParameter;
		parameters = other.parameters;		
		defaultResultField = other.defaultResultField;
	}

	FunctionInfo(const unsigned int _line, const std::vector<bool>& _isOutParameter, 
		const std::vector<MethodParameter>& _parameters, const std::string& _defaultResultField)
	{
		line = _line;
		isOutParameter = _isOutParameter;
		parameters = _parameters;		
		defaultResultField = _defaultResultField;
	}
};

typedef std::map<std::string, FunctionInfo> GlobalFunctionsMap;

class CGlobalFunctions
{
public:
	static CGlobalFunctions* GetInstance();

	void AddFunction(const std::string& name, const unsigned int line, const std::vector<bool>& isOutParameter, 
		const std::vector<MethodParameter>& parameters, const std::string& defaultResultField);

	bool CheckSecondFunctionDeclaration(const std::string& name, const unsigned int line);

	bool IsFunctionDeclared(const std::string& name)const;

	bool ValidateFunctionParametersCount(const std::string& name, const unsigned int parametersCount)const;
	std::string GetExpectedParameters(const std::string& name)const;

	std::string GetForwardDeclarationCode()const;
	std::string GetNamedGlobalFunctionsSupportCode()const;
	
	bool IsOutFunctionParameter(const std::string& name, const unsigned int parameterNum)const;

	Json::Value GetInfoAsJSONObject()const;

	std::string GetFunctionDefaultResultField(const std::string& name)const;

	void SetCurrentFunction(const std::string& name, const unsigned int line);
	std::string GetCurrentFunction()const;
	unsigned int GetCurrentFunctionDeclarationLine()const;

	void AddGlobalFunctionsToList(std::vector<std::string>& list);

private:
	std::string GetParametersDeclaration(const unsigned int parameterCount)const;
	std::string GetProxyParametersDeclaration(const unsigned int parameterCount)const;

	Json::Value GetFunctionInfoAsJSONObject(GlobalFunctionsMap::const_iterator it)const;
	
	CGlobalFunctions();
	CGlobalFunctions(const CGlobalFunctions& other);
	void operator = (const CGlobalFunctions& other);
	~CGlobalFunctions();

	GlobalFunctionsMap m_map;
	std::string m_currentFunction;
	unsigned int m_currentFunctionDeclarationLine;
};