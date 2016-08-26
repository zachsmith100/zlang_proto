
#pragma once

#include <vector>
#include "node.h"

class CNodeListBuilder
{
public:
	CNodeListBuilder();
	~CNodeListBuilder();

	void Process(node* nd);
	void Clear();

	bool IsNodeInList(node* nd)const;

private:
	std::vector<node *> m_list;
};