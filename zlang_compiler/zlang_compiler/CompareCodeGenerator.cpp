
#include "Common.h"
#include "CompareCodeGenerator.h"
#include "CodeTemplates.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CCompareCodeGenerator)

PROCESS_TOKENS_START(CCompareCodeGenerator)
	PROCESS_TOKEN(CMP_EQUAL, ProcessEqualCmp);
	PROCESS_TOKEN(CMP_NOT_EQUAL, ProcessNotEqualCmp);
	PROCESS_TOKEN(CMP_LESS, ProcessLessCmp);
	PROCESS_TOKEN(CMP_MORE, ProcessMoreCmp);
	PROCESS_TOKEN(CMP_LESS_EQUAL, ProcessLessEqualCmp);
	PROCESS_TOKEN(CMP_MORE_EQUAL, ProcessMoreEqualCmp);
	PROCESS_TOKEN(CMP_COMPLEX, ProcessComplexCmp);	
PROCESS_TOKENS_END

std::string CCompareCodeGenerator::GetComparisonString(node* nd, CompileContext& context, const std::string& cmpSign)
{
	USE_TPL(compare, simpleComparison);
	USE_INDEX(simpleComparison, leftValue);
	USE_INDEX(simpleComparison, compareSign);
	USE_INDEX(simpleComparison, rightValue);

	std::string left = GetCodeForNode(nd->left, context, true);
	std::string right = GetCodeForNode(nd->center, context, true);

	simpleComparisonTpl->SetVariableValue(leftValueIndex, left);
	simpleComparisonTpl->SetVariableValue(compareSignIndex, cmpSign);
	simpleComparisonTpl->SetVariableValue(rightValueIndex, right);

	return simpleComparisonTpl->GetText();
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessEqualCmp)
{
	return GetComparisonString(nd, context, "=="); 	
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessNotEqualCmp)
{
	return GetComparisonString(nd, context, "!="); 	
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessLessCmp)
{
	return GetComparisonString(nd, context, "<"); 	
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessMoreCmp)
{
	return GetComparisonString(nd, context, ">"); 	
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessLessEqualCmp)
{
	return GetComparisonString(nd, context, "<="); 		
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessMoreEqualCmp)
{
	return GetComparisonString(nd, context, ">="); 		
}

DECLARE_HANDLER(CCompareCodeGenerator::ProcessComplexCmp)
{
	USE_TPL(compare, simpleComparison);
	USE_INDEX(simpleComparison, leftValue);
	USE_INDEX(simpleComparison, compareSign);
	USE_INDEX(simpleComparison, rightValue);

	std::string right = GetCodeForNode(nd->right, context, true);
	std::string sign = GetCodeForNode(nd->center, context, true);
	std::string left = GetCodeForNode(nd->left, context, true);

	simpleComparisonTpl->SetVariableValue(leftValueIndex, left);
	simpleComparisonTpl->SetVariableValue(compareSignIndex, sign);
	simpleComparisonTpl->SetVariableValue(rightValueIndex, right);

	return simpleComparisonTpl->GetText();	
}

std::vector<int> CCompareCodeGenerator::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(CMP_EQUAL);
	res.push_back(CMP_NOT_EQUAL);
	res.push_back(CMP_LESS);
	res.push_back(CMP_MORE);
	res.push_back(CMP_COMPLEX);
	res.push_back(CMP_LESS_EQUAL);
	res.push_back(CMP_MORE_EQUAL);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CCompareCodeGenerator)

