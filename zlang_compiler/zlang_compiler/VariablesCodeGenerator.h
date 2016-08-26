#pragma once

#include "BaseCodeGenerator.h"
#include "CompileTarget.h"
#include "CodeTemplate.h"

enum VarRepresentation
{
	VR_USUAL,
	VR_PARAMETER,
	VR_DECLARATION,
	VR_CONSTRUCTION,
	VR_CONVERT_TO_POINTER,
	VR_COMPLEX_CONVERT_TO_POINTER,
	VR_SINGLETON_USE,
	VR_PARAMETER_AS_POINTER,
	REPRESENTATION_COUNT
};

class CVariablesCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CVariablesCodeGenerator)

	DECLARE_HANDLER(ProcessVariable);
	DECLARE_HANDLER(ProcessAccessArrayElement);
	DECLARE_HANDLER(ProcessArbitraryFloatDeclaration);
	DECLARE_HANDLER(ProcessArbitraryFixedDeclaration);
	
	void CheckIfSymbolCannotBeUsedAsVariable(node* nd, bool permitSingletons = false);	

	CCodeTemplate* m_varTemplates[REPRESENTATION_COUNT];	

private:
	bool IsNumeric(const char* const token)const;
	bool IsSymbol(const char* const token)const;
	bool IsString(const char* const token)const;
	bool IsBoolean(const char* const token)const;

	bool IsHexHumeric(const char* const token)const;
	bool IsFloatHumeric(const char* const token)const;
	bool IsSimpleHumeric(const char* const token)const;
	bool IsArbitraryPrecisionFloatHumeric(const char* const token)const;

	std::string GetArbitraryPrecisionFloatCode(const std::string& token);

	ZTokenType GetTokenType(const char* const token)const;
	std::string PatchSimpleIntegerToFloat(const std::string& token);
	std::string PatchHexIntegerToFloat(const std::string& token);

public:
	std::string GetVariableCode(const std::string& token, const VarRepresentation representation);	

	bool IsNonVariable(const char* const token)const;

	static CVariablesCodeGenerator* GetInstance(CCodeGenerator* codeGenerator);
};