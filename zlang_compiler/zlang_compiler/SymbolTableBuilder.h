
#pragma once

#include "ScopeLevel.h"
#include "SymbolTable.h"
#include "node.h"
#include "INodeHandler.h"
#include "NodeListBuilder.h"

class CSymbolTableBuilder : public INodeHandler
{
public:
	CSymbolTableBuilder();
	~CSymbolTableBuilder();

	void Init(CScopeLevel* scopeLevel, CSymbolTable* parent);

	void Process(node* syntaxTree);

	void ProcessNode(node* nd);	

	void SetRootStatus(const bool isRoot);

private:
	void AddSymbolForHigherLevel(const std::string& name);
	void AddSymbolsFromLowerLevel();

	CScopeLevel* m_scopeLevel;
	CSymbolTable* m_symbolTable;
	CSymbolTable* m_parent;
	bool m_isRoot;
	
	CNodeListBuilder m_functionParamsNodes;
	std::vector<std::string> m_symbolsForNextLevel;	
};