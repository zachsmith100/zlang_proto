
#include "Common.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <assert.h>
#include "PredefinedVariables.h"
#include <cstdint>

CSymbolTable::CSymbolTable():
	m_parent(NULL)
{
}

CSymbolTable* CSymbolTable::GetRoot()
{
	static CSymbolTable* root = NULL;

	if (!root)
	{
		root = new CSymbolTable();
	}

	return root;
}

void CSymbolTable::SaveState()
{
	m_savedSymbols = m_symbols;
}

void CSymbolTable::ResetState()
{
	m_symbols = m_savedSymbols;
}

void CSymbolTable::SetParent(CSymbolTable* const table)
{	
	m_parent = table;
}

CSymbolTable* CSymbolTable::GetParent()
{
	return m_parent;
}

void CSymbolTable::AddChild(CSymbolTable* const table)
{
	m_children.push_back(table);	
}

std::vector<CSymbolTable *> CSymbolTable::GetChildren()
{
	return m_children;
}

bool CSymbolTable::IsSymbolUsed(const std::string& id)const
{
	SymbolsMap::const_iterator it = m_symbols.find(id);
	if (it == m_symbols.end())
	{
		return m_parent ? m_parent->IsSymbolUsed(id) : false;
	}
	else
	{
		return true;
	}
}

bool CSymbolTable::IsSymbolCanBeUsedForVariable(const std::string& id)const
{
	assert(false);
	if (m_parent)
	if (!m_parent->IsSymbolCanBeUsedForVariable(id))
	{
		return false;
	}	
		
	return true;
}

bool CSymbolTable::IsSymbolUsedFirstly(const std::string& id)const
{
	if (CPredefinedVariables::GetInstance()->IsPredefinedVariable(id))
	{
		return false;
	}

	if (m_parent)
	if (!m_parent->IsSymbolUsedFirstly(id))
	{		
		return false;
	}

	SymbolsMap::const_iterator it = m_symbols.find(id);
	if (it != m_symbols.end())
	if (((*it).second.isUsed) || ((*it).second.isOutParameter))
	{
		return false;
	}

	return true;
}

SymbolDesc CSymbolTable::GetSymbolDesc(const std::string& id)const
{
	if (CPredefinedVariables::GetInstance()->IsPredefinedVariable(id))
	{
		return SymbolDesc();
	}

	if (m_parent)
	if (!m_parent->IsSymbolUsedFirstly(id))
	{		
		return m_parent->GetSymbolDesc(id);
	}

	SymbolsMap::const_iterator it = m_symbols.find(id);
	if (it != m_symbols.end())
	{
		return (*it).second;
	}

	return SymbolDesc();
}

bool CSymbolTable::IsSymbolOutParameter(const std::string& id)const
{
	if (CPredefinedVariables::GetInstance()->IsPredefinedVariable(id))
	{
		return false;
	}

	if (m_parent)
	if (m_parent->IsSymbolOutParameter(id))
	{		
		return true;
	}

	SymbolsMap::const_iterator it = m_symbols.find(id);
	if (it != m_symbols.end())
	if ((*it).second.isOutParameter == true)
	{
		return true;
	}

	return false;
}

void CSymbolTable::SetSymbolStatus(const std::string& id, const bool isUsed)
{
	if (id.size() > 0)
	{
		SymbolsMap::iterator it = m_symbols.find(id);
		if (it != m_symbols.end())
		{
			(*it).second.isUsed = isUsed;
		}
	}
}

void CSymbolTable::SetSymbolType(const std::string& id, const bool isOutParameter)
{
	if (id.size() > 0)
	{
		SymbolsMap::iterator it = m_symbols.find(id);
		if (it != m_symbols.end())
		{
			(*it).second.isOutParameter = isOutParameter;
		}
	}
}

void CSymbolTable::MarkSymbolAsUsed(const std::string& id)
{	
	SetSymbolStatus(id, true);	
}

void CSymbolTable::MarkSymbolAsOutParameter(const std::string& id)
{
	SetSymbolType(id, true);
	SetSymbolStatus(id, false);	
}

void CSymbolTable::AddSymbol(const std::string& id)
{	
	if (id.size() > 0)
	{
		m_symbols.insert(SymbolsMap::value_type(id, SymbolDesc()));
	}
}

void CSymbolTable::Print()
{
	printf("SYMBOL TABLE PTR<%lld> PARENT <%lld>\n", (int64_t)this, (int64_t)m_parent);

	SymbolsMap::iterator it = m_symbols.begin();
	SymbolsMap::iterator itEnd = m_symbols.end();

	for ( ; it != itEnd; it++)
	{
		printf("%s - %s\n", (char*)(*it).first.c_str(), IsSymbolUsedFirstly((*it).first) ? "first" : "...");
	}

	printf("====================\n");
}

void CSymbolTable::MoveSymbolToHigherLevel(const std::string& id)
{
	std::vector<CSymbolTable *>::iterator itc = m_children.begin();
	std::vector<CSymbolTable *>::iterator itcEnd = m_children.end();
	for ( ; itc != itcEnd; itc++)
	{
		SymbolsMap::iterator it = m_symbols.find(id);
		if (it != m_symbols.end())
		{
			(*itc)->AddSymbol(id);
			if ((*it).second.isUsed)
			{
				(*itc)->MarkSymbolAsUsed(id);
			}
			m_symbols.erase(it);
		}

	}
}

CSymbolTable::~CSymbolTable()
{
}
