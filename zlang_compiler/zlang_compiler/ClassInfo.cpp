
#include "Common.h"
#include "ClassInfo.h"
#include <assert.h>
#include "Utils.h"
#include "Classes.h"
#include <algorithm>
#include "CodeTemplates.h"
#include "ErrorReporter.h"
#include "GlobalSymbolEnumeration.h"

#define RESULT_DEFAULT_FIELD	"result"

void MethodParameter::CheckValidators(CClassInfo* classInfo, const std::string& methodName)
{
	std::vector<std::string>::iterator sit = validators.begin();
	std::vector<std::string>::iterator sitEnd = validators.end();

	for ( ; sit != sitEnd; sit++)
	{
		if (!classInfo->IsMethodExist(*sit))
		{
			CErrorReporter::GetInstance()->OnFatalError(ANNOTATION_INVALID_VALIDATOR, line, methodName, *sit);
		}
	}
}

CClassInfo::CClassInfo(const std::string& name, const unsigned int line, const std::string& classTypeName):
	m_id(0),
	m_name(name),
	m_line(line),
	m_isPublic(true),
	m_baseClass(NULL),
	m_classType(GetClassTypeFromString(classTypeName)),
	m_isBuiltIn(false),
	m_isCompiled(false),
	m_baseClassExists(false),
	m_currentSectionType(SECTION_TYPE_PUBLIC)
{
}

CClassInfo::CClassInfo(const std::string& name, const unsigned int line, const ClassType classType):
	m_id(0),
	m_name(name),
	m_line(line),
	m_isPublic(true),
	m_baseClass(NULL),	
	m_classType(classType),
	m_isBuiltIn(false),
	m_isCompiled(false),
	m_baseClassExists(false),
	m_currentSectionType(SECTION_TYPE_PUBLIC)
{
}

void CClassInfo::SetID(const unsigned int id)
{
	m_id = id;
}

unsigned int CClassInfo::GetID()const
{
	return m_id;
}

void CClassInfo::SetPublicStatus(const bool isPublic)
{
	m_isPublic = isPublic;
}

bool CClassInfo::IsPublic()const
{
	return m_isPublic;
}

void CClassInfo::SetBaseCppClass(const std::string& baseCppClassName)
{
	m_baseCppClassName = baseCppClassName;
}

std::string CClassInfo::GetBaseCppClass()const
{
	return m_baseCppClassName;
}

bool CClassInfo::IsChildClass()const
{
	return m_baseClassExists;
}

bool CClassInfo::SetBaseClass(const std::string& baseClassName)
{
	m_baseClassExists = (baseClassName.size() > 0);
	m_baseClass = Classes::GetInstance()->GetClassByName(baseClassName);
	return (m_baseClass != NULL);
}

CClassInfo* CClassInfo::GetBaseClass()const
{
	return m_baseClass;
}

std::string CClassInfo::GetBaseProtoClassName()const
{
	if (m_baseCppClassName.size() > 0)
	{
		return "CClassDataContainer, " + m_baseCppClassName;
	}

	if (!m_baseClass)
	{
		return "CClassDataContainer";
	}

	return m_baseClass->GetName() + "Proto";
}

void CClassInfo::AddMethod(const std::string& name, const unsigned int parametersCount, const unsigned int line, const std::vector<bool>& isOutParameter, const std::vector<MethodParameter>& parameters, const bool noDeclareMethod, const std::string& defaultResultField)
{
	if (CheckSecondMethodDeclaration(name, line))
	{		
		assert(false);
		return;
	}

	if (!m_isBuiltIn)
	{
		assert(parametersCount == parameters.size());
	}

	CGlobalSymbolEnumeration::GetInstance()->AddMethod(name, parametersCount, m_currentSectionType == SECTION_TYPE_PRIVATE, isOutParameter);

	MethodInfo methodInfo(parametersCount, m_currentSectionType, line, isOutParameter, parameters, noDeclareMethod, defaultResultField);

	assert(parametersCount == isOutParameter.size());

	if (m_isBuiltIn)
	{
		MethodMap::iterator it = m_methods.find(name);
		if (it == m_methods.end())
		{
			m_methods.insert(MethodMap::value_type(name, methodInfo));
		}
		else
		{
			(*it).second.parametersCount.push_back(parametersCount);
			if ((*it).second.isOutParameter.size() < isOutParameter.size())
			{
				(*it).second.isOutParameter = isOutParameter;
			}
		}
	}
	else 
	{
		MethodMap::iterator it = m_methods.find(name);
		if (it == m_methods.end())
		{
			m_methods.insert(MethodMap::value_type(name, methodInfo));
		}
		else
		{
			(*it).second.parameters = parameters;
			(*it).second.defaultResultField = defaultResultField;
		}
	}
}

bool CClassInfo::CheckSecondMethodDeclaration(const std::string& name, const unsigned int line)
{
	MethodMap::const_iterator it = m_methods.find(name);
	if (it == m_methods.end())
	{
		return false;
	}

	return ((*it).second.line != line);
}

bool CClassInfo::IsLocalMethodExist(const std::string& name)const
{
	MethodMap::const_iterator it = m_methods.find(name);
	return (it != m_methods.end());
}

bool CClassInfo::IsOutMethodParameter(const std::string& methodName, const unsigned int paramIndex)
{
	MethodMap::const_iterator it = m_methods.find(methodName);
	if (it == m_methods.end())
	{
		return false;
	}	

	if (paramIndex >= (*it).second.isOutParameter.size())
	{
		return false;

	}
	return (*it).second.isOutParameter[paramIndex];
}

bool CClassInfo::IsMethodExist(const std::string& name)const
{
	MethodMap::const_iterator it = m_methods.find(name);
	if (it != m_methods.end())
	{
		return true;
	}

	if ((m_baseClass) && (m_baseClass->IsMethodExist(name)))
	{
		return true;
	}

	return false;
}

void CClassInfo::AddProperty(const std::string& name, const unsigned int line, const std::string& initializationCode)
{
	if (CheckSecondPropertyDeclaration(name, line))
	{
		assert(false);
		return;
	}

	m_properties.insert(PropertyMap::value_type(name, PropInfo(line, m_currentSectionType)));

	if (initializationCode.size() != 0)
	{
		m_initializationCode.push_back(initializationCode);
	}

	CGlobalSymbolEnumeration::GetInstance()->AddProperty(name, (m_currentSectionType == SECTION_TYPE_PUBLIC));
}

bool CClassInfo::IsLocalPropertyExist(const std::string& name)const
{
	PropertyMap::const_iterator it = m_properties.find(name);
	return (it != m_properties.end());
}

bool CClassInfo::IsPropertyExist(const std::string& name)const
{
	PropertyMap::const_iterator it = m_properties.find(name);
	if (it != m_properties.end())
	{
		return true;
	}

	if ((m_baseClass) && (m_baseClass->IsPropertyExist(name)))
	{
		return true;
	}

	return false;
}

PropertyType CClassInfo::GetPropertyType(const std::string& name)const
{
	PropertyMap::const_iterator it = m_properties.find(name);
	if (it == m_properties.end())
	{
		if (m_baseClass)
		{
			return m_baseClass->GetPropertyType(name);
		}
		else
		{
			return PROP_NOT_FOUND;
		}
	}

	return ((*it).second.section == SECTION_TYPE_PUBLIC) ? PROP_PUBLIC : PROP_PRIVATE;
}

MethodCallValidateError CClassInfo::ValidateMethodCall(const std::string& methodName, const unsigned int parametersCount, const bool isInnerCall)const
{
	MethodMap::const_iterator it = m_methods.find(methodName);
	if (it == m_methods.end())
	{
		if (m_baseClass)
		{
			return m_baseClass->ValidateMethodCall(methodName, parametersCount, isInnerCall);
		}
		else
		{
			return NO_METHOD;
		}
	}

	if ((!isInnerCall) && ((*it).second.section != SECTION_TYPE_PUBLIC))
	{
		return PRIVATE_METHOD;
	}

	std::vector<unsigned int>::const_iterator itCheck = find((*it).second.parametersCount.begin(), (*it).second.parametersCount.end(), parametersCount);
	if (itCheck == (*it).second.parametersCount.end())
	{
		return INVALID_PARAMETERS_COUNT;
	}

	return OK;
}

std::string CClassInfo::GetInitializationCode()const
{
	USE_TPL(classes, constructor);
		
	USE_INDEX(constructor, className);
	USE_INDEX(constructor, initCode);
	USE_INDEX(constructor, functionPointers);
			
	constructorTpl->SetVariableValue(classNameIndex, m_name);	
	constructorTpl->SetVariableValue(initCodeIndex, m_initializationCode);

	ValuesList methods;
	GetMethodsArray(methods);

	constructorTpl->SetVariableValue(functionPointersIndex, methods);
	std::string code = constructorTpl->GetText();

	return code;
}

std::string CClassInfo::GetPropertySupportCode()const
{
	USE_TPL(classes, propertySupport);

	USE_INDEX(propertySupport, className);
	propertySupportTpl->SetVariableValue(classNameIndex, m_name);

	return propertySupportTpl->GetText();
}

std::string CClassInfo::GetCustomReturnCode(const std::string& fieldName)const
{
	USE_TPL(functions, customResultFieldCode);
	USE_INDEX(customResultFieldCode, fieldName);
	customResultFieldCodeTpl->SetVariableValue(fieldNameIndex, (fieldName == "") ? RESULT_DEFAULT_FIELD : fieldName);
	return customResultFieldCodeTpl->GetText();	
}

void CClassInfo::GetMethodsArray(ValuesList& methods)const
{
	MethodMap::const_iterator mit = m_methods.begin();
	MethodMap::const_iterator mitEnd = m_methods.end();

	for ( ; mit != mitEnd; mit++)
	{
		methods.push_back((*mit).first);
	}
}

std::string CClassInfo::GetMethodCallSupportCode()const
{
	std::string code;

	USE_TPL(classes, methodsCallHeader);
	USE_INDEX(methodsCallHeader, className);
	methodsCallHeaderTpl->SetVariableValue(classNameIndex, GetName());
	code += methodsCallHeaderTpl->GetText();		
		
	MethodMap::const_iterator it = m_methods.begin();
	MethodMap::const_iterator itEnd = m_methods.end();

	for ( ; it != itEnd; it++)
	{
		unsigned int paramCount = (*it).second.parametersCount[0];

		USE_TPL(classes, methodsCallItem);
		USE_INDEX(methodsCallItem, paramCount);
		USE_INDEX(methodsCallItem, initVar);
		USE_INDEX(methodsCallItem, param);
		USE_INDEX(methodsCallItem, methodName);
		USE_INDEX(methodsCallItem, minCallStatus);		
		USE_INDEX(methodsCallItem, customReturnCode);

		methodsCallItemTpl->SetVariableValue(paramCountIndex, paramCount + ADD_PARAM_COUNT); //plus 3 because of 3 additional variables
		methodsCallItemTpl->SetVariableValue(methodNameIndex, (*it).first);
		methodsCallItemTpl->SetVariableRange(initVarIndex, 0, paramCount - 1); 
		methodsCallItemTpl->SetVariableRange(paramIndex, 0, paramCount - 1, false); 
		methodsCallItemTpl->SetVariableValue(minCallStatusIndex, ((*it).second.section == SECTION_TYPE_PRIVATE) ? 1 : 0);
		methodsCallItemTpl->SetVariableValue(customReturnCodeIndex, GetCustomReturnCode((*it).second.defaultResultField));

		code += methodsCallItemTpl->GetText();
	}

	if (m_baseClass)
	{
		USE_TPL(classes, methodsCallFooterChildClass);
		USE_INDEX(methodsCallFooterChildClass, baseClassName);
		methodsCallFooterChildClassTpl->SetVariableValue(baseClassNameIndex, m_baseClass->GetName());
		code += methodsCallFooterChildClassTpl->GetText();		
	}
	else
	{
		ADD_FROM_TPL(classes, methodsCallFooterBaseClass, code); 
	}

	ValuesList methods;
	GetMethodsArray(methods);

	{
		USE_TPL(classes, methodExistFunc);
		USE_INDEX(methodExistFunc, names);
		USE_INDEX(methodExistFunc, className);
		methodExistFuncTpl->SetVariableValue(classNameIndex, GetName());
		methodExistFuncTpl->SetVariableValue(namesIndex, methods); 
		code += methodExistFuncTpl->GetText();
	}

	return code;
}

std::string CClassInfo::GetMethodCallByNameSupportCode()const
{
	std::string code;

	USE_TPL(classes, methodsCallByNameHeader);
	USE_INDEX(methodsCallByNameHeader, className);
	methodsCallByNameHeaderTpl->SetVariableValue(classNameIndex, GetName());
	code += methodsCallByNameHeaderTpl->GetText();		
		
	MethodMap::const_iterator it = m_methods.begin();
	MethodMap::const_iterator itEnd = m_methods.end();

	for ( ; it != itEnd; it++)
	{
		unsigned int paramCount = (*it).second.parametersCount[0];

		USE_TPL(classes, methodsCallByNameItem);
		USE_INDEX(methodsCallByNameItem, paramCount);
		USE_INDEX(methodsCallByNameItem, initVar);
		USE_INDEX(methodsCallByNameItem, param);
		USE_INDEX(methodsCallByNameItem, methodName);
		
		methodsCallByNameItemTpl->SetVariableValue(paramCountIndex, paramCount);
		methodsCallByNameItemTpl->SetVariableValue(methodNameIndex, (*it).first);
		methodsCallByNameItemTpl->SetVariableRange(initVarIndex, 0, paramCount - 1); 
		methodsCallByNameItemTpl->SetVariableRange(paramIndex, 0, paramCount - 1, false); 
		
		code += methodsCallByNameItemTpl->GetText();
	}

	if (m_baseClass)
	{
		USE_TPL(classes, methodsCallByNameFooterChildClass);
		USE_INDEX(methodsCallByNameFooterChildClass, baseClassName);
		methodsCallByNameFooterChildClassTpl->SetVariableValue(baseClassNameIndex, m_baseClass->GetName());
		code += methodsCallByNameFooterChildClassTpl->GetText();		
	}
	else
	{
		ADD_FROM_TPL(classes, methodsCallByNameFooterBaseClass, code); 
	}
	
	return code;
}

std::string CClassInfo::GetName()const
{
	return m_name;
}

ClassType CClassInfo::GetType()const
{
	if ((m_baseClass) && (m_baseClass->GetType() == CLASS_TYPE_SINGLETON))
	{
		return CLASS_TYPE_SINGLETON;
	}
	return m_classType;
}

ClassType CClassInfo::GetClassTypeFromString(const std::string& nameType)
{
	if (nameType == "singleton")
	{
		return CLASS_TYPE_SINGLETON;
	}

	return CLASS_TYPE_UNKNOWN;
}

bool CClassInfo::CheckSecondPropertyDeclaration(const std::string& name, const unsigned int line)
{
	PropertyMap::const_iterator it = m_properties.find(name);
	if (it == m_properties.end())
	{
		return false;
	}

	return ((*it).second.line != line);
}

void CClassInfo::SetCurrentSectionType(SectionType sectionType)
{
	m_currentSectionType = sectionType;
}

SectionType CClassInfo::GetCurrentSectionType()const
{
	return m_currentSectionType;
}

Json::Value CClassInfo::GetParameterInfoAsJSONValue(std::vector<MethodParameter>::const_iterator pit)
{
	Json::Value paramObject;
	paramObject["name"] = (*pit).name;
	paramObject["description"] = (*pit).description;
	paramObject["group"] = (*pit).group;
	paramObject["type"] = (*pit).valueType;
	if (((*pit).min != -1) && ((*pit).max != -1))
	{
		paramObject["min"] = (*pit).min;
		paramObject["max"] = (*pit).max;
	}

	Json::Value validatorsObject;
	validatorsObject.resize((*pit).validators.size());

	std::vector<std::string>::const_iterator it = (*pit).validators.begin();
	std::vector<std::string>::const_iterator itEnd = (*pit).validators.end();

	for (int i = 0; it != itEnd; it++, i++)
	{
		validatorsObject[i] = *it;
	}

	paramObject["validators"] = validatorsObject;

	return paramObject;
}

Json::Value CClassInfo::GetMethodInfoAsJSONValue(MethodMap::const_iterator mit)const
{
	Json::Value methodObject;
	methodObject["name"] = (*mit).first;
	methodObject["public"] = (*mit).second.section == SECTION_TYPE_PUBLIC;

	Json::Value paramsObject;
	paramsObject.resize((*mit).second.parameters.size());

	std::vector<MethodParameter>::const_iterator pit = (*mit).second.parameters.begin();
	std::vector<MethodParameter>::const_iterator pitEnd = (*mit).second.parameters.end();

	for (int i = 0; pit != pitEnd; pit++, i++)	
	{
		paramsObject[i] = GetParameterInfoAsJSONValue(pit);
	}

	methodObject["parameters"] = paramsObject;
	
	return methodObject;
}

Json::Value CClassInfo::GetInfoAsJSONObject()const
{
	Json::Value classObject;
	classObject["name"] = GetName();
	classObject["type"] = (GetType() == CLASS_TYPE_SINGLETON ? "singleton" : "normal");
		
	Json::Value methodsArray;		

	MethodMap::const_iterator mit = m_methods.begin();
	MethodMap::const_iterator mitEnd = m_methods.end();

	for (int i = 0; mit != mitEnd; mit++)
	if (((*mit).second.section == SECTION_TYPE_PUBLIC) && ((*mit).first != CLASS_ON_CREATED_CALLBACK))
	{
		methodsArray.resize(methodsArray.size() + 1);
		methodsArray[i] = GetMethodInfoAsJSONValue(mit);
		i++;
	}

	classObject["methods"] = methodsArray;

	return classObject;
		
	/*printf("Class: %s, %s\n", GetName().c_str(), (GetType() == CLASS_TYPE_SINGLETON ? "singleton" : "normal"));

	PropertyMap::iterator it = m_properties.begin();
	PropertyMap::iterator itEnd = m_properties.end();

	for ( ; it != itEnd; it++)
	{
		printf("Property: %s = %s\n", (*it).first.c_str(), ((*it).second.section == SECTION_TYPE_PRIVATE) ? "private" : "public");
	}

	MethodMap::iterator mit = m_methods.begin();
	MethodMap::iterator mitEnd = m_methods.end();

	for ( ; mit != mitEnd; mit++)
	{
		printf("Method: %s = %s, %d parameters\n", (*mit).first.c_str(), ((*mit).second.section == SECTION_TYPE_PRIVATE) ? "private" : "public", (*mit).second.parametersCount[0]);
	}

	printf("\n\n");*/
}

unsigned int CClassInfo::GetDeclarationLine()const
{
	return m_line;
}

void CClassInfo::SetCompiledStatus(const bool isCompiled)
{
	m_isCompiled = isCompiled;
}

bool CClassInfo::IsCompiled()const
{
	return m_isCompiled;
}

std::string CClassInfo::GetClassForwardDeclarationCode()const
{
	USE_TPL(classes, forwardClassDeclaration);
	USE_INDEX(forwardClassDeclaration, name);
	forwardClassDeclarationTpl->SetVariableValue(nameIndex, GetName());	
	return forwardClassDeclarationTpl->GetText();
}

std::string CClassInfo::GetClassDeclarationCode()const
{
	std::string code = "";
	const char* sectionsName[2] = {"private", "public"};
	
	MethodMap::const_iterator mit = m_methods.begin();
	MethodMap::const_iterator mitEnd = m_methods.end();

	for (int section = SECTION_TYPE_PRIVATE; section <= SECTION_TYPE_PUBLIC; section++) 
	{
		std::string sectionContents;
		for ( ; mit != mitEnd; mit++)
		if (!(*mit).second.noDeclareMethod)
		{
			USE_TPL(classes, methodDeclaration);
			USE_INDEX(methodDeclaration, name);
			USE_INDEX(methodDeclaration, className);
			USE_INDEX(methodDeclaration, param);
			methodDeclarationTpl->SetVariableValue(nameIndex, (*mit).first);	
			methodDeclarationTpl->SetVariableValue(classNameIndex, GetName());	
			methodDeclarationTpl->SetVariableRange(paramIndex, 1, (*mit).second.parametersCount[0], false);	
			sectionContents += methodDeclarationTpl->GetText();	
		}

		USE_TPL(classes, classDeclarationSection);
		USE_INDEX(classDeclarationSection, section);
		USE_INDEX(classDeclarationSection, sectionContents);
		classDeclarationSectionTpl->SetVariableValue(sectionIndex, sectionsName[section]);	
		classDeclarationSectionTpl->SetVariableValue(sectionContentsIndex, sectionContents);	
		code += classDeclarationSectionTpl->GetText();	
	}

	std::vector<std::string>::const_iterator it = m_cppDeclarations.begin();
	std::vector<std::string>::const_iterator itEnd = m_cppDeclarations.end();

	for ( ; it != itEnd; it++)
	{
		code += (*it);
	}
	
	USE_TPL(classes, classDeclarationFooter);
	USE_INDEX(classDeclarationFooter, name);
	classDeclarationFooterTpl->SetVariableValue(nameIndex, GetName());	
	code += classDeclarationFooterTpl->GetText();	

	return code;
}

void CClassInfo::SetBuiltInStatus(const bool isBuiltIn)
{
	m_isBuiltIn = isBuiltIn;
}

bool CClassInfo::IsBuiltIn()const
{
	return m_isBuiltIn;
}

void CClassInfo::AddCppDeclaration(const std::string& cppCode)
{
	m_cppDeclarations.push_back(cppCode);
}

void CClassInfo::CheckValidators()
{
	MethodMap::iterator it = m_methods.begin();
	MethodMap::iterator itEnd = m_methods.end();

	for ( ; it != itEnd; it++)
	{
		MethodInfo mi = (*it).second;

		std::vector<MethodParameter>::iterator itp = mi.parameters.begin();
		std::vector<MethodParameter>::iterator itpEnd = mi.parameters.end();

		for ( ; itp != itpEnd; itp++)
		{
			(*itp).CheckValidators(this, (*it).first);
		}
	}
}

std::string CClassInfo::GetMethodDefaultResultField(const std::string& name)const
{
	MethodMap::const_iterator it = m_methods.find(name);
	if (it == m_methods.end())
	{
		return "";
	}

	return (*it).second.defaultResultField;
}

CClassInfo::~CClassInfo()
{
}
