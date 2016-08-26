
#pragma once

#include "BaseCodeGenerator.h"

class CCompareCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CCompareCodeGenerator)

	DECLARE_HANDLER(ProcessEqualCmp);
	DECLARE_HANDLER(ProcessNotEqualCmp);
	DECLARE_HANDLER(ProcessLessCmp);
	DECLARE_HANDLER(ProcessMoreCmp);
	DECLARE_HANDLER(ProcessLessEqualCmp);
	DECLARE_HANDLER(ProcessMoreEqualCmp);
	DECLARE_HANDLER(ProcessComplexCmp);	

	std::string GetComparisonString(node* nd, CompileContext& context, const std::string& cmpSign);
};