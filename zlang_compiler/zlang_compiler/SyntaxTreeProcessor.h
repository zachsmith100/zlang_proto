
#pragma once

#include "ScopeLevel.h"

class CSyntaxTreeProcessor
{
public:
	CSyntaxTreeProcessor(node* tree, FILE* outputFile);
	~CSyntaxTreeProcessor();
	
	void Process();

private:
	void ProcessImpl(node* tree);	

	void SetTabLevelForSubTree(node* tree, const int tabLevel);

	std::vector<CScopeLevel *> m_levels;
	node* m_syntaxTree;
	FILE* m_outputFile;
	int m_tabLevel;
	bool m_wasCommandFound;
};