
#include "Common.h"
#include "CyclesCodeGenerator.h"
#include "UnorderedVarDeclarations.h"
#include "ComplexStatementsManager.h"
#include "CodeTemplates.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CCyclesCodeGenerator)

PROCESS_TOKENS_START(CCyclesCodeGenerator)
	PROCESS_TOKEN(OP_WHILE, ProcessWhile);
	PROCESS_TOKEN(OP_FOR, ProcessFor);
	PROCESS_TOKEN(OP_COMMA, ProcessCommaSeparatedCommands);	
	PROCESS_TOKEN(OP_FOREACH, ProcessForEach);
	PROCESS_TOKEN(OP_BREAK, ProcessBreak);
	PROCESS_TOKEN(OP_CONTINUE, ProcessContinue);
PROCESS_TOKENS_END

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessBreak)
{
	if (!CComplexStatementsManager::GetInstance()->CanCycleOperatorBeUsed(nd->tabLevel))
	{
		CErrorReporter::GetInstance()->OnFatalError(INCORRECT_BREAK, nd->line);
	}

	RETURN_FROM_TPL(cycles, breakOp);	
}

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessContinue)
{
	if (!CComplexStatementsManager::GetInstance()->CanCycleOperatorBeUsed(nd->tabLevel))
	{
		CErrorReporter::GetInstance()->OnFatalError(INCORRECT_CONTINUE, nd->line);
	}

	RETURN_FROM_TPL(cycles, continueOp);	
}

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessWhile)
{
	USE_TPL(cycles, whileCycle);
	USE_INDEX(whileCycle, condition);

	CComplexStatementsManager::GetInstance()->RegisterCycle(nd->line, nd->tabLevel);
	
	whileCycleTpl->SetVariableValue(conditionIndex, GetCodeForNode(nd->left, context, true));
	std::string declString = CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString();
	CUnorderedVarDeclarations::GetInstance()->ClearAll();
	return declString + whileCycleTpl->GetText();	
}

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessFor)
{
	CUnorderedVarDeclarations::GetInstance()->ClearAll();
	CUnorderedVarDeclarations::GetInstance()->SetScopeLevel(nd->tabLevel);

	USE_TPL(cycles, forCycle);
	USE_INDEX(forCycle, initCode);
	USE_INDEX(forCycle, condition);
	USE_INDEX(forCycle, iterationCode);
	USE_INDEX(forCycle, unorderedDeclarations);

	forCycleTpl->SetVariableValue(initCodeIndex, GetCodeForNode(nd->left, context));
	forCycleTpl->SetVariableValue(conditionIndex, GetCodeForNode(nd->center, context, true));
	forCycleTpl->SetVariableValue(iterationCodeIndex,  GetCodeForNode(nd->right->left, context, true));	
	forCycleTpl->SetVariableValue(unorderedDeclarationsIndex, CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString());
	
	CUnorderedVarDeclarations::GetInstance()->ClearAll();

	CComplexStatementsManager::GetInstance()->RegisterCycle(nd->line, nd->tabLevel);

	return forCycleTpl->GetText();	
}

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessForEach)
{
	std::string cycleVar = GetCodeForNode(nd->left, context);
	std::string cycleHashtable = GetCodeForNode(nd->center, context);
	std::string cycleVarName = GetCodeForNode(nd->left, context);
	context.currentSymbolTable->MoveSymbolToHigherLevel(cycleVarName);

	if (cycleVarName == cycleHashtable)
	{
		CErrorReporter::GetInstance()->OnFatalError(USE_EQUAL_HASHTABLE_AND_CYCLE_VAR, nd->line, cycleVarName);
	}

	USE_TPL(cycles, forEachHeader);
	USE_INDEX(forEachHeader, varName);
	USE_INDEX(forEachHeader, map);

	forEachHeaderTpl->SetVariableValue(varNameIndex, cycleVarName);
	forEachHeaderTpl->SetVariableValue(mapIndex, cycleHashtable);

	std::string footerCode, iterationCode;
	{
		USE_TPL(cycles, forEachFooter);
		USE_INDEX(forEachFooter, varName);
		USE_INDEX(forEachFooter, map);

		forEachFooterTpl->SetVariableValue(varNameIndex, cycleVarName);
		forEachFooterTpl->SetVariableValue(mapIndex, cycleHashtable);
		footerCode = forEachFooterTpl->GetText();
	}
	
	{
		USE_TPL(cycles, forEachIteration);
		USE_INDEX(forEachIteration, map);
		USE_INDEX(forEachIteration, var);
		USE_INDEX(forEachIteration, varName);

		forEachIterationTpl->SetVariableValue(mapIndex, cycleHashtable);
		forEachIterationTpl->SetVariableValue(varIndex, cycleVar);
		forEachIterationTpl->SetVariableValue(varNameIndex, cycleVarName);
		iterationCode = forEachIterationTpl->GetText();
	}

	if (!CComplexStatementsManager::GetInstance()->CanCycleVarBeUsed(cycleVarName))
	{
		CErrorReporter::GetInstance()->OnFatalError(USE_CYCLE_VAR_IN_SECOND_FOREACH, nd->line, cycleVarName);
	}

	CComplexStatementsManager::GetInstance()->RegisterForEach(nd->line, nd->tabLevel, iterationCode, footerCode, cycleVarName);
	CComplexStatementsManager::GetInstance()->RegisterCycle(nd->line, nd->tabLevel);
		
	return forEachHeaderTpl->GetText();	
}

DECLARE_HANDLER(CCyclesCodeGenerator::ProcessCommaSeparatedCommands)
{
	USE_TPL(cycles, commaCommands);
	USE_INDEX(commaCommands, firstPart);
	USE_INDEX(commaCommands, secondPart);

	CHANGE_STATE(handlingCommaOperations, true);

	std::string code;
	if (nd->center)
	{
		std::string continuePart = GetCodeForNode(nd->center, context);	
		commaCommandsTpl->SetVariableValue(firstPartIndex, GetCodeForNode(nd->left, context));
		commaCommandsTpl->SetVariableValue(secondPartIndex, continuePart);
		code = commaCommandsTpl->GetText();
	}
	else
	{
		code = GetCodeForNode(nd->left, context);
	}

	RESTORE_STATE(handlingCommaOperations);

	return code;
}

std::vector<int> CCyclesCodeGenerator::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(OP_WHILE);
	res.push_back(OP_FOR);
	res.push_back(OP_COMMA);
	res.push_back(OP_FOREACH);
	res.push_back(OP_BREAK);
	res.push_back(OP_CONTINUE);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CCyclesCodeGenerator)