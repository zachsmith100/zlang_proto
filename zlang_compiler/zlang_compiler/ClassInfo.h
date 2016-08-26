
#pragma once

#include "GlobalSymbolEnumeration.h"
#include "CodeTemplate.h"
#include <string>
#include <vector>
#include <map>
#include "json.h"

#define ADD_PARAM_COUNT		3

enum ClassType
{
	CLASS_TYPE_UNKNOWN = 0,
	CLASS_TYPE_NORMAL,
	CLASS_TYPE_SINGLETON
};

enum SectionType
{
	SECTION_TYPE_PRIVATE = 0,
	SECTION_TYPE_PUBLIC
};

struct PropInfo
{
	SectionType section;
	unsigned int line;

	PropInfo(const PropInfo& other)
	{
		line = other.line;
		section = other.section;
	}

	PropInfo(const unsigned int _line, const SectionType _section)
	{
		line = _line;
		section = _section;
	}
};

class CClassInfo;

struct MethodParameter
{
	std::string name;
	std::string valueType;
	std::string group;
	std::string description;
	unsigned int line;
	int min, max;
	std::vector<std::string> validators;

	MethodParameter(const MethodParameter& other)
	{
		name = other.name;
		valueType = other.valueType;
		group = other.group;
		description = other.description;
		validators = other.validators;
		line = other.line;
		min = other.min;
		max = other.max;
	}

	MethodParameter(const std::string& _name)
	{
		name = _name;
		line = 0;
		min = -1;
		max = -1;
	}

	void CheckValidators(CClassInfo* classInfo, const std::string& methodName);
};

struct MethodInfo
{
	std::vector<unsigned int> parametersCount;
	unsigned int line;
	bool noDeclareMethod;
	SectionType section;
	std::vector<bool> isOutParameter;
	std::vector<MethodParameter> parameters;
	std::string defaultResultField;

	MethodInfo(const MethodInfo& other)
	{
		parametersCount = other.parametersCount;
		section = other.section;
		line = other.line;
		isOutParameter = other.isOutParameter;
		parameters = other.parameters;
		noDeclareMethod = other.noDeclareMethod;
		defaultResultField = other.defaultResultField;
	}

	MethodInfo(const unsigned int _parametersCount, const SectionType _section, const unsigned int _line, const std::vector<bool>& _isOutParameter, const std::vector<MethodParameter>& _parameters, const bool _noDeclareMethod, const std::string& _defaultResultField)
	{
		parametersCount.clear();
		parametersCount.push_back(_parametersCount);
		section = _section;
		line = _line;
		isOutParameter = _isOutParameter;
		parameters = _parameters;
		noDeclareMethod = _noDeclareMethod;
		defaultResultField = _defaultResultField;
	}
};

typedef std::map<std::string, PropInfo> PropertyMap;
typedef std::map<std::string, MethodInfo> MethodMap;

class CClassInfo
{
public:
	CClassInfo(const std::string& name, const unsigned int line, const std::string& classTypeName);
	CClassInfo(const std::string& name, const unsigned int line, const ClassType classType);
	~CClassInfo();

	void SetID(const unsigned int id);
	unsigned int GetID()const;

	void SetCurrentSectionType(SectionType sectionType);
	SectionType GetCurrentSectionType()const;

	void AddProperty(const std::string& name, const unsigned int line, const std::string& initializationCode = "");
	PropertyType GetPropertyType(const std::string& name)const;
	bool IsPropertyExist(const std::string& name)const;
	bool IsLocalPropertyExist(const std::string& name)const;
	bool CheckSecondPropertyDeclaration(const std::string& name, const unsigned int line);

	void AddMethod(const std::string& name, const unsigned int parametersCount, const unsigned int line, const std::vector<bool>& isOutParameter, const std::vector<MethodParameter>& parameters, const bool noDeclareMethod, const std::string& defaultResultField);
	bool IsLocalMethodExist(const std::string& name)const;
	bool IsMethodExist(const std::string& name)const;
	bool CheckSecondMethodDeclaration(const std::string& name, const unsigned int line);
	bool IsOutMethodParameter(const std::string& methodName, const unsigned int paramIndex);

	bool SetBaseClass(const std::string& baseClassName);
	CClassInfo* GetBaseClass()const;
	std::string GetBaseProtoClassName()const;
	bool IsChildClass()const;

	void SetBaseCppClass(const std::string& baseCppClassName);
	std::string GetBaseCppClass()const;

	MethodCallValidateError ValidateMethodCall(const std::string& methodName, const unsigned int parametersCount, const bool isInnerCall)const;

	std::string GetInitializationCode()const;
	std::string GetMethodCallSupportCode()const;
	std::string GetMethodCallByNameSupportCode()const;
	std::string GetPropertySupportCode()const;
	
	std::string GetName()const;
	ClassType GetType()const;

	Json::Value GetInfoAsJSONObject()const;	

	unsigned int GetDeclarationLine()const;

	void SetCompiledStatus(const bool isCompiled);
	bool IsCompiled()const;

	std::string GetClassDeclarationCode()const;
	std::string GetClassForwardDeclarationCode()const;

	void SetBuiltInStatus(const bool isBuiltIn);
	bool IsBuiltIn()const;

	void SetPublicStatus(const bool isPublic);
	bool IsPublic()const;

	void AddCppDeclaration(const std::string& cppCode);

	void CheckValidators();

	static Json::Value GetParameterInfoAsJSONValue(std::vector<MethodParameter>::const_iterator pit);

	std::string GetMethodDefaultResultField(const std::string& name)const;
	std::string GetCustomReturnCode(const std::string& fieldName)const;

private:
	ClassType GetClassTypeFromString(const std::string& nameType);	

	Json::Value GetMethodInfoAsJSONValue(MethodMap::const_iterator mit)const;

	void GetMethodsArray(ValuesList& methods)const;

	std::string m_name;
	unsigned int m_line;
	unsigned int m_id;

	std::vector<std::string> m_initializationCode;
	PropertyMap m_properties;
	MethodMap m_methods;

	ClassType m_classType;
	SectionType m_currentSectionType;

	CClassInfo* m_baseClass;

	bool m_isBuiltIn;
	bool m_isCompiled;
	bool m_isPublic;
	bool m_baseClassExists;

	std::string m_baseCppClassName;

	std::vector<std::string> m_cppDeclarations;
};
