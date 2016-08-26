
#include "Common.h"
#include "GlobalSymbolEnumeration.h"
#include "UnorderedVarDeclarations.h"
#include "CurrentMethodAnnotation.h"
#include "FunctionsCodeAnalyzer.h"
#include "PredefinedVariables.h"
#include "ClassCodeGenerator.h"
#include "StandardFunctions.h"
#include "GlobalFunctions.h"
#include "CodeTemplates.h"
#include "Classes.h"
#include "Utils.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(CFunctionsCodeAnalyzer)
{
	m_outParamDeclared = false;
}

PROCESS_TOKENS_START(CFunctionsCodeAnalyzer)
	PROCESS_TOKEN(FUNC_DECL, ProcessFunctionDeclaration);
	PROCESS_TOKEN(FUNC_PARAMS, ProcessFunctionParameters);		
	PROCESS_TOKEN(OUT_PARAM, ProcessOutParameter);	
	PROCESS_TOKEN(PROP_IN_CALL, ProcessInnerPropertiesForCalls);
	PROCESS_TOKEN(GLOBAL_FUNC_DECL, ProcessGlobalFunctionDeclaration);
PROCESS_TOKENS_END

DECLARE_HANDLER(CFunctionsCodeAnalyzer::ProcessFunctionDeclaration)
{
	m_outParamStatus.clear();
	m_parametersDescription.clear();
	m_parametersCount = 0;
	CHANGE_STATE(functionParametersDeclaration, true);
	std::string paramsStr = GetCodeForNode(nd->center, context);
	RESTORE_STATE(functionParametersDeclaration);	

	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();
	assert(currentClass);
	if (currentClass != NULL)
	{		
		if ((m_parametersCount == 1) && (m_outParamStatus.size() == 0))
		{
			m_outParamStatus.push_back(m_outParamDeclared);
		}
		if (!currentClass->CheckSecondMethodDeclaration(nd->left->token, nd->line))
		{
			currentClass->AddMethod(nd->left->token, m_parametersCount, nd->line, m_outParamStatus, m_parametersDescription, false, "");
		}
	}

	return "";
}

DECLARE_HANDLER(CFunctionsCodeAnalyzer::ProcessFunctionParameters)
{
	m_parametersCount++;
	std::string txt = GetCodeForNode(nd->left, context);
	if ((nd->left->cmd == -1) || (nd->left->cmd == OUT_PARAM))
	{
		m_outParamStatus.push_back(m_outParamDeclared);
		m_parametersDescription.push_back(MethodParameter((nd->left->cmd == OUT_PARAM) ? nd->left->left->token : nd->left->token));
		m_outParamDeclared = false;
	}
	if (nd->center)
	{
		txt += (", " + GetCodeForNode(nd->center, context));
		m_outParamStatus.push_back(m_outParamDeclared);
		m_parametersDescription.push_back(MethodParameter((nd->center->cmd == OUT_PARAM) ? nd->center->left->token : nd->center->token));
		m_outParamDeclared = false;
	}
	return txt;		
}

DECLARE_HANDLER(CFunctionsCodeAnalyzer::ProcessOutParameter)
{
	m_outParamDeclared = true;
	return GetCodeForNode(nd->left, context);
}

std::string CFunctionsCodeAnalyzer::GetInnerProperty(node* nd, CompileContext& context)
{
	CHANGE_STATE(functionCallParameters, false);
	CHANGE_STATE(methodCallParameters, false);
	CHANGE_STATE(functionDeclaration, true);
	std::string prop = GetCodeForNode(nd, context);			
	RESTORE_STATE(functionDeclaration);
	RESTORE_STATE(methodCallParameters);
	RESTORE_STATE(functionCallParameters);

	return prop;
}

DECLARE_HANDLER(CFunctionsCodeAnalyzer::ProcessInnerPropertiesForCalls)
{
	if (nd->center == NULL)
	{				
		return GetInnerProperty(nd->left, context);
	}
		
	USE_TPL(functions, callSequence);
	USE_INDEX(callSequence, object);
	USE_INDEX(callSequence, property);
		
	callSequenceTpl->SetVariableValue(objectIndex, GetInnerProperty(nd->left, context));
	callSequenceTpl->SetVariableValue(propertyIndex, GetInnerProperty(nd->center, context));
		
	return callSequenceTpl->GetText();
}

std::string CFunctionsCodeAnalyzer::GetFunctionName(node* nd, CompileContext& context)
{
	CHANGE_STATE(methodCallParameters, false);
	CHANGE_STATE(functionDeclaration, true);	
	std::string name = GetCodeForNode(nd, context);
	RESTORE_STATE(functionDeclaration);
	RESTORE_STATE(methodCallParameters);

	return name;
}

DECLARE_HANDLER(CFunctionsCodeAnalyzer::ProcessGlobalFunctionDeclaration)
{
	std::string name = GetFunctionName(nd->left, context);
	Classes::GetInstance()->ClearActiveClass();

	m_outParamStatus.clear();
	m_parametersCount = 0;
	m_parametersDescription.clear();
	
	CHANGE_STATE(functionParametersDeclaration, true);
	GetCodeForNode(nd->center, context);	
	RESTORE_STATE(functionParametersDeclaration);	

	CGlobalFunctions::GetInstance()->AddFunction(name, nd->line, m_outParamStatus, m_parametersDescription, "");		

	return "";
}

std::vector<int> CFunctionsCodeAnalyzer::GetListOfSupportedTokens()const
{	
	std::vector<int> res;
	res.push_back(FUNC_DECL);
	res.push_back(FUNC_PARAMS);	
	res.push_back(OUT_PARAM);
	res.push_back(PROP_IN_CALL);
	res.push_back(GLOBAL_FUNC_DECL);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CFunctionsCodeAnalyzer)