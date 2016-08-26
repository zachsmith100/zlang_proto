
#include "ReservedKeywords.h"
#include <assert.h>

CReservedKeywords::CReservedKeywords()
{
	AddKeyword("result", PROHIBITED_FOR_FUNCTIONS | PROHIBITED_FOR_CLASSES | PROHIBITED_FOR_FUNCTION_PARAMS);
}

void CReservedKeywords::AddKeyword(const std::string& name, const unsigned int flags)
{
	m_reservedSymbols.insert(std::map<std::string, unsigned int>::value_type(name, flags));
}

CReservedKeywords::CReservedKeywords(const CReservedKeywords& other)
{
	assert(false);
}

CReservedKeywords* CReservedKeywords::GetInstance()
{
	static CReservedKeywords* obj = NULL;

	if (!obj)
	{
		obj = new CReservedKeywords();
	}

	return obj;
}

bool CReservedKeywords::CheckPermissions(const std::string& name, const unsigned int flags)const
{
	std::map<std::string, unsigned int>::const_iterator it = m_reservedSymbols.find(name);
	if (it == m_reservedSymbols.end())
	{
		return false;
	}

	return (((*it).second & flags) > 0);
}
	
bool CReservedKeywords::CanBeSymbolUsedForFunctionName(const std::string& name)const
{
	return !CheckPermissions(name, PROHIBITED_FOR_FUNCTIONS);
}

bool CReservedKeywords::CanBeSymbolUsedForFunctionParameter(const std::string& name)const
{
	return !CheckPermissions(name, PROHIBITED_FOR_FUNCTION_PARAMS);
}

bool CReservedKeywords::CanBeSymbolUsedForClassName(const std::string& name)const
{
	return !CheckPermissions(name, PROHIBITED_FOR_CLASSES);
}

bool CReservedKeywords::CanBeSymbolUsedForClassProperty(const std::string& name)const
{
	return !CheckPermissions(name, PROHIBITED_FOR_PROPERTIES);
}

bool CReservedKeywords::CanBeSymbolUsedForVariable(const std::string& name)const
{
	return !CheckPermissions(name, PROHIBITED_FOR_VARIABLES);
}

void CReservedKeywords::operator =(const CReservedKeywords& other)
{
	assert(false);
}

CReservedKeywords::~CReservedKeywords()
{
}