
#pragma once

#include <map>
#include <vector>
#include <string>
#include "node.h"

struct FunctionDesc
{
	// count of parameters
	std::vector<unsigned int> parameters;
	// must parameters be initialized or not?
	bool isOutParametersUsed;
	bool isLineParamNeeded;
	bool isFileParamNeeded;
	bool isGlobalNamespaceNeeded;
	bool isArgumentCountAdditionNeeded;

	FunctionDesc(const std::vector<unsigned int>& _parameters, const bool _isOutParametersUsed)
	{
		parameters = _parameters;
		isOutParametersUsed = _isOutParametersUsed;
		isLineParamNeeded = false;
		isFileParamNeeded = false;
		isGlobalNamespaceNeeded = true;
		isArgumentCountAdditionNeeded = false;
	}

	FunctionDesc(const FunctionDesc& desc)
	{
		parameters = desc.parameters;
		isOutParametersUsed = desc.isOutParametersUsed;
		isLineParamNeeded = desc.isLineParamNeeded;
		isFileParamNeeded = desc.isFileParamNeeded;
		isGlobalNamespaceNeeded = desc.isGlobalNamespaceNeeded;
		isArgumentCountAdditionNeeded = desc.isArgumentCountAdditionNeeded;
	}

	void SetAdditionalParameters(const bool _isLineParamNeeded, const bool _isFileParamNeeded,
		const bool _isGlobalNamespaceNeeded, const bool _isArgumentCountAdditionNeeded)
	{
		isLineParamNeeded = _isLineParamNeeded;
		isFileParamNeeded = _isFileParamNeeded;
		isGlobalNamespaceNeeded = _isGlobalNamespaceNeeded;
		isArgumentCountAdditionNeeded = _isArgumentCountAdditionNeeded;
	}
};

typedef std::map<std::string, FunctionDesc> FunctionsMap;
	
class CStandardFunctions
{
public:
	static CStandardFunctions* GetInstance();

	bool IsFunctionExists(const std::string& id)const;	

	bool ValidateFunctionParametersCount(const std::string& id, const unsigned int parametersCount)const;
	std::string GetExpectedParameters(const std::string& id)const;

	// must parameters be initialized or not?
	bool IsOutParametersUsed(const std::string& id)const;

	std::string GetAdditionalParameters(const std::string& id, node* nd)const;

	std::string PatchFunctionName(const std::string& name, const unsigned int argumentCount)const;

private:
	CStandardFunctions();
	CStandardFunctions(const CStandardFunctions& other);
	void operator =(const CStandardFunctions& other);
	~CStandardFunctions();

	FunctionDesc* Register(const std::string& id, const int parametersCount, const bool isOutParametersUsed, const bool isGlobalNamespaceNeeded = true, const bool isArgumentCountAdditionNeeded = false);

	FunctionsMap m_functions;
};