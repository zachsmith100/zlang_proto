
#include "Common.h"
#include "GlobalSymbolEnumeration.h"
#include "VariablesCodeGenerator.h"
#include "ClassCodeAnalyzer.h"
#include "CodeTemplates.h"
#include "Classes.h"
#include "Utils.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CClassCodeAnalyzer)

PROCESS_TOKENS_START(CClassCodeAnalyzer)
	PROCESS_TOKEN(DECL_CLASS, ProcessClass);
	PROCESS_TOKEN(DECL_CLASS_NAME, ProcessClassName);
	PROCESS_TOKEN(DECL_CLASS_WITH_TYPE, ProcessClassWithType);
	PROCESS_TOKEN(DECL_CLASS_CONTENTS, ProcessClassContents)
	PROCESS_TOKEN(CLASS_TYPE_SINGLETON_DECL, ProcessSingleton);
	PROCESS_TOKEN(SECTION_PRIVATE_DECL, ProcessPrivate);
	PROCESS_TOKEN(SECTION_PUBLIC_DECL, ProcessPublic);	
	PROCESS_TOKEN(CLASS_PROPERTY_DECL, ProcessPropertyDeclaration);
	PROCESS_TOKEN(OP_MY_PROP, ProcessUsingPropertyInsideClass);
	PROCESS_TOKEN(OP_PROP, ProcessUsingPropertyOutsideClass);
	PROCESS_TOKEN(INHERITANCE_PRIVATE, ProcessInheritancePrivate);
	PROCESS_TOKEN(INHERITANCE_PUBLIC, ProcessInheritancePublic);
PROCESS_TOKENS_END

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessInheritancePrivate)
{
	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessInheritancePublic)
{
	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessPropertyDeclaration)
{
	CClassInfo* currentClass = Classes::GetInstance()->GetCurrentClass();

	std::string variable = GetCodeForNode(nd->left, context);

	CHANGE_STATE(varDeclarationExists, true);
	std::string propertyName = GetCodeForNode(nd->left, context);	
	RESTORE_STATE(varDeclarationExists);
	
	if (currentClass)
	{
		currentClass->AddProperty(propertyName, nd->line, "");		
	}

	return "";
}
	
std::string CClassCodeAnalyzer::ProcessClassDefinition(const std::string& className, const std::string& classType, 
	const std::string& baseClassName, const std::string& baseCppClassName, CompileContext& context, node* nextNode)
{
	CClassInfo* newInstance = (classType.size() > 0) ? new CClassInfo(className, nextNode->line, classType) : new CClassInfo(className, nextNode->line, CLASS_TYPE_NORMAL);	
	newInstance = Classes::GetInstance()->AddClass(newInstance);	
	newInstance->SetBaseClass(baseClassName);
	newInstance->SetBaseCppClass(baseCppClassName);
	if (newInstance->GetType() == CLASS_TYPE_SINGLETON)
	{
		CSymbolTable* root = CSymbolTable::GetRoot();
		root->AddSymbol(className);
		root->MarkSymbolAsUsed(className);		
	}
	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessClass)
{
	GetCodeForNode(nd->left, context);

	return ProcessClassDefinition(m_className, m_classType, m_baseClassName, m_baseCppClassName, context, nd->center);	
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessClassWithType)
{
	m_className = nd->center->token;
	m_classType = GetCodeForNode(nd->left, context);
	m_baseClassName = nd->right ? nd->right->token : "";
	m_baseCppClassName = "";

	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessClassName)
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
		m_baseCppClassName = ((nd->center->cmd == INHERITANCE_PRIVATE) ? "private" : "public") + m_baseCppClassName;
	}
	return "";	
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessSingleton)
{	
	// it's needed to register class of correct type (normal, singleton)
	RETURN_FROM_TPL(classes, singletonType); 
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessPrivate)
{
	if (Classes::GetInstance()->GetCurrentClass())
	{
		Classes::GetInstance()->GetCurrentClass()->SetCurrentSectionType(SECTION_TYPE_PRIVATE);	
	}
	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessPublic)
{
	if (Classes::GetInstance()->GetCurrentClass())
	{
		Classes::GetInstance()->GetCurrentClass()->SetCurrentSectionType(SECTION_TYPE_PUBLIC);	
	}
	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessClassContents)
{
	return GetCodeForNode(nd->left, context) + GetCodeForNode(nd->center, context);
}

std::string CClassCodeAnalyzer::GetProperty(node* nd, CompileContext& context)
{
	CHANGE_STATE(functionCallParameters, false);
	CHANGE_STATE(methodCallParameters, false)
	CHANGE_STATE(functionDeclaration, true);
	std::string propertyName = GetCodeForNode(nd, context);
	RESTORE_STATE(functionDeclaration);
	RESTORE_STATE(functionCallParameters);
	RESTORE_STATE(methodCallParameters);

	return propertyName;
}

std::vector<std::string> CClassCodeAnalyzer::GetProperties(node* nd, CompileContext& context)
{
	std::string propertyName = GetProperty(nd, context);
	
	std::vector<std::string> properties;
	if (propertyName.find(".") != std::string::npos)
	{
		properties = SplitStringIntoVector(propertyName, '.');			
	}	
	else
	{
		properties.push_back(propertyName);
	}

	return properties;
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessUsingPropertyInsideClass)
{	
	if (nd->left)
	{
		std::vector<std::string> properties = GetProperties(nd->left, context);
		assert(properties.size() > 0);
	
		for (unsigned int propID = 0; propID < properties.size(); propID++)
		{
			bool isPublic = true;
			if (propID == 0)
			{
				CClassInfo* classInfo = Classes::GetInstance()->GetCurrentClass();
				isPublic = (classInfo) && (classInfo->GetPropertyType(properties[propID]) != PROP_PRIVATE);
			}
			CGlobalSymbolEnumeration::GetInstance()->AddProperty(properties[propID], isPublic);
		}
	}

	return "";
}

DECLARE_HANDLER(CClassCodeAnalyzer::ProcessUsingPropertyOutsideClass)
{
	std::vector<std::string> properties = GetProperties(nd->center, context);
	assert(properties.size() > 0);

	for (unsigned int propID = 0; propID < properties.size(); propID++)
	{
		CGlobalSymbolEnumeration::GetInstance()->AddProperty(properties[propID], true);
	}

	return "";
}

std::vector<int> CClassCodeAnalyzer::GetListOfSupportedTokens()const
{		
	std::vector<int> res;
	res.push_back(DECL_CLASS_WITH_TYPE);
	res.push_back(DECL_CLASS);
	res.push_back(DECL_CLASS_NAME);
	res.push_back(CLASS_TYPE_SINGLETON_DECL);
	res.push_back(SECTION_PRIVATE_DECL);
	res.push_back(SECTION_PUBLIC_DECL);
	res.push_back(DECL_CLASS_CONTENTS);	
	res.push_back(OP_MY_PROP);
	res.push_back(OP_PROP);
	res.push_back(CLASS_PROPERTY_DECL);
	res.push_back(INHERITANCE_PRIVATE);
	res.push_back(INHERITANCE_PUBLIC);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CClassCodeAnalyzer)


