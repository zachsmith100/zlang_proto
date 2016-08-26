
#include "Common.h"
#include "UnorderedVarDeclarations.h"
#include "CodeTemplates.h"
#include "CodeGenerator.h"
#include <assert.h>
#include "Classes.h"

CUnorderedVarDeclarations::CUnorderedVarDeclarations():
	m_scopeLevel(0)
{
}

CUnorderedVarDeclarations::CUnorderedVarDeclarations(const CUnorderedVarDeclarations&)
{
	assert(false);
}

CUnorderedVarDeclarations* CUnorderedVarDeclarations::GetInstance()
{
	static CUnorderedVarDeclarations* obj = NULL;

	if (!obj)
	{
		obj = new CUnorderedVarDeclarations();
	}

	return obj;
}

bool CUnorderedVarDeclarations::IsEmpty()const
{
	return (m_variables.size() == 0);
}

void CUnorderedVarDeclarations::AddVariable(const std::string& varName)
{
	m_variables.push_back(varName);
}

void CUnorderedVarDeclarations::ClearAll()
{
	m_variables.clear();
}

void CUnorderedVarDeclarations::SetScopeLevel(const unsigned int scopeLevel)
{
	m_scopeLevel = scopeLevel;
}

std::string CUnorderedVarDeclarations::GetScopeLevelString()const
{
	std::string str = "";
	int scopeLevel = Classes::GetInstance()->GetCurrentClass() ? m_scopeLevel + SCOPE_OFFSET : m_scopeLevel;
	for (int i = 0; i < scopeLevel; i++)
	{
		str += "\t";
	}
	return str;
}

std::string CUnorderedVarDeclarations::GetDeclarationsString()const
{
	USE_TPL(variables, outOfOrderDeclaration);
	USE_INDEX(outOfOrderDeclaration, tabLevel);
	outOfOrderDeclarationTpl->SetVariableValue(tabLevelIndex, GetScopeLevelString());	

	std::string str = "";

	std::vector<std::string>::const_iterator it = m_variables.begin();
	std::vector<std::string>::const_iterator itEnd = m_variables.end();

	for ( ; it != itEnd; it++)
	{
		USE_INDEX(outOfOrderDeclaration, variable);	
		outOfOrderDeclarationTpl->SetVariableValue(variableIndex, *it);
		str += outOfOrderDeclarationTpl->GetText();			
	}

	return str;
}

void CUnorderedVarDeclarations::operator =(const CUnorderedVarDeclarations&)
{
	assert(false);
}

CUnorderedVarDeclarations::~CUnorderedVarDeclarations()
{
}