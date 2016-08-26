
#include "Common.h"
#include "VariablesCodeGenerator.h"
#include <cstdlib>
#include "UnorderedVarDeclarations.h"
#include "StandardFunctions.h"
#include "CodeTemplates.h"
#include "ErrorReporter.h"
#include "Classes.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(CVariablesCodeGenerator)
{
	m_varTemplates[VR_USUAL] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varUsual.tpl");
	m_varTemplates[VR_PARAMETER] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varParameter.tpl");
	m_varTemplates[VR_DECLARATION] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varDeclaration.tpl");
	m_varTemplates[VR_CONSTRUCTION] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varConstruction.tpl");
	m_varTemplates[VR_CONVERT_TO_POINTER] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varConvertToPointer.tpl");
	m_varTemplates[VR_COMPLEX_CONVERT_TO_POINTER] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varComplexConvertToPointer.tpl");
	m_varTemplates[VR_SINGLETON_USE] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varSingletonUse.tpl");
	m_varTemplates[VR_PARAMETER_AS_POINTER] = CCodeTemplates::GetInstance()->LoadTemplate("code_templates\\variables\\varParameterAsPointer.tpl");	
}

PROCESS_TOKENS_START(CVariablesCodeGenerator)
	PROCESS_TOKEN(ACCESS_ARRAY_ELEMENT, ProcessAccessArrayElement);
	PROCESS_TOKEN(OP_FLOAT_DECL, ProcessArbitraryFloatDeclaration);
	PROCESS_TOKEN(OP_FIXED_DECL, ProcessArbitraryFixedDeclaration);
	// it's no command token. No command == variable
	PROCESS_TOKEN(-1, ProcessVariable);	
PROCESS_TOKENS_END	

ZTokenType CVariablesCodeGenerator::GetTokenType(const char* const token)const
{
	if (IsSimpleHumeric(token))
	{
		return TT_INTEGER;
	}
	if (IsBoolean(token))
	{
		return TT_BOOLEAN;
	}
	if (IsSymbol(token))
	{
		return TT_SYMBOL;
	}
	if (IsString(token))
	{
		return TT_STRING;
	}
	if (IsFloatHumeric(token))
	{
		return TT_FLOAT;
	}
	if (IsArbitraryPrecisionFloatHumeric(token))
	{
		return TT_ARB_FLOAT;
	}
	if (IsHexHumeric(token))
	{
		return TT_HEX_INTEGER;
	}

	return TT_VARIABLE;
}

bool CVariablesCodeGenerator::IsSimpleHumeric(const char* const token)const
{
	unsigned int len = strlen(token);

	unsigned int startOffset = ((token[0] == '-') || (token[0] == '+')) ? 1 : 0;

	for (unsigned int i = startOffset; i < len; i++)
	if ((token[i] < '0') || (token[i] > '9'))
	{		
		return false;
	}

	return true;
}

bool CVariablesCodeGenerator::IsNonVariable(const char* const token)const
{
	return GetTokenType(token) != TT_VARIABLE;
}

bool CVariablesCodeGenerator::IsBoolean(const char* const token)const
{
	return ((strcmp(token, "true") == 0) || (strcmp(token, "false") == 0));
}

bool CVariablesCodeGenerator::IsFloatHumeric(const char* const token)const
{
	unsigned int len = strlen(token);
	unsigned int dotsCount = 0;

	unsigned int startOffset = ((token[0] == '-') || (token[0] == '+')) ? 1 : 0;

	for (unsigned int i = startOffset; i < len; i++)
	if ((token[i] < '0') || (token[i] > '9'))
	{		
		if ((token[i] == '.') && (0 == dotsCount))
		{
			dotsCount++;			
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CVariablesCodeGenerator::IsArbitraryPrecisionFloatHumeric(const char* const token)const
{
	unsigned int len = strlen(token);
	if ((len == 0) || (token[0] != '~'))
	{
		return false;
	}

	return IsFloatHumeric(token + 1);
}

bool CVariablesCodeGenerator::IsHexHumeric(const char* const token)const
{
	unsigned int len = strlen(token);

	if ((len % 2 != 0) || (len < 4) || (len > 10))
	{
		return false;
	}

	if ((token[0] != '0') || (token[1] != 'x'))
	{
		return false;
	}	

	for (unsigned int i = 2; i < len; i++)
	if (!(((token[i] >= '0') && (token[i] <= '9')) || ((token[i] >= 'a') && (token[i] <= 'f')) || ((token[i] >= 'A') && (token[i] <= 'F'))))	
	{
		return false;
	}

	return true;
}

bool CVariablesCodeGenerator::IsNumeric(const char* const token)const
{
	if (IsSimpleHumeric(token))
	{
		return true;
	}

	if (IsArbitraryPrecisionFloatHumeric(token))
	{
		return true;
	}
	
	if (IsFloatHumeric(token))
	{
		return true;
	}

	if (IsHexHumeric(token))
	{
		return true;
	}

	return false;
}

bool CVariablesCodeGenerator::IsSymbol(const char* const token)const
{
	int len = strlen(token);
	if ((len == 3) && (token[0] == '\'') && (token[2] == '\''))
	{
		return true;
	}
	
	return false;
}

bool CVariablesCodeGenerator::IsString(const char* const token)const
{
	int len = strlen(token);
	if ((len >= 2) && (token[0] == '"') && (token[len - 1] == '"'))
	{
		return true;
	}
	
	return false;
}

DECLARE_HANDLER(CVariablesCodeGenerator::ProcessArbitraryFloatDeclaration)
{
	DEFINE_END_SIGN(nd->line);

	std::string precision = nd->left->token;
	if ((!IsNumeric(precision.c_str())) || (atoi(precision.c_str()) < 16))
	{
		CErrorReporter::GetInstance()->OnFatalError(PRECISION_MUST_BE_POSITIVE_INT, nd->line, nd->left->token);
	}
	
	bool noDefaultValue = true;
	bool defaultValueIsNumericConst = false;
	if (nd->center)
	{
		noDefaultValue = false;
		if (nd->center->token)
		{
			defaultValueIsNumericConst = IsNumeric(nd->center->token);
		}		
	}

	if (noDefaultValue)
	{
		USE_TPL(variables, declFloatNoDefault);
		USE_INDEX(declFloatNoDefault, precision);	
		USE_INDEX(declFloatNoDefault, end);	
		declFloatNoDefaultTpl->SetVariableValue(precisionIndex, precision);	
		declFloatNoDefaultTpl->SetVariableValue(endIndex, end);	
		return declFloatNoDefaultTpl->GetText();
	}
	else
	{
		if (defaultValueIsNumericConst)
		{
			USE_TPL(variables, declFloatDefaultConst);
			USE_INDEX(declFloatDefaultConst, precision);	
			USE_INDEX(declFloatDefaultConst, end);	
			USE_INDEX(declFloatDefaultConst, value);	
			declFloatDefaultConstTpl->SetVariableValue(precisionIndex, precision);	
			declFloatDefaultConstTpl->SetVariableValue(valueIndex, nd->center->token);	
			declFloatDefaultConstTpl->SetVariableValue(endIndex, end);	
			return declFloatDefaultConstTpl->GetText();
		}
		else
		{
			USE_TPL(variables, declFloatDefaultVar);
			USE_INDEX(declFloatDefaultVar, precision);	
			USE_INDEX(declFloatDefaultVar, value);	
			USE_INDEX(declFloatDefaultVar, end);	
			declFloatDefaultVarTpl->SetVariableValue(precisionIndex, precision);	
			declFloatDefaultVarTpl->SetVariableValue(valueIndex, GetCodeForNode(nd->center, context));	
			declFloatDefaultVarTpl->SetVariableValue(endIndex, end);	
			return declFloatDefaultVarTpl->GetText();
		}
	}
}

DECLARE_HANDLER(CVariablesCodeGenerator::ProcessArbitraryFixedDeclaration)
{
	return "";
}

DECLARE_HANDLER(CVariablesCodeGenerator::ProcessAccessArrayElement)
{
	if (nd->center)
	{
		USE_TPL(variables, arrayElement);
		USE_INDEX(arrayElement, arrayName);
		USE_INDEX(arrayElement, elementId);
		
		std::string index = GetCodeForNode(nd->left, context, true);
		arrayElementTpl->SetVariableValue(elementIdIndex, GetCodeForNode(nd->center, context, true));		
		arrayElementTpl->SetVariableValue(arrayNameIndex, index);
		
		return arrayElementTpl->GetText();	
	}
	else
	{
		if (context.newVariablesProhibited)
		{
			CErrorReporter::GetInstance()->OnFatalError(WRONG_ATTEMPT_TO_ADD_TO_ARRAY, nd->line, nd->left->token);
			return "";
		}
		else
		{
			USE_TPL(variables, arrayElementAdd);
			USE_INDEX(arrayElementAdd, arrayName);

			std::string index = GetCodeForNode(nd->left, context, true);
								
			arrayElementAddTpl->SetVariableValue(arrayNameIndex, index);
			return arrayElementAddTpl->GetText();	
		}
	}
}

std::string CVariablesCodeGenerator::GetArbitraryPrecisionFloatCode(const std::string& token)
{
	USE_TPL(variables, arbFloat);
	USE_INDEX(arbFloat, value);
	USE_INDEX(arbFloat, defaultPrecision);
	arbFloatTpl->SetVariableValue(valueIndex, (token[0] == '~') ? token.substr(1, token.size() - 1) : token);
	arbFloatTpl->SetVariableValue(defaultPrecisionIndex, CCompileTarget::GetInstance()->GetDefaultPrecision());

	return arbFloatTpl->GetText();
}

std::string CVariablesCodeGenerator::PatchSimpleIntegerToFloat(const std::string& token)
{
	USE_TPL(variables, intToFloat);
	USE_INDEX(intToFloat, value);	
	intToFloatTpl->SetVariableValue(valueIndex, token);	
	return intToFloatTpl->GetText();
}

std::string CVariablesCodeGenerator::PatchHexIntegerToFloat(const std::string& token)
{
	USE_TPL(variables, hexToFloat);
	USE_INDEX(hexToFloat, value);	
	hexToFloatTpl->SetVariableValue(valueIndex, token);	
	return hexToFloatTpl->GetText();
}

std::string CVariablesCodeGenerator::GetVariableCode(const std::string& token, const VarRepresentation representation)
{
	assert(m_varTemplates[representation]);

	if (representation == VR_CONSTRUCTION)
	{
		ZTokenType srcTokenType = GetTokenType(token.c_str());
		ZTokenType tokenType = CCompileTarget::GetInstance()->RefineTokenType(srcTokenType);
		
		if (tokenType == TT_ARB_FLOAT)
		{
			m_varTemplates[representation]->SetVariableValue(0, GetArbitraryPrecisionFloatCode(token));
		}
		else if ((tokenType == TT_FLOAT) && ((srcTokenType == TT_INTEGER) || (srcTokenType == TT_HEX_INTEGER)))
		{
			if (srcTokenType == TT_INTEGER)
			{				
				m_varTemplates[representation]->SetVariableValue(0, PatchSimpleIntegerToFloat(token));
			}
			else if (srcTokenType == TT_HEX_INTEGER)
			{				
				m_varTemplates[representation]->SetVariableValue(0, PatchHexIntegerToFloat(token));
			}
		}
		else 
		{
			m_varTemplates[representation]->SetVariableValue(0, token);
		}
	}
	else
	{
		m_varTemplates[representation]->SetVariableValue(0, token);
	}	
	
	return m_varTemplates[representation]->GetText();
}

void CVariablesCodeGenerator::CheckIfSymbolCannotBeUsedAsVariable(node* nd, bool permitSingletons)
{	
	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();	

	bool isStandardFunctionExists = CStandardFunctions::GetInstance()->IsFunctionExists(nd->token);
	bool isClassMethodExists = currentClass ? currentClass->IsMethodExist(nd->token) : false;
	bool isClassExists = Classes::GetInstance()->IsClassExists(nd->token);

	if ((isClassExists) && (permitSingletons))
	{
		CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(nd->token);
		assert(classInfo);
		if (classInfo->GetType() == CLASS_TYPE_SINGLETON)
		{
			isClassExists = false;
		}
	}

	if ((isStandardFunctionExists) || (isClassMethodExists) || (isClassExists))
	{		
		if (isStandardFunctionExists)
		{
			CErrorReporter::GetInstance()->OnFatalError(VARIABLE_WITH_STANDARD_FUNCTION_NAME, nd->line, nd->token);			
		}
		else if (isClassMethodExists)
		{
			CErrorReporter::GetInstance()->OnFatalError(VARIABLE_WITH_METHOD_NAME, nd->line, nd->token);			
		}
		else if (isClassExists)
		{
			CErrorReporter::GetInstance()->OnFatalError(VARIABLE_WITH_CLASS_NAME, nd->line, nd->token);		
		}
	}	
}
	
DECLARE_HANDLER(CVariablesCodeGenerator::ProcessVariable)
{
	if (!context.currentSymbolTable)
	{
		assert(false);
		return "";
	}

	if (strlen(nd->token) == 0)
	{
		return "";
	}

	bool isReallyVariable = !IsNonVariable(nd->token);
	bool noVarDeclaration = ((context.handlingCommaOperations) || (context.functionDeclaration));
	bool isFirstVarUse = isReallyVariable && (((context.currentSymbolTable->IsSymbolUsedFirstly(nd->token)) && (!context.varDeclarationExists) && (!context.classInstanceCreationExists)) || (context.functionParametersDeclaration));

	if ((!isFirstVarUse) && (context.currentSymbolTable->IsSymbolOutParameter(nd->token)))
	{
		SymbolDesc ss = context.currentSymbolTable->GetSymbolDesc(nd->token);
		/*if ((ss.isUsed != true) && (context.newVariablesProhibited))
		{
			//CErrorReporter::GetInstance()->OnWarning(WARNING_USE_POSSIBLE_UNINITIALIZED_OUT_PARAMETER, nd->line, nd->token);
		}*/
		if (ss.isUsed == false)
		{
			context.currentSymbolTable->MarkSymbolAsUsed(nd->token);
		}
	}

	if (context.functionCallOutParameters)
	{		
		if (!isReallyVariable)
		{
			CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_USE_CONST_AS_OUT_PARAMETER, nd->line, nd->token);
		}
		if ((isFirstVarUse) && (!context.functionDeclaration))
		{			
			context.currentSymbolTable->MarkSymbolAsUsed(nd->token);
			CUnorderedVarDeclarations::GetInstance()->AddVariable(nd->token);
		}
		isFirstVarUse = false;
	}

	if (((!context.functionDeclaration) && (!context.classInstanceCreationExists)) || (context.usingObject))
	{
		CheckIfSymbolCannotBeUsedAsVariable(nd, context.usingObject);
	}		

	if (context.functionDeclaration)
	{
		if (Classes::GetInstance()->IsClassExists(nd->token))
		{			
			CErrorReporter::GetInstance()->OnFatalError(METHOD_WITH_CLASS_NAME, nd->line, nd->token, "");			
		}
	}

	if (context.functionParametersDeclaration)
	{
		if (Classes::GetInstance()->GetCurrentClass())
		if (Classes::GetInstance()->GetCurrentClass()->IsPropertyExist(nd->token))
		{			
			CErrorReporter::GetInstance()->OnFatalError(PARAMETER_WITH_CLASS_PROPERTY_NAME, nd->line, nd->token);
		}
	}

	if (isFirstVarUse)
	{
		if (!context.functionDeclaration)
		{
			context.currentSymbolTable->MarkSymbolAsUsed(nd->token);
		}
		if (!noVarDeclaration)
		{
			if ((context.newVariablesProhibited) || (context.usingObject))
			{		
				CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(nd->token);
				if ((classInfo) && (classInfo->GetType() == CLASS_TYPE_SINGLETON))
				{
					return GetVariableCode(nd->token, VR_SINGLETON_USE);
				}
				else if (!context.currentSymbolTable->IsSymbolOutParameter(nd->token)) 
				{
					if (m_codeGenerator->GetMode() != ANALYZING)
					{
						context.currentSymbolTable->MarkSymbolAsUsed(nd->token);
						CUnorderedVarDeclarations::GetInstance()->AddVariable(nd->token);
						return GetVariableCode(nd->token, VR_USUAL);
					}
					//CErrorReporter::GetInstance()->OnFatalError(UNINITIALIZED_VARIABLE, nd->line, nd->token);				
				}
			}
									
			return GetVariableCode(nd->token, !context.functionParametersDeclaration ? VR_DECLARATION : VR_PARAMETER);
		}
		else
		{	
			if (context.handlingCommaOperations)
			{
				CUnorderedVarDeclarations::GetInstance()->AddVariable(nd->token);
				return GetVariableCode(nd->token, VR_USUAL);
			}
			else
			{
				return GetVariableCode(nd->token, VR_USUAL);
			}
		}
	}
	else
	{
		if (!context.methodCallParameters)
		{			
			if (isReallyVariable)
			{	
				return GetVariableCode(nd->token, VR_USUAL);
			}		
			else
			{
				return GetVariableCode(nd->token, VR_CONSTRUCTION);		
			}
		}
		else
		{						
			if (isReallyVariable)
			{	
				if (context.usingObject)
				{
					CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(nd->token);
					if ((classInfo) && (classInfo->GetType() == CLASS_TYPE_SINGLETON))
					{
						return GetVariableCode(nd->token, VR_SINGLETON_USE);
					}
				}

				return GetVariableCode(nd->token, VR_USUAL);				
			}
			else
			{
				return GetVariableCode(nd->token, VR_CONSTRUCTION);					
			}
		}
	}
}

std::vector<int> CVariablesCodeGenerator::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(-1);	
	res.push_back(ACCESS_ARRAY_ELEMENT);
	res.push_back(OP_FLOAT_DECL);
	res.push_back(OP_FIXED_DECL);
	return res;
}

CVariablesCodeGenerator* CVariablesCodeGenerator::GetInstance(CCodeGenerator* codeGenerator)
{
	static CVariablesCodeGenerator* obj = NULL;

	if (!obj)
	{
		obj = new CVariablesCodeGenerator(codeGenerator);
	}	

	return obj;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CVariablesCodeGenerator)

