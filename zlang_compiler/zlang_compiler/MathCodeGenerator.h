
#pragma once

#include "BaseCodeGenerator.h"

class CMathCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CMathCodeGenerator)

	DECLARE_HANDLER(ProcessPlus);
	DECLARE_HANDLER(ProcessMinus);
	DECLARE_HANDLER(ProcessUnaryMinus);
	DECLARE_HANDLER(ProcessEqual);
	DECLARE_HANDLER(ProcessDecrement);
	DECLARE_HANDLER(ProcessIncrement);
	DECLARE_HANDLER(ProcessMultiply);
	DECLARE_HANDLER(ProcessDivision);
	DECLARE_HANDLER(ProcessBrackets);
	DECLARE_HANDLER(ProcessMinusEqual);
	DECLARE_HANDLER(ProcessPlusEqual);
	DECLARE_HANDLER(ProcessMultiplyEqual);
	DECLARE_HANDLER(ProcessDivisionEqual);
	DECLARE_HANDLER(ProcessChangeValue);
	DECLARE_HANDLER(ProcessConcat);
	DECLARE_HANDLER(ProcessAnd);
	DECLARE_HANDLER(ProcessOr);
	DECLARE_HANDLER(ProcessXor);
	DECLARE_HANDLER(ProcessAndEqual);
	DECLARE_HANDLER(ProcessOrEqual);
	DECLARE_HANDLER(ProcessXorEqual);

private:
	std::string ProcessBaseMathOperation(const std::string& operand1, const std::string& sign, const std::string& operand2);
	std::string ProcessUnaryOperation(const std::string& operand, const std::string& operation, const std::string& end);
	std::string ProcessBaseUnaryMathOperation(const std::string& sign, const std::string& operand);
	std::string GetFunctionPointerCode(const std::string& name);
};