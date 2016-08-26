
#pragma once

#include "BaseCodeGenerator.h"
#include "ClassInfo.h"
#include "Classes.h"

class CClassCodeAnalyzer : public CBaseCodeGenerator
{
friend class CClassCodeGenerator;

	DECLARE_CLASS_DEFINITION(CClassCodeAnalyzer)

	DECLARE_HANDLER(ProcessClass);
	DECLARE_HANDLER(ProcessClassName);
	DECLARE_HANDLER(ProcessClassWithType);	
	DECLARE_HANDLER(ProcessSingleton);
	DECLARE_HANDLER(ProcessPrivate);
	DECLARE_HANDLER(ProcessPublic);
	DECLARE_HANDLER(ProcessClassContents);	
	DECLARE_HANDLER(ProcessUsingPropertyInsideClass);
	DECLARE_HANDLER(ProcessUsingPropertyOutsideClass);
	DECLARE_HANDLER(ProcessPropertyDeclaration);
	DECLARE_HANDLER(ProcessInheritancePrivate);
	DECLARE_HANDLER(ProcessInheritancePublic);
	
	std::string ProcessClassDefinition(const std::string& className, const std::string& classType,
		const std::string& baseClassName, const std::string& baseCppClassName, CompileContext& context, node* nextNode);

public:	
	std::vector<std::string> GetProperties(node* nd, CompileContext& context);
	std::string GetProperty(node* nd, CompileContext& context);

	std::string m_className;
	std::string m_classType;
	std::string m_baseClassName;
	std::string m_baseCppClassName;
};