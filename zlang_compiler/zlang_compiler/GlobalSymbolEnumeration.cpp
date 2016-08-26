
#include "Common.h"
#include "GlobalSymbolEnumeration.h"
#include "CodeTemplates.h"
#include "Utils.h"
#include <assert.h>
#include "Classes.h"
#include "GlobalFunctions.h"

CGlobalSymbolEnumeration::CGlobalSymbolEnumeration()
{
}

CGlobalSymbolEnumeration::CGlobalSymbolEnumeration(const CGlobalSymbolEnumeration& other)
{
	assert(false);
}

CGlobalSymbolEnumeration* CGlobalSymbolEnumeration::GetInstance()
{
	static CGlobalSymbolEnumeration* obj = NULL;

	if (!obj)
	{
		obj = new CGlobalSymbolEnumeration();
	}

	return obj;
}

void CGlobalSymbolEnumeration::AddMethod(const std::string& name, const unsigned int parametersCount, const bool isPrivate, const std::vector<bool>& isOutParameter)
{
	std::map<std::string, CUniversalMethodDesc>::iterator it = m_methodsMap.find(name);
	if (it == m_methodsMap.end())
	{		
		CUniversalMethodDesc desc(parametersCount, !isPrivate, isOutParameter);
		m_methodsMap.insert(std::map<std::string, CUniversalMethodDesc>::value_type(name, desc));
	}
	else
	{
		CUniversalMethodDesc& desc = (*it).second;		
		desc.AddParameterCount(parametersCount, !isPrivate, isOutParameter);
	}
}

bool CGlobalSymbolEnumeration::IsMethodExists(const std::string& name)const
{
	std::map<std::string, CUniversalMethodDesc>::const_iterator it = m_methodsMap.find(name);
	return (it != m_methodsMap.end());
}

void CGlobalSymbolEnumeration::AddProperty(const std::string& name, const bool isPublic)
{
	std::map<std::string, SVariableDesc>::iterator it = m_propertyMap.find(name);
	if (it == m_propertyMap.end())
	{		
		SVariableDesc varDesc(isPublic);
		m_propertyMap.insert(std::map<std::string, SVariableDesc>::value_type(name, varDesc));
	}
	else if (isPublic)
	{
		(*it).second.canBePublic = isPublic;
	}		
}

bool CGlobalSymbolEnumeration::IsPropertyExists(const std::string& name)const
{
	std::map<std::string, SVariableDesc>::const_iterator it = m_propertyMap.find(name);
	return (it != m_propertyMap.end());
}

PropertyType CGlobalSymbolEnumeration::GetPropertyType(const std::string& name)const
{
	std::map<std::string, SVariableDesc>::const_iterator it = m_propertyMap.find(name);
	if (it == m_propertyMap.end())
	{
		return PROP_NOT_FOUND;
	}

	return (*it).second.canBePublic ? PROP_PUBLIC : PROP_PRIVATE;
}

std::string CGlobalSymbolEnumeration::GetMethodEnumerationCode()const
{
	USE_TPL(classes, methodsEnumeration);
	{
		USE_INDEX(methodsEnumeration, item);
		methodsEnumerationTpl->SetVariableValue(itemIndex, m_methodNames);
	}

	USE_TPL(classes, methodNamesEnumeration);
	{
		USE_INDEX(methodNamesEnumeration, item);	
		methodNamesEnumerationTpl->SetVariableValue(itemIndex, m_methodNames);
	}

	USE_TPL(classes, classNamesEnumeration);
	{
		USE_INDEX(classNamesEnumeration, item);	
		USE_INDEX(classNamesEnumeration, count);	
		classNamesEnumerationTpl->SetVariableValue(itemIndex, Classes::GetInstance()->GetClassesList());
		classNamesEnumerationTpl->SetVariableValue(countIndex, Classes::GetInstance()->GetClassCount());
	}
	
	return methodsEnumerationTpl->GetText() + methodNamesEnumerationTpl->GetText() + 
		classNamesEnumerationTpl->GetText();	
}

std::string CGlobalSymbolEnumeration::GetPropertyEnumerationCode()
{
	if (m_propertyNames.size() == 0)
	{
		m_propertyNames.push_back("some_property_placeholder");
	}

	USE_TPL(classes, propertyEnumeration);
	USE_INDEX(propertyEnumeration, item);

	propertyEnumerationTpl->SetVariableValue(itemIndex, m_propertyNames);

	USE_TPL(classes, propertyNamesEnumeration);
	{
		USE_INDEX(propertyNamesEnumeration, item);	
		propertyNamesEnumerationTpl->SetVariableValue(itemIndex, m_propertyNames);
	}

	return propertyEnumerationTpl->GetText() + propertyNamesEnumerationTpl->GetText();
}

bool CGlobalSymbolEnumeration::WriteCodeToFile(const std::string& path)
{
	FILE* enumsFile = OpenFile(path, "w");
	if (!enumsFile)
	{
		return false;
	}

	PrepareCommonSymbols();
	fprintf(enumsFile, "%s", GetMethodEnumerationCode().c_str());
	fprintf(enumsFile, "%s", GetPropertyEnumerationCode().c_str());
	fclose(enumsFile);

	return true;
}

MethodCallValidateError CGlobalSymbolEnumeration::ValidateUniversalMethodCall(const std::string& methodName, const unsigned int parametersCount)const
{
	std::map<std::string, CUniversalMethodDesc>::const_iterator mit = m_methodsMap.find(methodName);

	if (mit == m_methodsMap.end())
	{
		return NO_METHOD;
	}

	if (!(*mit).second.CanBePublic())
	{
		return PRIVATE_METHOD;
	}

	if (!(*mit).second.IsParameterCountSupported(parametersCount, true))
	{
		if ((*mit).second.IsParameterCountSupported(parametersCount, false))
		{
			return ONLY_PRIVATE_METHOD_HAS_NEEDED_PARAMETERS_COUNT;
		}
		else
		{
			return INVALID_PARAMETERS_COUNT;
		}
	}

	return OK;
}

void CGlobalSymbolEnumeration::PrepareCommonSymbols()
{
	std::map<std::string, CUniversalMethodDesc>::iterator mit = m_methodsMap.begin();
	std::map<std::string, CUniversalMethodDesc>::iterator mitEnd = m_methodsMap.end();

	for ( ; mit != mitEnd; mit++)
	{		
		m_propertyNames.push_back((*mit).first);
	}

	for (std::map<std::string, SVariableDesc>::iterator it = m_propertyMap.begin(); it != m_propertyMap.end(); it++)
	{
		std::map<std::string, CUniversalMethodDesc>::iterator pit = m_methodsMap.find((*it).first);
		if (pit == m_methodsMap.end())
		{
			m_propertyNames.push_back((*it).first);
		}
	}

	m_methodNames = m_propertyNames;

	CGlobalFunctions::GetInstance()->AddGlobalFunctionsToList(m_methodNames);
}

bool CGlobalSymbolEnumeration::IsOutMethodParameter(const std::string& methodName, const unsigned int paramIndex)const
{
	std::map<std::string, CUniversalMethodDesc>::const_iterator it = m_methodsMap.find(methodName);
	if (it == m_methodsMap.end())
	{
		return false;
	}	
	return (*it).second.IsOutMethodParameter(paramIndex);
}

void CGlobalSymbolEnumeration::operator =(const CGlobalSymbolEnumeration& other)
{
	assert(false);
}

CGlobalSymbolEnumeration::~CGlobalSymbolEnumeration()
{
}