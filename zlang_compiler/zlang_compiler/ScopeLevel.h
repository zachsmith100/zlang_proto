
#pragma once

#include "node.h"
#include <vector>
#include "SymbolTable.h"
#include "INodeHandler.h"

class CScopeLevel : public INodeHandler
{
public:
	CScopeLevel();
	~CScopeLevel();

	void SetPrevLevel(CScopeLevel* const level);
	CScopeLevel* GetPrevLevel()const;

	void SetNextLevel(CScopeLevel* const level);
	CScopeLevel* GetNextLevel()const;

	void SetTabLevel(const int tabLevel);
	int GetTabLevel()const;

	void AddNode(node* nd);

	void Process(node* syntaxTree, INodeHandler* nodeHandler);

	void SetSymbolTable(CSymbolTable* const symbolTable);
	CSymbolTable* GetSymbolTable();

	bool IsEmpty()const;

	void Print(node* syntaxTree);
	void ProcessNode(node* nd);

	unsigned int GetStartLine()const;

private:
	void ProcessTree(node* tree);

	std::vector<node *> m_nodes;

	CScopeLevel* m_prevLevel;
	CScopeLevel* m_nextLevel;

	CSymbolTable* m_symbolTable;

	int m_tabLevel;

	// for process scope tree
	node* m_startNode;
	node* m_firstNode;
	bool m_isWorking;
	INodeHandler* m_nodeHandler;
};