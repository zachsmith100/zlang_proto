
#pragma once

#include "CodeGenerator.h"
#include <string>

class CBaseCodeGenerator : public ICodeGenerator
{
public:
	CBaseCodeGenerator(CCodeGenerator* codeGenerator);
	virtual ~CBaseCodeGenerator();

	std::string GetCodeForNode(node* const nd, CompileContext& context, bool isNewVariablesProhibited = false);

protected:
	CCodeGenerator* m_codeGenerator;
};