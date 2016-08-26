
#pragma once

#include "node.h"

class INodeHandler
{
public:
	virtual void ProcessNode(node* nd) = 0;
	virtual ~INodeHandler(){ };
};