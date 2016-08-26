
#include "Common.h"
#include "GlobalSymbolEnumeration.h"
#include "UnorderedVarDeclarations.h"
#include "FunctionsCodeGenerator.h"
#include "VariablesCodeGenerator.h"
#include "PredefinedVariables.h"
#include "ClassCodeGenerator.h"
#include "StandardFunctions.h"
#include "CodeTemplates.h"
#include "Classes.h"
#include "Utils.h"
#include "CurrentMethodAnnotation.h"
#include "GlobalFunctions.h"
#include "ReservedKeywords.h"
#include "ConditionsCodeGenerator.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(CFunctionsCodeGenerator)
{	
	m_outParamDeclared = false;
	m_maxUsedFunctionId = 0;
	m_classOfMethod = NULL;	
}

PROCESS_TOKENS_START(CFunctionsCodeGenerator)
	PROCESS_TOKEN(FUNC_DECL, ProcessFunctionDeclaration);
	PROCESS_TOKEN(GLOBAL_FUNC_DECL, ProcessGlobalFunctionDeclaration);
	PROCESS_TOKEN(FUNC_PARAMS, ProcessFunctionParameters);
	PROCESS_TOKEN(VALUE_TRUE, ProcessTRUE);
	PROCESS_TOKEN(VALUE_FALSE, ProcessFALSE);
	PROCESS_TOKEN(OP_RETURN, ProcessReturn);
	PROCESS_TOKEN(OP_RETURN_PROPS, ProcessReturnProperties);
	PROCESS_TOKEN(OP_METHOD_CALL, ProcessMethodCall);
	PROCESS_TOKEN(OP_BASE_METHOD_CALL, ProcessBaseMethodCall);
	PROCESS_TOKEN(OP_PROP_METHOD_CALL, ProcessPropMethodCall);
	PROCESS_TOKEN(OP_FUNCTION_CALL, ProcessFunctionCall);
	PROCESS_TOKEN(OP_FUNCTION_CALL_PARAMS, ProcessFunctionCallParameters);
	PROCESS_TOKEN(OP_FUNCTION_ALL_CALL_PARAMS, ProcessFunctionAllCallParameters);
	PROCESS_TOKEN(PROP_IN_CALL, ProcessInnerPropertiesForCalls);
	PROCESS_TOKEN(OUT_PARAM, ProcessOutParameter)	
	PROCESS_TOKEN(OP_PARAM_ANNOTATION, ProcessParamAnnotation);
	PROCESS_TOKEN(OP_PARAM_ANNOTATION_PROP, ProcessParamAnnotationProperty);
	PROCESS_TOKEN(OP_PARAM_ANNOTATION_PROP_VALUE, ProcessParamAnnotationPropertyValue);
PROCESS_TOKENS_END

std::string CFunctionsCodeGenerator::GetStartFunctionCode()
{
	USE_TPL(functions, start);
	USE_INDEX(start, line);
	USE_INDEX(start, classID);
	USE_INDEX(start, function);

	CClassInfo* classInfo = Classes::GetInstance()->GetCurrentClass();

	unsigned int line = classInfo ? Classes::GetInstance()->GetCurrentMethodDeclarationLine() :
		CGlobalFunctions::GetInstance()->GetCurrentFunctionDeclarationLine();

	CErrorReporter::GetInstance()->GetErrorLineInfo(line, NULL);

	std::string function = classInfo ? Classes::GetInstance()->GetCurrentMethod() :
		CGlobalFunctions::GetInstance()->GetCurrentFunction();

	int classID = classInfo ? classInfo->GetID() : -1;

	startTpl->SetVariableValue(lineIndex, line);
	startTpl->SetVariableValue(classIDIndex, classID);
	startTpl->SetVariableValue(functionIndex, function);

	return startTpl->GetText();
}

std::string CFunctionsCodeGenerator::GetFunctionName(node* nd, CompileContext& context)
{
	CHANGE_STATE(methodCallParameters, false);
	CHANGE_STATE(functionDeclaration, true);	
	std::string name = GetCodeForNode(nd, context);
	RESTORE_STATE(functionDeclaration);
	RESTORE_STATE(methodCallParameters);

	return name;
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessParamAnnotation)
{
	if (nd->left)
	{
		std::string paramName = nd->left->token;
		CCurrentMethodAnnotation::GetInstance()->SetCurrentParameter(paramName, nd->line);
	}
	else
	{
		CCurrentMethodAnnotation::GetInstance()->SetCurrentParameter("return", nd->line);
	}
	GetCodeForNode(nd->center, context);
	return "";
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessParamAnnotationProperty)
{
	std::string name = nd->left->token;
	m_annotationPropertyValues.clear();
	if (nd->center->token == NULL)
	{
		GetCodeForNode(nd->center, context);
	}
	else
	{
		m_annotationPropertyValues.push_back(UnquoteString(nd->center->token));
	}
		
	if (!CCurrentMethodAnnotation::GetInstance()->IsArrayAsValueNeeded(name))
	{
		if (!CCurrentMethodAnnotation::GetInstance()->SetProperty(name, m_annotationPropertyValues[0]))
		{
			CErrorReporter::GetInstance()->OnFatalError(ANNOTATION_INVALID_PARAMETER, nd->line, name);
		}
	}
	else
	{
		if (!CCurrentMethodAnnotation::GetInstance()->SetProperty(name, m_annotationPropertyValues))
		{
			CErrorReporter::GetInstance()->OnFatalError(ANNOTATION_INVALID_PARAMETER, nd->line, name);			
		}
	}

	return nd->right ? GetCodeForNode(nd->right, context) : "";
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessParamAnnotationPropertyValue)
{	
	m_annotationPropertyValues.push_back(nd->left->token);
	if (nd->center)
	{
		GetCodeForNode(nd->center, context);
	}
	return "";
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessGlobalFunctionDeclaration)
{
	std::string begin = "";
	if (Classes::GetInstance()->GetCurrentClass())
	{
		begin = CClassCodeGenerator::GetGeneratedCodeForCurrentClass();
	}

	USE_TPL(functions, globalDeclaration);
	USE_INDEX(globalDeclaration, methodName);
	USE_INDEX(globalDeclaration, parameters);
	USE_INDEX(globalDeclaration, parametersAsPtr);
	USE_INDEX(globalDeclaration, parametersForCall);
	
	std::string name = GetFunctionName(nd->left, context);
	globalDeclarationTpl->SetVariableValue(methodNameIndex, name);
	Classes::GetInstance()->ClearActiveClass();

	m_outParamStatus.clear();
	m_parametersCount = 0;
	m_parametersDescription.clear();
	
	CHANGE_STATE(functionParametersDeclaration, true);
	std::string paramsStr = GetCodeForNode(nd->center, context);
	if (paramsStr != "")
	{
		paramsStr = ", " + paramsStr;
	}
	globalDeclarationTpl->SetVariableValue(parametersIndex, paramsStr);
	StringReplace(paramsStr, "CUniVar&", "CUniVar*");
	globalDeclarationTpl->SetVariableValue(parametersAsPtrIndex, paramsStr);
	StringReplace(paramsStr, "CUniVar* ", "*");
	globalDeclarationTpl->SetVariableValue(parametersForCallIndex, paramsStr);
	RESTORE_STATE(functionParametersDeclaration);	

	std::vector<MethodParameter>::iterator it = m_parametersDescription.begin();
	std::vector<MethodParameter>::iterator itEnd = m_parametersDescription.end();

	for ( ; it != itEnd; it++)
	{
		ParameterAnnotation* annot = CCurrentMethodAnnotation::GetInstance()->FindParameterAnnotation((*it).name);
		if (annot)
		{
			(*it).group = annot->group;
			(*it).description = annot->description;
			(*it).valueType = annot->editor;
			(*it).validators = annot->validators;
			(*it).line = annot->startLine;
			(*it).min = annot->min;
			(*it).max = annot->max;
		}
	}

	if (CGlobalFunctions::GetInstance()->CheckSecondFunctionDeclaration(nd->left->token, nd->line))
	{
		CErrorReporter::GetInstance()->OnFatalError(MULTIPLE_FUNCTIONS_WITH_ONE_NAME, nd->left->line, nd->left->token);
	}
	else
	{
		if ((m_parametersCount != m_outParamStatus.size()) && (m_parametersCount == 1))
		{
			m_outParamStatus.push_back(m_outParamDeclared);
		}
		CGlobalFunctions::GetInstance()->AddFunction(name, nd->line, m_outParamStatus, m_parametersDescription, CCurrentMethodAnnotation::GetInstance()->GetDefaultReturnField());
		CGlobalFunctions::GetInstance()->SetCurrentFunction(name, nd->line);
	}

	assert(m_parametersCount == m_outParamStatus.size());
	
	CCurrentMethodAnnotation::GetInstance()->Reset();

	return begin + globalDeclarationTpl->GetText();
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFunctionDeclaration)
{
	USE_TPL(functions, declaration);
	USE_INDEX(declaration, methodName);
	USE_INDEX(declaration, parameters);
	USE_INDEX(declaration, className);
	
	std::string name = GetFunctionName(nd->left, context);
	declarationTpl->SetVariableValue(methodNameIndex, name);
	assert(Classes::GetInstance()->GetCurrentClass());
	declarationTpl->SetVariableValue(classNameIndex, Classes::GetInstance()->GetCurrentClass()->GetName());

	m_outParamStatus.clear();
	m_parametersCount = 0;
	m_parametersDescription.clear();
	
	CHANGE_STATE(functionParametersDeclaration, true);
	std::string paramsStr = GetCodeForNode(nd->center, context);
	if (paramsStr != "")
	{
		paramsStr = ", " + paramsStr;
	}
	declarationTpl->SetVariableValue(parametersIndex, paramsStr);
	RESTORE_STATE(functionParametersDeclaration);	

	std::vector<MethodParameter>::iterator it = m_parametersDescription.begin();
	std::vector<MethodParameter>::iterator itEnd = m_parametersDescription.end();

	for ( ; it != itEnd; it++)
	{
		ParameterAnnotation* annot = CCurrentMethodAnnotation::GetInstance()->FindParameterAnnotation((*it).name);
		if (annot)
		{
			(*it).group = annot->group;
			(*it).description = annot->description;
			(*it).valueType = annot->editor;
			(*it).validators = annot->validators;
			(*it).line = annot->startLine;
			(*it).min = annot->min;
			(*it).max = annot->max;
		}
	}
	
	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();
	assert(currentClass);
	if (currentClass != NULL)
	{
		if (currentClass->CheckSecondMethodDeclaration(nd->left->token, nd->line))
		{			
			CErrorReporter::GetInstance()->OnFatalError(MULTIPLE_METHODS_WITH_ONE_NAME, nd->left->line, nd->left->token, currentClass->GetName());
		}
		else
		{
			if ((m_parametersCount != m_outParamStatus.size()) && (m_parametersCount == 1))
			{
				m_outParamStatus.push_back(m_outParamDeclared);
			}
			currentClass->AddMethod(nd->left->token, m_parametersCount, nd->line, m_outParamStatus, m_parametersDescription, false, CCurrentMethodAnnotation::GetInstance()->GetDefaultReturnField());
			Classes::GetInstance()->SetCurrentMethod(nd->left->token, nd->left->line);
		}
	}

	assert(m_parametersCount == m_outParamStatus.size());

	if (Classes::GetInstance()->IsClassExists(nd->left->token))
	{
		CErrorReporter::GetInstance()->OnFatalError(METHOD_WITH_CLASS_NAME, nd->left->line, nd->left->token, currentClass->GetName());
	}

	CCurrentMethodAnnotation::GetInstance()->Reset();

	return declarationTpl->GetText();
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFunctionParameters)
{
	m_parametersCount++;

	std::string txt = GetCodeForNode(nd->left, context);	
	if ((nd->left->cmd == -1) || (nd->left->cmd == OUT_PARAM))
	{
		m_outParamStatus.push_back(m_outParamDeclared);
		std::string paramName = (nd->left->cmd == OUT_PARAM) ? nd->left->left->token : nd->left->token;		
		if (!CReservedKeywords::GetInstance()->CanBeSymbolUsedForFunctionParameter(paramName))
		{
			CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_USE_RESERVED_KEYWORD, nd->line, paramName);
		}
		m_parametersDescription.push_back(MethodParameter(paramName));
		m_outParamDeclared = false;		
	}
	if (nd->center)
	{
		txt += ", " + GetCodeForNode(nd->center, context);
		m_outParamStatus.push_back(m_outParamDeclared);
		std::string paramName = (nd->center->cmd == OUT_PARAM) ? nd->center->left->token : nd->center->token;		
		if (!CReservedKeywords::GetInstance()->CanBeSymbolUsedForFunctionParameter(paramName))
		{
			CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_USE_RESERVED_KEYWORD, nd->line, paramName);
		}
		m_parametersDescription.push_back(MethodParameter(paramName));
		m_outParamDeclared = false;
	}
	return txt;		
}

std::string CFunctionsCodeGenerator::GetResultPropertyInitCode(node* nd, CompileContext& context)
{
	std::string prop = nd->token;
	if (context.currentSymbolTable->IsSymbolUsedFirstly(prop))
	{
		CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_RETURN_UNKNOWN_VARIABLE, nd->line, prop);
	}

	CGlobalSymbolEnumeration::GetInstance()->AddProperty(prop, true);	
	
	USE_TPL(functions, resultPropertyInit);
	USE_INDEX(resultPropertyInit, prop);

	resultPropertyInitTpl->SetVariableValue(propIndex, prop);
	
	return resultPropertyInitTpl->GetText();	
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessReturnProperties)
{
	std::string code = GetResultPropertyInitCode(nd->left, context);

	if (nd->center)
	{
		code += GetCodeForNode(nd->center, context);
	}

	return code;
}

std::string CFunctionsCodeGenerator::GetReturnValue(node* nd, CompileContext& context)
{
	std::string val = GetCodeForNode(nd->left, context);

	if (nd->left->cmd == VALUE_TRUE)
	{
		RETURN_FROM_TPL(conditions, trueConst);		
	}
	else if (nd->left->cmd == VALUE_FALSE)
	{		
		RETURN_FROM_TPL(conditions, falseConst);
	}
	
	return val;
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessReturn)
{
	if (!nd->center)
	{
		USE_TPL(functions, returnCmd);
		USE_INDEX(returnCmd, value);
		returnCmdTpl->SetVariableValue(valueIndex, GetReturnValue(nd, context));
		return returnCmdTpl->GetText();		
	}
	else
	{
		std::string declarations = GetCodeForNode(nd->center, context);

		USE_TPL(functions, extReturnCmd);
		USE_INDEX(extReturnCmd, declarations);
		USE_INDEX(extReturnCmd, value);
		extReturnCmdTpl->SetVariableValue(valueIndex, GetReturnValue(nd, context));
		extReturnCmdTpl->SetVariableValue(declarationsIndex, declarations);
		return extReturnCmdTpl->GetText();		
	}
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessTRUE)
{
	RETURN_FROM_TPL(conditions, trueVal);	
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFALSE)
{
	RETURN_FROM_TPL(conditions, falseVal);	
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFunctionCall)
{
	bool isMainCmd = IS_FIRST_COMMAND_IN_LINE;
	DEFINE_END_SIGN(nd->line);

	CUnorderedVarDeclarations::GetInstance()->ClearAll();
	CUnorderedVarDeclarations::GetInstance()->SetScopeLevel(nd->tabLevel);

	CCodeTemplate* functionCallTpl = NULL;
		
	std::string functionName = GetFunctionName(nd->left, context);	
	
	bool errorExists = false;
	bool isGlobalFunction = false;
	if (!CStandardFunctions::GetInstance()->IsFunctionExists(functionName))
	{
		if (CGlobalFunctions::GetInstance()->IsFunctionDeclared(functionName))
		{
			isGlobalFunction = true;
		}
		else if (CPredefinedVariables::GetInstance()->IsPredefinedVariable(functionName))
		{
			nd->cmd = OP_FUNCTION_CALL_PARAMS;			
			return GetCodeForNode(nd, context);	
		}		
		else
		{
			CErrorReporter::GetInstance()->OnFatalError(CALL_NON_STANDARD_FUNCTION, nd->line, functionName);
			errorExists = true;
		}
	}

	if (!isGlobalFunction)
	{
		INIT_TPL(functionCall, functions, functionCall);
	}
	else
	{
		INIT_TPL(functionCall, functions, globalFunctionCall);
	}

	USE_INDEX(functionCall, functionName);
	USE_INDEX(functionCall, functionParameters);
	USE_INDEX(functionCall, end);
	USE_INDEX(functionCall, resultFieldPlaceholder);

	bool areOutParametersUsed = CStandardFunctions::GetInstance()->IsOutParametersUsed(functionName);		
	CHANGE_STATE(functionCallParameters, true);
	if (areOutParametersUsed)
	{
		CHANGE_STATE(functionCallOutParameters, true);
	}

	if (!isGlobalFunction)
	{
		CHANGE_STATE(standardFunctionCompiling, true);
	}
	else
	{
		CHANGE_STATE(globalFunctionCompiling, true);
		CHANGE_STATE(methodCallParameters, true);
	}
	
	m_maxUsedFunctionId++;	
	context.NewFunction(m_maxUsedFunctionId);
	GetCurrentFunctionInfo(context.GetCurrentFunctionId()).name = functionName;
	bool defaultResultNeeded = true;
	std::string functionParams = nd->center ? GetCodeForNode(nd->center, context, true) : "";
	if ((nd->center) && (nd->center->center != NULL))
	{
		defaultResultNeeded = false;
	}
	if ((isGlobalFunction) && (functionParams != ""))
	{
		functionParams = ", " + functionParams;
	}
	int parametersCount = GetCurrentFunctionInfo(context.GetCurrentFunctionId()).parametersCount;	
	std::string refinedFunctionName = functionName;
	if (context.standardFunctionCompiling)
	{
		refinedFunctionName = CStandardFunctions::GetInstance()->PatchFunctionName(functionName, parametersCount);
	}
	context.EndFunction();
	if (!isGlobalFunction)
	{
		RESTORE_STATE(standardFunctionCompiling);	
	}
	else
	{
		RESTORE_STATE(methodCallParameters);
		RESTORE_STATE(globalFunctionCompiling);	
	}
	RESTORE_STATE(functionCallParameters);
	if (areOutParametersUsed)
	{
		RESTORE_STATE(functionCallOutParameters);
	}	

	if (!errorExists)
	{
		if (!isGlobalFunction)
		{
			if (!CStandardFunctions::GetInstance()->ValidateFunctionParametersCount(functionName, parametersCount))
			{
				CErrorReporter::GetInstance()->OnFatalError(INVALID_PARAMETER_COUNT, nd->line, functionName, CStandardFunctions::GetInstance()->GetExpectedParameters(functionName));
			}
		}
		else
		{
			if (!CGlobalFunctions::GetInstance()->ValidateFunctionParametersCount(functionName, parametersCount))
			{
				CErrorReporter::GetInstance()->OnFatalError(INVALID_PARAMETER_COUNT, nd->line, functionName, CGlobalFunctions::GetInstance()->GetExpectedParameters(functionName));
			}
		}
	}

	if (!isGlobalFunction)
	{
		functionParams += CStandardFunctions::GetInstance()->GetAdditionalParameters(functionName, nd);
	}
	
	functionCallTpl->SetVariableValue(endIndex, end);
	functionCallTpl->SetVariableValue(functionNameIndex, refinedFunctionName);
	functionCallTpl->SetVariableValue(functionParametersIndex, functionParams);	

	if (defaultResultNeeded)
	{
		std::string placeholderCode = "";
		if (isGlobalFunction)
		{
			std::string defaultResultField = CGlobalFunctions::GetInstance()->GetFunctionDefaultResultField(functionName);
			if (defaultResultField == "")
			{
				ADD_FROM_TPL(functions, defaultResultField, placeholderCode);
			}
			else
			{
				USE_TPL(functions, customResultField);
				USE_INDEX(customResultField, fieldName);
				customResultFieldTpl->SetVariableValue(fieldNameIndex, defaultResultField);
				placeholderCode = customResultFieldTpl->GetText();
			}
		}
		functionCallTpl->SetVariableValue(resultFieldPlaceholderIndex, placeholderCode);	
	}
	else
	{
		functionCallTpl->SetVariableValue(resultFieldPlaceholderIndex, "");	
	}

	if (isMainCmd)
	{
		std::string declString = CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString();
		CUnorderedVarDeclarations::GetInstance()->ClearAll();
		return declString + functionCallTpl->GetText();
	}
	else
	{
		return functionCallTpl->GetText();
	}
}

void CFunctionsCodeGenerator::FixObjectNameIfNeeded(node* nd, CompileContext& context, std::string& objectName, CClassInfo*& classInfo)
{
	USE_TPL(classes, propertyOutsideClass);
	USE_INDEX(propertyOutsideClass, objectName);
	USE_INDEX(propertyOutsideClass, propertyName);
	USE_INDEX(propertyOutsideClass, end);
		
	std::string fullMethodName = GetCodeForNode(nd->center, context);
	int pos = fullMethodName.find_last_of(".");
	if (pos != std::string::npos)
	{
		std::vector<std::string> arr = SplitStringIntoVector(fullMethodName.substr(0, pos), '.');
		for (unsigned int i = 0; i < arr.size(); i++)
		{			
			if (!CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(arr[i]))
			{
				CErrorReporter::GetInstance()->OnFatalError(UNINITIALIZED_VARIABLE, nd->line, arr[i]);				
			}

			propertyOutsideClassTpl->SetVariableValue(objectNameIndex, "");
			propertyOutsideClassTpl->SetVariableValue(propertyNameIndex, arr[i]);
			propertyOutsideClassTpl->SetVariableValue(endIndex, "");
			objectName += propertyOutsideClassTpl->GetText();
		}
		classInfo = NULL;
	}
}

std::string CFunctionsCodeGenerator::GetMethodName(node* nd, CompileContext& context)
{
	CHANGE_STATE(functionCallParameters, false);
	CHANGE_STATE(methodCallParameters, false);		
	std::string methodName = GetFunctionName(nd, context);		
	RESTORE_STATE(methodCallParameters);
	RESTORE_STATE(functionCallParameters);

	return methodName;
}

std::string CFunctionsCodeGenerator::GetObjectName(node* nd, CompileContext& context, const TypeOfMethodCall typeOfCall)
{
	CHANGE_STATE(newVariablesProhibited, true);
	CHANGE_STATE(usingObject, true);
	std::string objectName = "";
	switch (typeOfCall)
	{
	case CALL_OUTSIDE_OBJECT:
		objectName = GetCodeForNode(nd->left, context);
		break;
	case CALL_INSIDE_OBJECT:
		objectName = MY_ID;
		break;
	case CALL_TO_BASE_OBJECT:
		objectName = BASE_ID;
		break;
	};
	RESTORE_STATE(usingObject);
	RESTORE_STATE(newVariablesProhibited);

	return objectName;
}

void CFunctionsCodeGenerator::HandlePossibleErrors(node* nd, std::string& methodName, CClassInfo* classInfo, MethodCallValidateError error)
{
	switch (error)
	{
    case OK:
        // just to shut up stupid clang
        break;
	case NO_METHOD:
		{
			bool isError = true;
			if (classInfo)
			{
				isError = !(classInfo->IsPropertyExist(methodName));
			}
			else
			{
				isError = !(CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(methodName));
			}

			if (isError)
			{
				if (nd->left != NULL)
				{
					if (classInfo)
					{
						CErrorReporter::GetInstance()->OnFatalError(UNKNOWN_METHOD_CALL_IN_SINGLETON, nd->line, methodName, classInfo->GetName());
					}
					else
					{
						CErrorReporter::GetInstance()->OnFatalError(UNKNOWN_METHOD_CALL, nd->line, methodName);
					}
				}		
				else
				{
					CErrorReporter::GetInstance()->OnFatalError(UNKNOWN_METHOD_CALL_INSIDE_CLASS, nd->line, methodName);
				}
			}
		}
		break;

	case PRIVATE_METHOD:					
		CErrorReporter::GetInstance()->OnFatalError(PRIVATE_METHOD_CALL, nd->line, methodName);
		break;

	case INVALID_PARAMETERS_COUNT:					
		CErrorReporter::GetInstance()->OnFatalError(INVALID_PARAMETERS_METHOD_CALL, nd->line, methodName);
		break;

	case ONLY_PRIVATE_METHOD_HAS_NEEDED_PARAMETERS_COUNT:
		CErrorReporter::GetInstance()->OnFatalError(INVALID_PARAMETERS_PUBLIC_METHOD_CALL, nd->line, methodName);
		break;
	}	
}

std::string CFunctionsCodeGenerator::ProcessMethodCallImpl(node* nd, CompileContext& context, const TypeOfMethodCall typeOfCall)
{
	bool isMainCmd = IS_FIRST_COMMAND_IN_LINE;
	
	CHANGE_STATE(methodCallParameters, true);
	
	if (isMainCmd)
	{
		CUnorderedVarDeclarations::GetInstance()->ClearAll();
		CUnorderedVarDeclarations::GetInstance()->SetScopeLevel(nd->tabLevel);
	}

	bool isFinalOperation = (context.highLevelCounter > 0);

	CCodeTemplate* methodCallTpl = NULL;

	DEFINE_END_SIGN(nd->line);
	std::string objectName = GetObjectName(nd, context, typeOfCall);

	if ((objectName == BASE_ID) && (nd->center->center == NULL))
	{
		INIT_TPL(methodCall, functions, localMethodCall);
	} 
	else
	{
		INIT_TPL(methodCall, functions, methodCall);
	}
	
	USE_INDEX(methodCall, objectName);
	USE_INDEX(methodCall, paramCount);
	USE_INDEX(methodCall, methodName);
	USE_INDEX(methodCall, methodParams);
	USE_INDEX(methodCall, end);
	USE_INDEX(methodCall, callStatus);
	USE_INDEX(methodCall, callOperator);
	USE_INDEX(methodCall, staticPostfix);
	USE_INDEX(methodCall, resultFieldPlaceholder);
	USE_INDEX(methodCall, defaultResultStatus);	

	bool isInnerCall = (nd->left == NULL);

	std::string methodName = GetMethodName(nd->center->center ? nd->center->center : nd->center->left, context);

	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();
	assert(currentClass);

	m_classOfMethod = NULL;
	if (objectName == MY_ID) 
	{
		m_classOfMethod = currentClass;
	}
	else if (objectName == BASE_ID) 
	{
		m_classOfMethod = currentClass->GetBaseClass();
	}
	
	if ((objectName == BASE_ID) && (currentClass->GetBaseClass() == NULL))
	{
		CErrorReporter::GetInstance()->OnFatalError(CALL_TO_NON_EXIST_BASE_CLASS, nd->line, methodName, currentClass->GetName());
	}	
	int parametersCount = 0;
	std::string functionParams;
	bool defaultResultNeeded = true;

	if (nd->right)
	{
		if (nd->right->center != NULL)
		{
			defaultResultNeeded = false;
		}

		CHANGE_STATE(globalFunctionCompiling, false);
		CHANGE_STATE(standardFunctionCompiling, false);		
		CHANGE_STATE(methodCallParameters, true);
		m_methodName = methodName;
		m_maxUsedFunctionId++;
		GetCurrentFunctionInfo(m_maxUsedFunctionId).parametersCount = 0;
		GetCurrentFunctionInfo(m_maxUsedFunctionId).name = methodName;
		context.NewFunction(m_maxUsedFunctionId);

		functionParams = GetCodeForNode(nd->right, context);
		if (functionParams.size() > 0)
		{
			functionParams = ", " + functionParams;	
		}
		
		parametersCount = GetCurrentFunctionInfo(context.GetCurrentFunctionId()).parametersCount;
		context.EndFunction();		
		RESTORE_STATE(methodCallParameters);		
		RESTORE_STATE(standardFunctionCompiling);
		RESTORE_STATE(globalFunctionCompiling);
	}

	m_classOfMethod = NULL;

	CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(objectName);

	FixObjectNameIfNeeded(nd, context, objectName, classInfo);

	MethodCallValidateError error = OK;
	bool isPropertyWithSameNameExists = false;
	
	if (classInfo)
	{		
		error = classInfo->ValidateMethodCall(methodName, parametersCount, isInnerCall);
		isPropertyWithSameNameExists = classInfo->IsPropertyExist(methodName);
	}
	else
	{
		error = CGlobalSymbolEnumeration::GetInstance()->ValidateUniversalMethodCall(methodName, parametersCount);
		isPropertyWithSameNameExists = CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(methodName);						
	}
	
	// we call method of single object
	if ((nd->left) && (nd->center) && (nd->center->left) && (!nd->center->left->left))
	{
		CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(nd->left->token);
		if ((classInfo) && (classInfo->GetType() == CLASS_TYPE_SINGLETON) && (classInfo->IsBuiltIn()))
		{
			HandlePossibleErrors(nd, methodName, classInfo, error);
			
			USE_TPL(classes, standardObjectMethodCall);
			USE_INDEX(standardObjectMethodCall, objectName);	
			USE_INDEX(standardObjectMethodCall, methodName);
			USE_INDEX(standardObjectMethodCall, methodParams);
			USE_INDEX(standardObjectMethodCall, end);

			standardObjectMethodCallTpl->SetVariableValue(objectNameIndex, nd->left->token);
			standardObjectMethodCallTpl->SetVariableValue(methodNameIndex, methodName);
			standardObjectMethodCallTpl->SetVariableValue(methodParamsIndex, functionParams.size() == 0 ? "" : functionParams.substr(1, functionParams.size() - 1));
			standardObjectMethodCallTpl->SetVariableValue(endIndex, end);
	
			return standardObjectMethodCallTpl->GetText();
		}
	}	

	HandlePossibleErrors(nd, methodName, classInfo, error);
	
	if (typeOfCall == CALL_TO_BASE_OBJECT)
	{
		objectName = Classes::GetInstance()->GetCurrentClass()->GetBaseProtoClassName();
	}
	
	methodCallTpl->SetVariableValue(objectNameIndex, CClassCodeGenerator::GetResultObjectName(objectName));
	methodCallTpl->SetVariableValue(paramCountIndex, parametersCount + ADD_PARAM_COUNT); // plus 2 because three additional params: count (parameters), callStatus (public/private), default parameter of return object or not (true/false)
	methodCallTpl->SetVariableValue(methodNameIndex, methodName);
	methodCallTpl->SetVariableValue(methodParamsIndex, functionParams);
	methodCallTpl->SetVariableValue(callStatusIndex, (int)isInnerCall);	
	methodCallTpl->SetVariableValue(defaultResultStatusIndex, (int)defaultResultNeeded);	
	methodCallTpl->SetVariableValue(callOperatorIndex,  (typeOfCall == CALL_TO_BASE_OBJECT) ? "::" : "->");
	methodCallTpl->SetVariableValue(resultFieldPlaceholderIndex, "");
	methodCallTpl->SetVariableValue(staticPostfixIndex, (typeOfCall == CALL_TO_BASE_OBJECT) ? "_" : "");
	methodCallTpl->SetVariableValue(endIndex, end);

	if (isMainCmd)
	{
		std::string declString = CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString();
		CUnorderedVarDeclarations::GetInstance()->ClearAll();
		return declString + methodCallTpl->GetText();	
	}
	else
	{
		return methodCallTpl->GetText();	
	}
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessMethodCall)
{
	return ProcessMethodCallImpl(nd, context, nd->left ? CALL_OUTSIDE_OBJECT : CALL_INSIDE_OBJECT);
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessPropMethodCall)
{
	return ProcessMethodCallImpl(nd, context, nd->left ? CALL_OUTSIDE_OBJECT : CALL_INSIDE_OBJECT);
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessBaseMethodCall)
{
	return ProcessMethodCallImpl(nd, context, CALL_TO_BASE_OBJECT);
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFunctionAllCallParameters)
{
	GetCurrentFunctionInfo(context.GetCurrentFunctionId()).parametersCount = 0;
	return nd->left ? GetCodeForNode(nd->left, context) : "";
}

bool CFunctionsCodeGenerator::IsMathExpression(const int cmd)const
{
	return (cmd == OP_BRACKETS) || (cmd == OP_PLUS) || (cmd == OP_MINUS) || (cmd == OP_MULTIPLY) || (cmd == OP_DIVISION);
}

std::string CFunctionsCodeGenerator::GetFunctionParameterCode(node* nd, CompileContext& context, const int parameterNum, bool anywayCheck)
{
	bool isOutParamUsed = false;
	if (context.globalFunctionCompiling)
	{
		isOutParamUsed = CGlobalFunctions::GetInstance()->IsOutFunctionParameter(GetCurrentFunctionInfo(context.GetCurrentFunctionId()).name, parameterNum);
	}
	else if (context.standardFunctionCompiling)
	{
		isOutParamUsed = CStandardFunctions::GetInstance()->IsOutParametersUsed(GetCurrentFunctionInfo(context.GetCurrentFunctionId()).name);		
	}
	else
	{
		isOutParamUsed = (!context.standardFunctionCompiling) && ((nd->cmd == -1) || anywayCheck) && ((((m_classOfMethod) && (m_classOfMethod->IsOutMethodParameter(m_methodName, parameterNum))) || (CGlobalSymbolEnumeration::GetInstance()->IsOutMethodParameter(GetCurrentFunctionInfo(context.GetCurrentFunctionId()).name, parameterNum))));
	}
			
	CHANGE_STATE(functionCallOutParameters, isOutParamUsed);
	
	std::string code = GetCodeForNode(nd, context, true);	
	
	if (nd->cmd != OP_FUNCTION_CALL_PARAMS)
	{			
		GetCurrentFunctionInfo(context.GetCurrentFunctionId()).parametersCount++;
		if ((!context.standardFunctionCompiling) || (context.globalFunctionCompiling))
		{		
			CVariablesCodeGenerator* varCodeGen = CVariablesCodeGenerator::GetInstance(NULL);
			bool isNonVar = varCodeGen->IsNonVariable(code.c_str());						
			code = varCodeGen->GetVariableCode(code, (((nd->cmd != -1) && (isNonVar)) || IsMathExpression(nd->cmd)) ? VR_COMPLEX_CONVERT_TO_POINTER : VR_CONVERT_TO_POINTER);
		}
		else if ((context.standardFunctionCompiling) && (CConditionsCodeGenerator::IsCondition(nd->cmd)))
		{	
			CVariablesCodeGenerator* varCodeGen = CVariablesCodeGenerator::GetInstance(NULL);
			code = varCodeGen->GetVariableCode(code, VR_CONSTRUCTION);
		}
	}
	
	RESTORE_STATE(functionCallOutParameters);	
	return code;
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessFunctionCallParameters)
{
	if (nd->center == NULL)
	{
		return GetFunctionParameterCode(nd->left, context, 0);
	}

	USE_TPL(functions, methodCallParams);
	USE_INDEX(methodCallParams, firstPart);
	USE_INDEX(methodCallParams, secondPart);	
	
	std::string firstPart = GetFunctionParameterCode(nd->left, context, 0);	
	methodCallParamsTpl->SetVariableValue(firstPartIndex, firstPart);
		
	std::string continuePart = GetFunctionParameterCode(nd->center, context, GetCurrentFunctionInfo(context.GetCurrentFunctionId()).parametersCount, true);
	methodCallParamsTpl->SetVariableValue(secondPartIndex, continuePart);	

	return methodCallParamsTpl->GetText();
}

std::string CFunctionsCodeGenerator::GetInnerProperty(node* nd, CompileContext& context)
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

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessInnerPropertiesForCalls)
{
	if (nd->center == NULL)
	{				
		return GetInnerProperty(nd->left, context);
	}
		
	USE_TPL(functions, callSequence);
	USE_INDEX(callSequence, object);
	USE_INDEX(callSequence, property);
		
	callSequenceTpl->SetVariableValue(objectIndex, nd->left ? GetInnerProperty(nd->left, context) : "");
	callSequenceTpl->SetVariableValue(propertyIndex, GetInnerProperty(nd->center, context));
		
	return callSequenceTpl->GetText();
}

DECLARE_HANDLER(CFunctionsCodeGenerator::ProcessOutParameter)
{
	m_outParamDeclared = true;	

	// in this line we add symbol to symbol table
	std::string var = GetCodeForNode(nd->left, context);

	if (m_outParamDeclared)
	{
		// and change it's status if needed
		std::vector<CSymbolTable *> children = context.currentSymbolTable->GetChildren();

		std::vector<CSymbolTable *>::iterator it = children.begin();
		std::vector<CSymbolTable *>::iterator itEnd = children.end();

		for ( ; it != itEnd; it++)
		{
			(*it)->MarkSymbolAsOutParameter(nd->left->token);
		}
	}

	return var;
}

std::vector<int> CFunctionsCodeGenerator::GetListOfSupportedTokens()const
{	
	std::vector<int> res;
	res.push_back(FUNC_DECL);
	res.push_back(GLOBAL_FUNC_DECL);
	res.push_back(FUNC_PARAMS);
	res.push_back(VALUE_TRUE);
	res.push_back(VALUE_FALSE);
	res.push_back(OP_RETURN);
	res.push_back(OP_RETURN_PROPS);
	res.push_back(OP_METHOD_CALL);
	res.push_back(OP_PROP_METHOD_CALL);
	res.push_back(OP_BASE_METHOD_CALL);
	res.push_back(OP_FUNCTION_CALL);
	res.push_back(OP_FUNCTION_CALL_PARAMS);
	res.push_back(OP_FUNCTION_ALL_CALL_PARAMS);
	res.push_back(PROP_IN_CALL);
	res.push_back(OUT_PARAM);
	res.push_back(OP_PARAM_ANNOTATION);
	res.push_back(OP_PARAM_ANNOTATION_PROP);
	res.push_back(OP_PARAM_ANNOTATION_PROP_VALUE);
	return res;
}

SCurrentFunction& CFunctionsCodeGenerator::GetCurrentFunctionInfo(unsigned int functionId)
{
	std::map<unsigned int, SCurrentFunction>::iterator it = m_parameters.find(functionId);		
	if (it == m_parameters.end())
	{
		m_parameters.insert(std::map<unsigned int, SCurrentFunction>::value_type(functionId, SCurrentFunction()));
		it = m_parameters.find(functionId);		
	}	

	return (*it).second;
}

std::string CFunctionsCodeGenerator::GetEndFunctionCode()
{	
	USE_TPL(functions, functionEnd);
	USE_INDEX(functionEnd, functionName);

	std::string functionName = "";
	if (Classes::GetInstance()->GetCurrentClass())
	{
		USE_TPL(functions, fullFunctionName);
		USE_INDEX(fullFunctionName, className);
		USE_INDEX(fullFunctionName, methodName);

		fullFunctionNameTpl->SetVariableValue(classNameIndex, Classes::GetInstance()->GetCurrentClass()->GetName());
		fullFunctionNameTpl->SetVariableValue(methodNameIndex, Classes::GetInstance()->GetCurrentMethod());

		functionName = fullFunctionNameTpl->GetText();
	}
	else
	{
		functionName = CGlobalFunctions::GetInstance()->GetCurrentFunction();
	}
			
	functionEndTpl->SetVariableValue(functionNameIndex, functionName);
			
	return functionEndTpl->GetText();
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CFunctionsCodeGenerator)