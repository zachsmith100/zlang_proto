
#include "Common.h"
#include "SyntaxTreeProcessor.h"
#include "SymbolTableBuilder.h"
#include "CodeGenerator.h"
#include "Classes.h"

CSyntaxTreeProcessor::CSyntaxTreeProcessor(node* tree, FILE* outputFile):
	m_wasCommandFound(false),
	m_outputFile(outputFile),
	m_syntaxTree(tree),	
	m_tabLevel(0)
{	
}

void CSyntaxTreeProcessor::SetTabLevelForSubTree(node* tree, const int tabLevel)
{
	if (!tree)
	{
		return;
	}

	tree->tabLevel = tabLevel;
	SetTabLevelForSubTree(tree->left, tabLevel);
	SetTabLevelForSubTree(tree->center, tabLevel);
	SetTabLevelForSubTree(tree->right, tabLevel);
}

void CSyntaxTreeProcessor::ProcessImpl(node* tree)
{
	if ((tree->cmd == OP_NEWLINE) || (tree->cmd == OP_SINGLELINE_COMMENT))
	{
		if (tree->cmd == OP_SINGLELINE_COMMENT)
		{
			tree->tabLevel = m_tabLevel;
			tree->left->tabLevel = m_tabLevel;			
		}
		m_tabLevel = 0;
		return;
	}
	else if (tree->cmd == TAB)
	{
		m_tabLevel++;
	}
	else if (tree->cmd == TAB3)
	{
		m_tabLevel += 3;
	}
	else 
	{
		if ((tree->cmd == DECL_CLASS) || (tree->cmd == DECL_CLASS_WITH_TYPE) || (tree->cmd == OP_PARAM_ANNOTATION))
		{
			m_tabLevel = 0;			
		}		

		CScopeLevel* lastLevel = m_levels[m_levels.size() - 1];
		if (lastLevel->GetTabLevel() != m_tabLevel)
		{
			if ((lastLevel->IsEmpty()) && (m_levels.size() > 1))
			{
				m_levels.erase(m_levels.begin() + m_levels.size() - 1);
				delete lastLevel;
				lastLevel = m_levels[m_levels.size() - 1];
			}
			if (lastLevel->GetTabLevel() != m_tabLevel)
			{
				lastLevel = new CScopeLevel();
				lastLevel->SetTabLevel(m_tabLevel);
				m_levels.push_back(lastLevel);
			}
		}

		if (!((tree->cmd == -1) && (strlen(tree->token) == 0)))
		{
			lastLevel->AddNode(tree);
		}
		tree->tabLevel = m_tabLevel;		
	}

	if (tree->left)
	{
		ProcessImpl(tree->left);
	}
	if (tree->center)
	{
		ProcessImpl(tree->center);
	}
	if (tree->right)
	{
		ProcessImpl(tree->right);
	}
	
}
	
void CSyntaxTreeProcessor::Process()
{
	CScopeLevel* defaultLevel = new CScopeLevel();
	defaultLevel->SetTabLevel(0);

	m_levels.push_back(defaultLevel);

	ProcessImpl(m_syntaxTree);

	CSymbolTableBuilder builder;
	builder.SetRootStatus(true);

	for (unsigned int i = 0; i < m_levels.size(); i++)
	{
		CSymbolTable* parentSymbolTable = NULL;
		if (i != 0)
		{
			if (m_levels[i]->GetTabLevel() == 0)
			{
				parentSymbolTable = defaultLevel->GetSymbolTable();
			}
			else if (m_levels[i]->GetTabLevel() > m_levels[i - 1]->GetTabLevel())
			{
				parentSymbolTable = m_levels[i - 1]->GetSymbolTable();
			}
			else
			{
				int k = i - 1;
				bool isFound = false;
				while ((k >= 1) && (m_levels[k]->GetTabLevel() != m_levels[i]->GetTabLevel()))
				{
					k--;
					isFound = true;
				}
				if (isFound)
				{
					parentSymbolTable = m_levels[k]->GetSymbolTable();
				}
			}
		}
				
		builder.Init(m_levels[i], parentSymbolTable);
		builder.Process(m_syntaxTree);
	}

	/*for (unsigned int i = 1; i < m_levels.size(); i++)
	{
		printf("Symbol Table <%d> level <%d>\n", i, m_levels[i]->GetTabLevel());
		m_levels[i]->GetSymbolTable()->Print();
	}*/

	CCodeGenerator codeGenerator(m_levels, m_syntaxTree, m_outputFile);	

	//Classes::GetInstance()->PrintInfo();
}

CSyntaxTreeProcessor::~CSyntaxTreeProcessor()
{
}
