
#include "Common.h"
#include "NodeListBuilder.h"
#include <algorithm>

CNodeListBuilder::CNodeListBuilder()
{
}

void CNodeListBuilder::Process(node* nd)
{
	if (nd->left)
	{
		Process(nd->left);
	}
	if (nd->center)
	{
		Process(nd->center);
	}
	if (nd->right)
	{
		Process(nd->right);
	}

	m_list.push_back(nd);
}

bool CNodeListBuilder::IsNodeInList(node* nd)const
{
	std::vector<node *>::const_iterator it = find(m_list.begin(), m_list.end(), nd);
	return (it != m_list.end());
}

void CNodeListBuilder::Clear()
{
	m_list.clear();
}

CNodeListBuilder::~CNodeListBuilder()
{
}