
#include "Common.h"
#include "MathCodeGenerator.h"
#include "GlobalSymbolEnumeration.h"
#include "CodeTemplates.h"
#include "Classes.h"
#include "GlobalFunctions.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CMathCodeGenerator)

PROCESS_TOKENS_START(CMathCodeGenerator)
	PROCESS_TOKEN(OP_MINUS, ProcessMinus);
	PROCESS_TOKEN(OP_UNARY_MINUS, ProcessUnaryMinus);
	PROCESS_TOKEN(OP_PLUS, ProcessPlus);
	PROCESS_TOKEN(OP_MULTIPLY, ProcessMultiply);
	PROCESS_TOKEN(OP_DIVISION, ProcessDivision);
	PROCESS_TOKEN(OP_MATH_AND, ProcessAnd);
	PROCESS_TOKEN(OP_MATH_OR, ProcessOr);
	PROCESS_TOKEN(OP_MATH_XOR, ProcessXor);
	PROCESS_TOKEN(OP_EQUAL, ProcessEqual);
	PROCESS_TOKEN(OP_DEC, ProcessDecrement);
	PROCESS_TOKEN(OP_INC, ProcessIncrement);	
	PROCESS_TOKEN(OP_BRACKETS, ProcessBrackets);
	PROCESS_TOKEN(OP_MINUS_EQUAL, ProcessMinusEqual);
	PROCESS_TOKEN(OP_PLUS_EQUAL, ProcessPlusEqual);
	PROCESS_TOKEN(OP_MULTIPLY_EQUAL, ProcessMultiplyEqual);
	PROCESS_TOKEN(OP_XOR_EQUAL, ProcessXorEqual);
	PROCESS_TOKEN(OP_OR_EQUAL, ProcessOrEqual);
	PROCESS_TOKEN(OP_AND_EQUAL, ProcessAndEqual);
	PROCESS_TOKEN(OP_DIVISION_EQUAL, ProcessDivisionEqual);
	PROCESS_TOKEN(OP_CHANGE_VALUE, ProcessChangeValue);
	PROCESS_TOKEN(OP_CONCAT, ProcessConcat)
PROCESS_TOKENS_END

std::string CMathCodeGenerator::ProcessBaseMathOperation(const std::string& operand1, const std::string& sign, const std::string& operand2)
{
	USE_TPL(math, baseOperation);
	USE_INDEX(baseOperation, operand1);
	USE_INDEX(baseOperation, sign);
	USE_INDEX(baseOperation, operand2);	

	baseOperationTpl->SetVariableValue(operand1Index, operand1);
	baseOperationTpl->SetVariableValue(signIndex, sign);
	baseOperationTpl->SetVariableValue(operand2Index, operand2);	

	return baseOperationTpl->GetText();
}

std::string CMathCodeGenerator::ProcessBaseUnaryMathOperation(const std::string& sign, const std::string& operand)
{
	USE_TPL(math, baseUnaryOperation);	
	USE_INDEX(baseUnaryOperation, sign);
	USE_INDEX(baseUnaryOperation, operand);	

	baseUnaryOperationTpl->SetVariableValue(signIndex, sign);
	baseUnaryOperationTpl->SetVariableValue(operandIndex, operand);	

	return baseUnaryOperationTpl->GetText();
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessPlus)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "+", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessMinus)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "-", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessUnaryMinus)
{
	return ProcessBaseUnaryMathOperation("-", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessAnd)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), " & ", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessXor)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "^", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessOr)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "|", GetCodeForNode(nd->center, context, true));
}

std::string CMathCodeGenerator::GetFunctionPointerCode(const std::string& name)
{
	USE_TPL(functions, functionPointer);
	USE_INDEX(functionPointer, name);
	functionPointerTpl->SetVariableValue(nameIndex, name);
	
	return functionPointerTpl->GetText();	
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessEqual)
{
	DEFINE_END_SIGN(nd->line);
	
	bool isRightValClass = (nd->center->cmd == -1) && (Classes::GetInstance()->IsClassExists(nd->center->token));
	
	if (isRightValClass)
	{	
		bool isNormalVariable = (nd->left->cmd != OP_MY_PROP) && (nd->left->cmd != OP_PROP) && (nd->left->cmd != ACCESS_ARRAY_ELEMENT);
		bool isNewVariable = false;
		if (isNormalVariable)
		{
			isNewVariable = context.currentSymbolTable->IsSymbolUsedFirstly(nd->left->token);
		}

		CHANGE_STATE(classInstanceCreationExists, true);
		std::string instanceName = GetCodeForNode(nd->left, context);
		std::string className = GetCodeForNode(nd->center, context, true);
		RESTORE_STATE(classInstanceCreationExists);			
				
		std::string realVariableName;
		if (nd->left->cmd == OP_MY_PROP)
		{
			CHANGE_STATE(functionDeclaration, true);
			realVariableName = GetCodeForNode(nd->left->left, context);
			RESTORE_STATE(functionDeclaration);
		}
		
		CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(className);
		assert(classInfo);
		if (classInfo->GetType() == CLASS_TYPE_SINGLETON)
		{
			CErrorReporter::GetInstance()->OnFatalError(CREATE_SINGLETON_INSTANCE, nd->line, instanceName, className);
		}

		CCodeTemplate* classInstanceTpl = NULL;

		if (!isNewVariable)
		{
			USE_TPL(math, classInstanceCreation);
			classInstanceTpl = classInstanceCreationTpl;
		}
		else
		{
			USE_TPL(math, classInstanceCreationNewVar);
			classInstanceTpl = classInstanceCreationNewVarTpl;
		}

		USE_INDEX(classInstance, className);
		USE_INDEX(classInstance, instanceName);
		USE_INDEX(classInstance, end);

		classInstanceTpl->SetVariableValue(classNameIndex, className);
		classInstanceTpl->SetVariableValue(instanceNameIndex, instanceName);
		classInstanceTpl->SetVariableValue(endIndex, end);

		if (nd->left->cmd == OP_MY_PROP)
		{			
			CGlobalSymbolEnumeration::GetInstance()->AddProperty(realVariableName, Classes::GetInstance()->GetCurrentClass()->GetPropertyType(instanceName) != PROP_PRIVATE);
		}
				
		context.currentSymbolTable->AddSymbol(instanceName);
		context.currentSymbolTable->MarkSymbolAsUsed(instanceName);
	
		return classInstanceTpl->GetText();		
	}
	else
	{		
		if (nd->center->cmd == OP_CONSTRUCTOR)
		{
			USE_TPL(classes, callConstructor);
			USE_INDEX(callConstructor, className);
			USE_INDEX(callConstructor, classInstanceName);
			USE_INDEX(callConstructor, objectName);	
			USE_INDEX(callConstructor, fullObjectName);	
			USE_INDEX(callConstructor, initCode);
			USE_INDEX(callConstructor, end);

			callConstructorTpl->SetVariableValue(initCodeIndex, GetCodeForNode(nd->center, context, true));
			// if this is new variable, then next line register it
			std::string fullObjName = GetCodeForNode(nd->left, context);
			// now we will get only name of variable
			if (nd->left->token != NULL)
			{
				CHANGE_STATE(functionDeclaration, true);			
			}
			std::string objName = GetCodeForNode(nd->left, context);
			callConstructorTpl->SetVariableValue(objectNameIndex, objName);
			callConstructorTpl->SetVariableValue(fullObjectNameIndex, fullObjName);
			if (nd->left->token != NULL)
			{
				RESTORE_STATE(functionDeclaration);
			}
			callConstructorTpl->SetVariableValue(classNameIndex, context.newClassName);
			callConstructorTpl->SetVariableValue(classInstanceNameIndex, context.newClassInstanceName);
			callConstructorTpl->SetVariableValue(endIndex, end);

			return callConstructorTpl->GetText();				
		}
		else
		{
			USE_TPL(math, changeValue);
			USE_INDEX(changeValue, operand1);
			USE_INDEX(changeValue, mathOperator);
			USE_INDEX(changeValue, operand2);
			USE_INDEX(changeValue, end);

			bool isFunctionPointer = false;
			bool isGlobalFunctionPointer = false;
			if ((nd->center->token) && (CGlobalFunctions::GetInstance()->IsFunctionDeclared(nd->center->token)))
			{
				isFunctionPointer = true;
				isGlobalFunctionPointer = true;
			}		
						
			if ((nd->center->cmd == OP_MY_PROP) || (nd->center->cmd == OP_PROP))
			{
				isFunctionPointer = true;				
			}

			context.functionPointerCompiling = isFunctionPointer;

			changeValueTpl->SetVariableValue(operand2Index, isGlobalFunctionPointer ? GetFunctionPointerCode(nd->center->token) : GetCodeForNode(nd->center, context, true));

			std::string leftPart = GetCodeForNode(nd->left, context);
			if ((nd->left->token) && (CGlobalFunctions::GetInstance()->IsFunctionDeclared(nd->left->token)))
			{
				CErrorReporter::GetInstance()->OnFatalError(GLOBAL_FUNCTION_CANNOT_BE_REPLACED_BY_FUNCTION_POINTER, nd->line, nd->left->token);
			}

			context.functionPointerCompiling = false;

			changeValueTpl->SetVariableValue(operand1Index, leftPart);
			changeValueTpl->SetVariableValue(mathOperatorIndex, "=");		
			changeValueTpl->SetVariableValue(endIndex, end);			

			return changeValueTpl->GetText();				
		}		
	}
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessMultiply)
{
	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "*", GetCodeForNode(nd->center, context, true));
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessDivision)
{
	if ((nd->center->token) && (strcmp(nd->center->token, "0") == 0))
	{
		CErrorReporter::GetInstance()->OnFatalError(DIVISION_BY_ZERO, nd->line, "");
	}

	return ProcessBaseMathOperation(GetCodeForNode(nd->left, context, true), "/", GetCodeForNode(nd->center, context, true));
}

std::string CMathCodeGenerator::ProcessUnaryOperation(const std::string& operand, const std::string& operation, const std::string& end)
{
	USE_TPL(math, unaryOperation);
	USE_INDEX(unaryOperation, operand);
	USE_INDEX(unaryOperation, mathOperator);
	USE_INDEX(unaryOperation, end);
	
	unaryOperationTpl->SetVariableValue(operandIndex, operand);
	unaryOperationTpl->SetVariableValue(mathOperatorIndex, operation);		
	unaryOperationTpl->SetVariableValue(endIndex, end);

	return unaryOperationTpl->GetText();	
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessDecrement)
{
    DEFINE_END_SIGN(nd->line);
	return ProcessUnaryOperation(GetCodeForNode(nd->left, context, true), "--", end);
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessIncrement)
{
    DEFINE_END_SIGN(nd->line);
	return ProcessUnaryOperation(GetCodeForNode(nd->left, context, true), "++", end);
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessBrackets)
{
	USE_TPL(math, brackets);
	USE_INDEX(brackets, expression);

	bracketsTpl->SetVariableValue(expressionIndex, GetCodeForNode(nd->left, context));
	
	return bracketsTpl->GetText();
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessAndEqual)
{
	return "^=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessOrEqual)
{
	return "|=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessXorEqual)
{
	return "^=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessMinusEqual)
{
	return "-=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessPlusEqual)
{
	return "+=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessMultiplyEqual)
{
	return "*=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessDivisionEqual)
{
	return "/=";
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessConcat)
{
	USE_TPL(math, changeValue);
	USE_INDEX(changeValue, operand1);
	USE_INDEX(changeValue, mathOperator);
	USE_INDEX(changeValue, operand2);
	USE_INDEX(changeValue, end);

	changeValueTpl->SetVariableValue(endIndex, GET_END_SIGN(nd->line));
	changeValueTpl->SetVariableValue(operand2Index, GetCodeForNode(nd->center, context, true));
	changeValueTpl->SetVariableValue(operand1Index, GetCodeForNode(nd->left, context, true));
	changeValueTpl->SetVariableValue(mathOperatorIndex, "%=");			

	return changeValueTpl->GetText();	
}

DECLARE_HANDLER(CMathCodeGenerator::ProcessChangeValue)
{
	if ((nd->center->cmd == OP_DIVISION_EQUAL) && (nd->right->token) && (strcmp(nd->right->token, "0") == 0))
	{
		CErrorReporter::GetInstance()->OnFatalError(DIVISION_BY_ZERO, nd->line, "");
	}

	USE_TPL(math, changeValue);
	USE_INDEX(changeValue, operand1);
	USE_INDEX(changeValue, mathOperator);
	USE_INDEX(changeValue, operand2);
	USE_INDEX(changeValue, end);

	changeValueTpl->SetVariableValue(endIndex, GET_END_SIGN(nd->line));
	changeValueTpl->SetVariableValue(operand2Index, GetCodeForNode(nd->right, context, true));
	changeValueTpl->SetVariableValue(operand1Index, GetCodeForNode(nd->left, context, true));
	changeValueTpl->SetVariableValue(mathOperatorIndex, GetCodeForNode(nd->center, context));			

	return changeValueTpl->GetText();
}

std::vector<int> CMathCodeGenerator::GetListOfSupportedTokens()const
{	
	std::vector<int> res;
	res.push_back(OP_MINUS);
	res.push_back(OP_PLUS);
	res.push_back(OP_EQUAL);
	res.push_back(OP_DEC);
	res.push_back(OP_INC);
	res.push_back(OP_MULTIPLY);
	res.push_back(OP_DIVISION);
	res.push_back(OP_BRACKETS);
	res.push_back(OP_MINUS_EQUAL);
	res.push_back(OP_PLUS_EQUAL);
	res.push_back(OP_MULTIPLY_EQUAL);
	res.push_back(OP_DIVISION_EQUAL);
	res.push_back(OP_CHANGE_VALUE);
	res.push_back(OP_CONCAT);
	res.push_back(OP_MATH_AND);
	res.push_back(OP_MATH_OR);
	res.push_back(OP_MATH_XOR);
	res.push_back(OP_AND_EQUAL);
	res.push_back(OP_OR_EQUAL);
	res.push_back(OP_XOR_EQUAL);
	res.push_back(OP_UNARY_MINUS);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CMathCodeGenerator)