
#include "Common.h"
#include "ConditionsCodeGenerator.h"
#include "ComplexStatementsManager.h"
#include "ErrorReporter.h"
#include "CodeTemplates.h"
#include "UnorderedVarDeclarations.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CConditionsCodeGenerator)

PROCESS_TOKENS_START(CConditionsCodeGenerator)
	PROCESS_TOKEN(OP_OR, ProcessOR);
	PROCESS_TOKEN(OP_AND, ProcessAND);
	PROCESS_TOKEN(OP_XOR, ProcessXOR);
	PROCESS_TOKEN(OP_IF, ProcessIF);
	PROCESS_TOKEN(OP_ELSE, ProcessElse);
	PROCESS_TOKEN(OP_SHORT_IF, ProcessShortIF);
PROCESS_TOKENS_END

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessOR)
{
	RETURN_FROM_TPL(conditions, or);	
}

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessAND)
{
	RETURN_FROM_TPL(conditions, and);	
}

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessXOR)
{
	RETURN_FROM_TPL(conditions, xor);	
}

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessElse)
{	
	if (!CComplexStatementsManager::GetInstance()->CanElseBeUsed())
	{
		CErrorReporter::GetInstance()->OnFatalError(INCORRECT_ELSE, nd->line);
	}

	CComplexStatementsManager::GetInstance()->MarkElseAsUsed();
	
	RETURN_FROM_TPL(conditions, elseOp);
}

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessIF)
{
	USE_TPL(conditions, condition);
	USE_INDEX(condition, condition);
	USE_INDEX(condition, unorderedDeclarations);

	conditionTpl->SetVariableValue(conditionIndex, GetCodeForNode(nd->left, context, true));
	std::string declString = CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString();
	CUnorderedVarDeclarations::GetInstance()->ClearAll();
	conditionTpl->SetVariableValue(unorderedDeclarationsIndex, declString);

	CComplexStatementsManager::GetInstance()->RegisterIF(nd->line, nd->tabLevel);

	return conditionTpl->GetText();		
}

DECLARE_HANDLER(CConditionsCodeGenerator::ProcessShortIF)
{
	USE_TPL(conditions, oneLineCondition);
	USE_INDEX(oneLineCondition, condition);
	USE_INDEX(oneLineCondition, leftValue);
	USE_INDEX(oneLineCondition, rightValue);

	oneLineConditionTpl->SetVariableValue(conditionIndex, GetCodeForNode(nd->left, context, true));
	oneLineConditionTpl->SetVariableValue(leftValueIndex, GetCodeForNode(nd->center, context, true));
	oneLineConditionTpl->SetVariableValue(rightValueIndex, GetCodeForNode(nd->right, context, true));	

	return oneLineConditionTpl->GetText();		
}

std::vector<int> CConditionsCodeGenerator::GetListOfSupportedTokens()const
{	
	std::vector<int> res;
	res.push_back(OP_OR);
	res.push_back(OP_AND);
	res.push_back(OP_XOR);
	res.push_back(OP_IF);
	res.push_back(OP_ELSE);
	res.push_back(OP_SHORT_IF);
	return res;
}

bool CConditionsCodeGenerator::IsCondition(const int cmd)
{
	return (cmd == CMP_COMPLEX) || (cmd == OP_OR) || (cmd == OP_AND) || (cmd == CMP_EQUAL) || (cmd == CMP_NOT_EQUAL)
		|| (cmd == CMP_LESS) || (cmd == CMP_MORE) || (cmd == CMP_LESS_EQUAL) || (cmd == CMP_MORE_EQUAL);
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CConditionsCodeGenerator)

