
#include "Common.h"
#include "BaseCodeGenerator.h"
#include <assert.h>

CBaseCodeGenerator::CBaseCodeGenerator(CCodeGenerator* const codeGenerator):
	m_codeGenerator(codeGenerator)
{
	assert(m_codeGenerator);
}

std::string CBaseCodeGenerator::GetCodeForNode(node* const nd, CompileContext& context, bool isNewVariablesProhibited)
{
	if (m_codeGenerator)
	{		
		if (isNewVariablesProhibited)
		{
			CHANGE_STATE(newVariablesProhibited, true);
		}
		std::string txt = m_codeGenerator->GetCodeForNode(nd, context);
		if (isNewVariablesProhibited)
		{
			RESTORE_STATE(newVariablesProhibited);
		}
		return txt;
	}
	else
	{
		assert(false);
		return "";
	}
}

CBaseCodeGenerator::~CBaseCodeGenerator()
{
}