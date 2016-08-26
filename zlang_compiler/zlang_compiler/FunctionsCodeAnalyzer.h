
#pragma once	

#include "BaseCodeGenerator.h"

class CFunctionsCodeAnalyzer : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CFunctionsCodeAnalyzer)

	DECLARE_HANDLER(ProcessFunctionDeclaration);
	DECLARE_HANDLER(ProcessFunctionParameters);
	DECLARE_HANDLER(ProcessOutParameter);
	DECLARE_HANDLER(ProcessInnerPropertiesForCalls);
	DECLARE_HANDLER(ProcessGlobalFunctionDeclaration);

	std::string GetInnerProperty(node* nd, CompileContext& context);

	std::string GetFunctionName(node* nd, CompileContext& context);

	unsigned int m_parametersCount;
		
	std::vector<bool> m_outParamStatus;
	bool m_outParamDeclared;

	std::vector<MethodParameter> m_parametersDescription;
};