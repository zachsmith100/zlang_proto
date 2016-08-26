
#include "Common.h"
#include "ScopeLevel.h"

CScopeLevel::CScopeLevel():	
	m_nodeHandler(NULL),
	m_symbolTable(NULL),
	m_isWorking(false),
	m_startNode(NULL),
	m_firstNode(NULL),
	m_prevLevel(NULL),
	m_nextLevel(NULL),
	m_tabLevel(0)
{
}

void CScopeLevel::SetTabLevel(const int tabLevel)
{
	m_tabLevel = tabLevel;
}

int CScopeLevel::GetTabLevel()const
{
	return m_tabLevel;
}

void CScopeLevel::SetPrevLevel(CScopeLevel* const level)
{
	m_prevLevel = level;
}

CScopeLevel* CScopeLevel::GetPrevLevel()const
{
	return m_prevLevel;
}

void CScopeLevel::SetNextLevel(CScopeLevel* const level)
{
	m_nextLevel = level;
}

CScopeLevel* CScopeLevel::GetNextLevel()const
{
	return m_nextLevel;
}

void CScopeLevel::AddNode(node* nd)
{
	m_nodes.push_back(nd);
}

bool CScopeLevel::IsEmpty()const
{
	return (m_nodes.size() == 0);
}

bool IsNoTabsNode(node* tree)
{
	if (!tree->left)
	{
		return true;
	}

	if ((tree->left->cmd != TAB3) && (tree->left->cmd != TAB))
	{
		return IsNoTabsNode(tree->left);
	}
	
	return false;	
}

void CScopeLevel::ProcessTree(node* tree)
{
	if (!m_isWorking)
	{
		return;
	}

	if (tree == m_startNode)
	{
		m_startNode = NULL;
	}

	if (!m_startNode)
	if (m_tabLevel != -1)
	{		
		if ((tree->cmd != TAB) && (tree->cmd != TAB3) && (tree->cmd != OP_NEWLINE)  && (tree->cmd != OP_SINGLELINE_COMMENT) &&			
			(IsNoTabsNode(tree))
			)
		if (tree->tabLevel != m_tabLevel)
		{
			m_isWorking = false;
			return;
		}
	}	
	
	if (!m_startNode)
	{
		m_nodeHandler->ProcessNode(tree);
	}

	if ((tree->left) && (m_isWorking))
	{
		ProcessTree(tree->left);
	}
	if ((tree->center) && (m_isWorking))
	{
		ProcessTree(tree->center);
	}
	if ((tree->right) && (m_isWorking))
	{
		ProcessTree(tree->right);
	}	
}

void CScopeLevel::Process(node* syntaxTree, INodeHandler* nodeHandler)
{
	if (m_nodes.size() > 0)
	{
		m_firstNode = m_nodes[0];
		m_startNode = m_nodes[0];
		m_isWorking = true;
		m_nodeHandler = nodeHandler;
		ProcessTree(syntaxTree);				
	}
}

void CScopeLevel::Print(node* syntaxTree)
{
	printf("Scope level start\n");
	Process(syntaxTree, this);
	printf("Scope level end\n");
}

void CScopeLevel::ProcessNode(node* nd)
{
	// this is process node for debug printing

	if (nd)
	{
		if (nd->token)
		{
			printf("Token: %s %d\n", nd->token, nd->cmd);
		}
		else
		{
			printf("Token: %d\n", nd->cmd);
		}
	}

}

void CScopeLevel::SetSymbolTable(CSymbolTable* const symbolTable)
{
	m_symbolTable = symbolTable;
}

CSymbolTable* CScopeLevel::GetSymbolTable()
{
	return m_symbolTable;
}

unsigned int CScopeLevel::GetStartLine()const
{
	if (m_firstNode)
	{
		return m_firstNode->line;
	}
	
	return 0;
}

CScopeLevel::~CScopeLevel()
{
}