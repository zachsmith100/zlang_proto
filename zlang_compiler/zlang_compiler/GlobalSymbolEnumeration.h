
#pragma once

#include "UniversalMethodDesc.h"
#include <string>
#include <vector>
#include <map>

enum MethodCallValidateError
{
	OK = 0,
	NO_METHOD,
	PRIVATE_METHOD,
	INVALID_PARAMETERS_COUNT,
	ONLY_PRIVATE_METHOD_HAS_NEEDED_PARAMETERS_COUNT
};

enum PropertyType
{
	PROP_NOT_FOUND = 0,
	PROP_PUBLIC,
	PROP_PRIVATE
};

struct SVariableDesc
{
	bool canBePublic;

	SVariableDesc(const bool _canBePublic)
	{
		canBePublic = _canBePublic;
	}

	SVariableDesc(const SVariableDesc& other)
	{
		canBePublic = other.canBePublic;
	}
};

class CGlobalSymbolEnumeration
{
public:
	static CGlobalSymbolEnumeration* GetInstance();

	void AddMethod(const std::string& name, const unsigned int parametersCount, const bool isPrivate, const std::vector<bool>& isOutParameter);
	bool IsMethodExists(const std::string& name)const;

	void AddProperty(const std::string& name, const bool isPublic);
	bool IsPropertyExists(const std::string& name)const;
	
	bool WriteCodeToFile(const std::string& path);

	MethodCallValidateError ValidateUniversalMethodCall(const std::string& methodName, const unsigned int parametersCount)const;
	PropertyType GetPropertyType(const std::string& name)const;

	bool IsOutMethodParameter(const std::string& methodName, const unsigned int paramIndex)const;

private:
	CGlobalSymbolEnumeration();
	CGlobalSymbolEnumeration(const CGlobalSymbolEnumeration& other);
	void operator =(const CGlobalSymbolEnumeration& other);
	~CGlobalSymbolEnumeration();

	void PrepareCommonSymbols();

	std::string GetMethodEnumerationCode()const;
	std::string GetPropertyEnumerationCode();

	std::vector<std::string> m_methodNames;
	std::map<std::string, CUniversalMethodDesc> m_methodsMap;

	std::vector<std::string> m_propertyNames;
	std::map<std::string, SVariableDesc> m_propertyMap;
};