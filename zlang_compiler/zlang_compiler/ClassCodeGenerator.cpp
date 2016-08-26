
#include "Common.h"
#include "GlobalSymbolEnumeration.h"
#include "VariablesCodeGenerator.h"
#include "ClassCodeGenerator.h"
#include "UniqueIdGenerator.h"
#include "CodeTemplates.h"
#include "Classes.h"
#include "Utils.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(CClassCodeGenerator),
	m_analyzer(codeGenerator)
{	
}

PROCESS_TOKENS_START(CClassCodeGenerator)
	PROCESS_TOKEN(DECL_CLASS, ProcessClass);
	PROCESS_TOKEN(DECL_CLASS_NAME, ProcessClassName);
	PROCESS_TOKEN(DECL_CLASS_WITH_TYPE, ProcessClassWithType);
	PROCESS_TOKEN(DECL_CLASS_CONTENTS, ProcessClassContents)
	PROCESS_TOKEN(CLASS_TYPE_SINGLETON_DECL, ProcessSingleton);
	PROCESS_TOKEN(SECTION_PRIVATE_DECL, ProcessPrivate);
	PROCESS_TOKEN(SECTION_PUBLIC_DECL, ProcessPublic);
	PROCESS_TOKEN(CLASS_PROPERTY_DECL, ProcessPropertyDeclaration);
	PROCESS_TOKEN(OP_MY_PROP, ProcessUsingPropertyInsideClass);
	PROCESS_TOKEN(OP_BASE_PROP, ProcessUsingPropertyInsideBaseClass);
	PROCESS_TOKEN(OP_PROP, ProcessUsingPropertyOutsideClass);
	PROCESS_TOKEN(INHERITANCE_PRIVATE, ProcessInheritancePrivate);
	PROCESS_TOKEN(INHERITANCE_PUBLIC, ProcessInheritancePublic);
	PROCESS_TOKEN(OP_FUNC_PROP, ProcessFunctionResultObjectProperty);	
	PROCESS_TOKEN(FUNC_RESULT_OBJECT, ProcessObjectToken);
	PROCESS_TOKEN(OP_CONSTRUCTOR, ProcessConstructor);
	PROCESS_TOKEN(OP_CONSTRUCTOR_PARAMS, ProcessConstructorParameters);
	PROCESS_TOKEN(OP_CONSTRUCTOR_PARAMETER, ProcessConstructorParameter);
PROCESS_TOKENS_END

DECLARE_HANDLER(CClassCodeGenerator::ProcessConstructor)
{
	context.newClassName = nd->left->token;
	context.newClassInstanceName = CUniqueIdGenerator::GetInstance()->GetId();
	CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(context.newClassName);
	if (!classInfo)
	{
		CErrorReporter::GetInstance()->OnFatalError(UNKNOWN_CLASS, nd->line, context.newClassName);
	}
	else if (classInfo->GetType() == CLASS_TYPE_SINGLETON)
	{
		CErrorReporter::GetInstance()->OnFatalError(CREATE_SINGLETON_INSTANCE, nd->line, "", context.newClassName);
	}
	
	return GetCodeForNode(nd->center, context);
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessConstructorParameters)
{
	std::string code = GetCodeForNode(nd->left, context);
	if (nd->center)
	{
		code += GetCodeForNode(nd->center, context);
	}
	return code;
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessConstructorParameter)
{
	assert(context.newClassName.size() > 0);
	assert(context.newClassInstanceName.size() > 0);

	USE_TPL(classes, constructorItem);	
	USE_INDEX(constructorItem, prop);
	USE_INDEX(constructorItem, value);	
	USE_INDEX(constructorItem, objectName);	
		
	constructorItemTpl->SetVariableValue(propIndex, nd->left->token);

	CGlobalSymbolEnumeration::GetInstance()->AddProperty(nd->left->token, true);

	if (nd->center->token)
	{
		if (Classes::GetInstance()->IsBuiltInPublicClassExists(nd->center->token))
		{
			USE_TPL(classes, standardObjectAssigment);
			USE_INDEX(standardObjectAssigment, name);
			standardObjectAssigmentTpl->SetVariableValue(nameIndex, nd->center->token);		

			constructorItemTpl->SetVariableValue(valueIndex, standardObjectAssigmentTpl->GetText());		
		}
		else
		{
			constructorItemTpl->SetVariableValue(valueIndex, nd->center->token);
		}
	}
	else
	{
		constructorItemTpl->SetVariableValue(valueIndex, GetCodeForNode(nd->center, context));		
	}
	constructorItemTpl->SetVariableValue(objectNameIndex, context.newClassInstanceName);		

	return constructorItemTpl->GetText();
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessObjectToken)
{
	return "";
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessInheritancePrivate)
{
	return "";
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessInheritancePublic)
{
	return "";
}

std::string CClassCodeGenerator::GetGeneratedCodeForCurrentClass()
{
	USE_TPL(classes, classGeneratedCode);	
	USE_INDEX(classGeneratedCode, initCode);
	USE_INDEX(classGeneratedCode, methodsCallCode);	
	USE_INDEX(classGeneratedCode, methodsCallByNameCode);	
		
	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();
	if (!currentClass)
	{
		return "";
	}
		
	classGeneratedCodeTpl->SetVariableValue(initCodeIndex, currentClass->GetInitializationCode());
	classGeneratedCodeTpl->SetVariableValue(methodsCallCodeIndex, currentClass->GetMethodCallSupportCode());		
	classGeneratedCodeTpl->SetVariableValue(methodsCallByNameCodeIndex, currentClass->GetMethodCallByNameSupportCode());		

	return classGeneratedCodeTpl->GetText();
}

std::string CClassCodeGenerator::GetResultObjectName(const std::string& objectName)
{
	if (objectName == MY_ID)
	{
		return objectName;
	}

	CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(objectName);

	CCodeTemplate* objectNameTpl = NULL;

	if ((!classInfo) || (classInfo->GetType() != CLASS_TYPE_SINGLETON))
	{
		INIT_TPL(objectName, classes, objectName);
	}
	else
	{
		INIT_TPL(objectName, classes, objectNameSingleton);
	}

	USE_INDEX(objectName, name);
	objectNameTpl->SetVariableValue(nameIndex, objectName);

	return objectNameTpl->GetText();
}
	
std::string CClassCodeGenerator::ProcessClassDefinition(const std::string& className, const std::string& classType, 
	const std::string& baseClassName, const std::string& baseCppClassName, CompileContext& context, node* nextNode)
{
	std::string begin;

	if (Classes::GetInstance()->GetCompiledClassCount() > 0)
	{		
		begin = GetGeneratedCodeForCurrentClass();
	}

	CClassInfo* newInstance = (classType.size() > 0) ? new CClassInfo(className, nextNode->line, classType) : new CClassInfo(className, nextNode->line, CLASS_TYPE_NORMAL);

	if (Classes::GetInstance()->CheckSecondClassDeclaration(className, nextNode->line))
	{
		CErrorReporter::GetInstance()->OnFatalError(SECOND_CLASS_DECLARATION, nextNode->line, className, baseClassName);
	}

	newInstance = Classes::GetInstance()->AddClass(newInstance);		

	newInstance->SetBaseCppClass(baseCppClassName);

	if (m_codeGenerator->GetMode() == GENERATING)
	{
		newInstance->SetCompiledStatus(true);
	}

	if (baseClassName.size() > 0)
	{
		if (!newInstance->SetBaseClass(baseClassName))
		{
			CErrorReporter::GetInstance()->OnFatalError(BASE_CLASS_UNDEFINED, nextNode->line, className, baseClassName);
		}
	}
	
	return begin;
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessClass)
{
	GetCodeForNode(nd->left, context);

	return ProcessClassDefinition(m_className, m_classType, m_baseClassName, m_baseCppClassName, context, nd->center);	
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessClassWithType)
{
	m_className = nd->center->token;
	m_classType = GetCodeForNode(nd->left, context);
	m_baseClassName = nd->right ? nd->right->token : "";
	m_baseCppClassName = "";

	return "";
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessClassName)
{
	m_className = nd->left->token;
	m_classType = "";
	if (!nd->right)
	{
		m_baseClassName = nd->center ? nd->center->token : "";
		m_baseCppClassName = "";
	}
	else
	{
		m_baseClassName = "";
		m_baseCppClassName = nd->right ? nd->right->token : "";
		m_baseCppClassName = " " + m_baseCppClassName;

		CCodeTemplate* classNameTpl = NULL;
		if (nd->center->cmd == INHERITANCE_PRIVATE)
		{
			INIT_TPL(className, classes, privateCppClassName);
		}
		else
		{
			INIT_TPL(className, classes, publicCppClassName);
		}

		USE_INDEX(className, name);
		classNameTpl->SetVariableValue(nameIndex, m_baseCppClassName);

		m_baseCppClassName = classNameTpl->GetText();
	}
	return "";	
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessSingleton)
{	
	return m_analyzer.ProcessSingleton(nd, context);	
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessPrivate)
{
	m_analyzer.ProcessPrivate(nd, context);	
	return "";
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessPublic)
{
	m_analyzer.ProcessPublic(nd, context);	
	return "";
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessClassContents)
{
	return m_analyzer.ProcessClassContents(nd, context);
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessPropertyDeclaration)
{
	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();

	std::string variable = GetCodeForNode(nd->left, context);

	CHANGE_STATE(varDeclarationExists, true);
	std::string propertyName = GetCodeForNode(nd->left, context);

	bool isRightValClass = (nd->center->cmd == -1) && (Classes::GetInstance()->IsClassExists(nd->center->token));

	CCodeTemplate* propertyInitializationTpl = NULL;

	if (isRightValClass)
	{
		INIT_TPL(propertyInitialization, classes, propertyInitializationAsClass);
	}
	else
	{
		INIT_TPL(propertyInitialization, classes, propertyInitialization);
	}

	USE_INDEX(propertyInitialization, name);
	USE_INDEX(propertyInitialization, value);
	USE_INDEX(propertyInitialization, callStatus);

	propertyInitializationTpl->SetVariableValue(nameIndex, propertyName);
	propertyInitializationTpl->SetVariableValue(callStatusIndex, currentClass->GetCurrentSectionType() == SECTION_TYPE_PUBLIC ? 1 : 2);
	
	if (isRightValClass)
	{	
		CHANGE_STATE(classInstanceCreationExists, true);		
		std::string className = GetCodeForNode(nd->center, context, true);
		RESTORE_STATE(classInstanceCreationExists);		
				
		propertyInitializationTpl->SetVariableValue(valueIndex, className);		
	}
	else
	{		
		propertyInitializationTpl->SetVariableValue(valueIndex, GetCodeForNode(nd->center, context));		
	}
	RESTORE_STATE(varDeclarationExists);
	
	if (currentClass)
	{
		if (currentClass->CheckSecondPropertyDeclaration(propertyName, nd->line))
		{
			CErrorReporter::GetInstance()->OnFatalError(MULTIPLE_PROPERTIES_WITH_ONE_NAME, nd->line, propertyName);
		}
		else
		{
			currentClass->AddProperty(propertyName, nd->line, propertyInitializationTpl->GetText());
		}
	}

	return "";
}

std::string CClassCodeGenerator::ProcessUsingPropertyInsideClassImpl(node* nd, CompileContext& context, bool isBaseClass)
{
	DEFINE_END_SIGN(nd->line);

	if (!nd->left)
	{
		USE_TPL(classes, returnThis);	
		USE_INDEX(returnThis, end);
		returnThisTpl->SetVariableValue(endIndex, end);
		return returnThisTpl->GetText();		
	}

	USE_TPL(classes, propertyInsideClass);	
	USE_INDEX(propertyInsideClass, propertyName);
	USE_INDEX(propertyInsideClass, objectName);
	USE_INDEX(propertyInsideClass, sign);
	USE_INDEX(propertyInsideClass, end);
	
	std::vector<std::string> properties = m_analyzer.GetProperties(nd->left, context);
	assert(properties.size() > 0);
	
	std::string code = "";

	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();
	if ((!currentClass) && (isBaseClass))
	{
		CErrorReporter::GetInstance()->OnFatalError(BASE_CLASS_CANNOT_BE_USED, nd->line);	
		return "";
	}

	for (unsigned int propID = 0; propID < properties.size(); propID++)
	{
		propertyInsideClassTpl->SetVariableValue(propertyNameIndex, properties[propID]);

		std::string objectNameValue = "";
		if (propID == 0)
		{
			CCodeTemplate* pointerTpl;
			if (isBaseClass)
			{
				INIT_TPL(pointer, classes, basePointer);
				USE_INDEX(pointer, baseClassName);
				pointerTpl->SetVariableValue(baseClassNameIndex, currentClass->GetBaseProtoClassName());
			}
			else
			{	
				INIT_TPL(pointer, classes, thisPointer);
			}
			objectNameValue = pointerTpl->GetText();
		}

		propertyInsideClassTpl->SetVariableValue(objectNameIndex, objectNameValue);		
		propertyInsideClassTpl->SetVariableValue(endIndex, (propID != properties.size() - 1) ? "" : end);

		bool isPublic = true;
		if ((propID == 0) && (currentClass))
		{
			isPublic = (currentClass->GetPropertyType(properties[propID]) != PROP_PRIVATE);
		}

		if (propID != 0)
		if (!CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(properties[propID]))
		{
			if ((propID != properties.size() - 1) || (context.newVariablesProhibited))
			{
				CErrorReporter::GetInstance()->OnFatalError(UNINITIALIZED_VARIABLE, nd->line, properties[propID]);				
			}
		}

		if (propID == properties.size() - 1)
		{
			CGlobalSymbolEnumeration::GetInstance()->AddProperty(properties[propID], isPublic);

			if ((isBaseClass) && (currentClass) && (currentClass->GetBaseClass()))
			{
				if (!currentClass->GetBaseClass()->IsPropertyExist(properties[propID]))
				{
					currentClass->GetBaseClass()->AddProperty(properties[propID], nd->line);
				}
			}
		}
		
		code += propertyInsideClassTpl->GetText();			
	}

	if ((properties.size() > 0) && (currentClass))
	{
		if ((currentClass->IsMethodExist(properties[0])) && (!context.functionPointerCompiling))
		{
			CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_USE_METHOD_AS_PROPERTY, nd->line, properties[0]);
		}
		else if (!currentClass->IsLocalPropertyExist(properties[0]))
		{
			currentClass->AddProperty(properties[0], nd->line);			
		}
	}

	return code;
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessUsingPropertyInsideClass)
{
	return ProcessUsingPropertyInsideClassImpl(nd, context, false);
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessUsingPropertyInsideBaseClass)
{
	return ProcessUsingPropertyInsideClassImpl(nd, context, true);
}

std::string CClassCodeGenerator::GetObjectName(node* nd, CompileContext& context)
{
	CHANGE_STATE(functionCallParameters, false);
	CHANGE_STATE(methodCallParameters, false);
	CHANGE_STATE(usingObject, true);
	std::string objectName = GetCodeForNode(nd, context);	
	RESTORE_STATE(usingObject);
	RESTORE_STATE(functionCallParameters);
	RESTORE_STATE(methodCallParameters);
	return objectName;
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessUsingPropertyOutsideClass)
{
	DEFINE_END_SIGN(nd->line);	

	USE_TPL(classes, propertyOutsideClass);
	USE_INDEX(propertyOutsideClass, objectName);
	USE_INDEX(propertyOutsideClass, propertyName);
	USE_INDEX(propertyOutsideClass, end);

	std::string objectName = GetObjectName(nd->left, context);	
		
	std::vector<std::string> properties = m_analyzer.GetProperties(nd->center, context);
	assert(properties.size() > 0);

	std::string code = "";

	CClassInfo* classInfo = Classes::GetInstance()->GetClassByName(objectName);
	if ((classInfo) && (classInfo->GetType() == CLASS_TYPE_SINGLETON))
	{
		if (classInfo->GetPropertyType(properties[0]) == PROP_PRIVATE)
		{
			CErrorReporter::GetInstance()->OnFatalError(ATTEMPT_TO_USE_PRIVATE_PROP_SINGLETON, nd->line, properties[0], objectName);
		}
	}

	for (unsigned int propID = 0; propID < properties.size(); propID++)
	{
		propertyOutsideClassTpl->SetVariableValue(objectNameIndex, (propID == 0) ? GetResultObjectName(objectName) : "");
		propertyOutsideClassTpl->SetVariableValue(propertyNameIndex, properties[propID]);
		propertyOutsideClassTpl->SetVariableValue(endIndex, (propID != properties.size() - 1) ? "" : end);
	
		if (propID == properties.size() - 1)
		{
			CGlobalSymbolEnumeration::GetInstance()->AddProperty(properties[propID], true);
		}

		if (!CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(properties[propID]))
		{
			CErrorReporter::GetInstance()->OnFatalError(UNINITIALIZED_VARIABLE, nd->line, properties[propID]);				
		}
	
		code += propertyOutsideClassTpl->GetText();		
	}

	return code;
}

std::string CClassCodeGenerator::GetFunctionResultObjectPropertyCode(node* functionCall, node* propertiesNode, CompileContext& context)
{	
	assert(functionCall);
	assert(propertiesNode);

	std::string code = GetCodeForNode(functionCall, context);
			
	std::vector<std::string> properties = m_analyzer.GetProperties(propertiesNode, context);
	
	USE_TPL(classes, propertyInsideClass);	
	USE_INDEX(propertyInsideClass, propertyName);
	USE_INDEX(propertyInsideClass, objectName);	
	USE_INDEX(propertyInsideClass, end);
		
	for (unsigned int propID = 0; propID < properties.size(); propID++)
	{
		propertyInsideClassTpl->SetVariableValue(propertyNameIndex, properties[propID]);
		propertyInsideClassTpl->SetVariableValue(objectNameIndex, "");
		propertyInsideClassTpl->SetVariableValue(endIndex, "");		

		if (!CGlobalSymbolEnumeration::GetInstance()->IsPropertyExists(properties[propID]))
		{
			if ((propID != properties.size() - 1) || (context.newVariablesProhibited))
			{
				CErrorReporter::GetInstance()->OnFatalError(UNINITIALIZED_VARIABLE, propertiesNode->line, properties[propID]);				
			}
		}			
		
		code += propertyInsideClassTpl->GetText();			
	}

	return code;
}

DECLARE_HANDLER(CClassCodeGenerator::ProcessFunctionResultObjectProperty)
{
	DEFINE_END_SIGN(nd->line);	
	std::string code = GetFunctionResultObjectPropertyCode(nd->left, nd->center, context);
	return code + end;
}

std::vector<int> CClassCodeGenerator::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(DECL_CLASS_WITH_TYPE);
	res.push_back(DECL_CLASS);
	res.push_back(DECL_CLASS_NAME);
	res.push_back(CLASS_TYPE_SINGLETON_DECL);
	res.push_back(SECTION_PRIVATE_DECL);
	res.push_back(SECTION_PUBLIC_DECL);
	res.push_back(DECL_CLASS_CONTENTS);
	res.push_back(CLASS_PROPERTY_DECL);
	res.push_back(OP_MY_PROP);
	res.push_back(OP_PROP);
	res.push_back(INHERITANCE_PRIVATE);
	res.push_back(INHERITANCE_PUBLIC);
	res.push_back(OP_FUNC_PROP);	
	res.push_back(FUNC_RESULT_OBJECT);	
	res.push_back(OP_CONSTRUCTOR);	
	res.push_back(OP_CONSTRUCTOR_PARAMS);	
	res.push_back(OP_CONSTRUCTOR_PARAMETER);
	res.push_back(OP_BASE_PROP);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CClassCodeGenerator)


