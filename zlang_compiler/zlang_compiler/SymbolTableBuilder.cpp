
#include "Common.h"
#include "SymbolTableBuilder.h"
#include <assert.h>

CSymbolTableBuilder::CSymbolTableBuilder():
	m_isRoot(false)
{	
}

void CSymbolTableBuilder::Init(CScopeLevel* scopeLevel, CSymbolTable* parent)
{
	m_scopeLevel = scopeLevel;
	m_parent = parent;

	assert(scopeLevel);
}

void CSymbolTableBuilder::SetRootStatus(const bool isRoot)
{
	m_isRoot = isRoot;
}
	
void CSymbolTableBuilder::Process(node* syntaxTree)
{	
	if ((!m_parent) && (m_isRoot))
	{
		m_symbolTable = CSymbolTable::GetRoot();
	}	
	else
	{
		m_symbolTable = new CSymbolTable();
	}

	AddSymbolsFromLowerLevel();
	
	if (m_scopeLevel)
	{
		m_scopeLevel->Process(syntaxTree, this);
	}
	m_symbolTable->SetParent(m_parent);
	if (m_parent)
	{
		m_parent->AddChild(m_symbolTable);
	}
	m_scopeLevel->SetSymbolTable(m_symbolTable);
}

void CSymbolTableBuilder::ProcessNode(node* nd)
{
	if (nd)
	{
		if (nd->cmd == FUNC_PARAMS)
		{
			if (!m_functionParamsNodes.IsNodeInList(nd))
			{
				m_functionParamsNodes.Clear();
				m_functionParamsNodes.Process(nd);
			}
		}

		if ((nd->token) && (nd->cmd == -1))
		{
			if (m_functionParamsNodes.IsNodeInList(nd))
			{
				AddSymbolForHigherLevel(nd->token);
			}
			else
			{
				m_symbolTable->AddSymbol(nd->token);
			}
		}
	}
}

void CSymbolTableBuilder::AddSymbolForHigherLevel(const std::string& name)
{
	if (name.size() > 0)
	{
		m_symbolsForNextLevel.push_back(name);
	}
}

void CSymbolTableBuilder::AddSymbolsFromLowerLevel()
{
	std::vector<std::string>::iterator it = m_symbolsForNextLevel.begin();
	std::vector<std::string>::iterator itEnd = m_symbolsForNextLevel.end();

	for ( ; it != itEnd; it++)
	{
		m_symbolTable->AddSymbol(*it);
		m_symbolTable->MarkSymbolAsUsed(*it);
	}

	m_symbolsForNextLevel.clear();
}

CSymbolTableBuilder::~CSymbolTableBuilder()
{
}