
#pragma once

#include "BaseCodeGenerator.h"
#include "ClassInfo.h"
#include "Classes.h"
#include "ClassCodeAnalyzer.h"

class CClassCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CClassCodeGenerator)

	DECLARE_HANDLER(ProcessObjectToken);
	DECLARE_HANDLER(ProcessClass);
	DECLARE_HANDLER(ProcessClassName);
	DECLARE_HANDLER(ProcessClassWithType);	
	DECLARE_HANDLER(ProcessSingleton);
	DECLARE_HANDLER(ProcessPrivate);
	DECLARE_HANDLER(ProcessPublic);
	DECLARE_HANDLER(ProcessClassContents);
	DECLARE_HANDLER(ProcessPropertyDeclaration);
	DECLARE_HANDLER(ProcessUsingPropertyInsideClass);
	DECLARE_HANDLER(ProcessUsingPropertyInsideBaseClass);
	DECLARE_HANDLER(ProcessUsingPropertyOutsideClass);
	DECLARE_HANDLER(ProcessInheritancePrivate);
	DECLARE_HANDLER(ProcessInheritancePublic);
	DECLARE_HANDLER(ProcessFunctionResultObjectProperty);
	DECLARE_HANDLER(ProcessConstructor);
	DECLARE_HANDLER(ProcessConstructorParameters);
	DECLARE_HANDLER(ProcessConstructorParameter);

	ClassType GetClassTypeFromString(const std::string& nameType);
	std::string ProcessClassDefinition(const std::string& className, const std::string& classType,
		const std::string& baseClassName, const std::string& baseCppClassName, CompileContext& context, node* nextNode);

private:
	std::string GetFunctionResultObjectPropertyCode(node* functionCall, node* properties, CompileContext& context);
	std::string ProcessUsingPropertyInsideClassImpl(node* nd, CompileContext& context, bool isBaseClass);

public:
	static std::string GetGeneratedCodeForCurrentClass();
	static std::string GetResultObjectName(const std::string& objectName); 

	std::vector<std::string> GetProperties(node* nd, CompileContext& context);	
	std::string GetObjectName(node* nd, CompileContext& context);

	std::string m_className;
	std::string m_classType;
	std::string m_baseClassName;
	std::string m_baseCppClassName;

	CClassCodeAnalyzer m_analyzer;

	std::map<std::string, std::string> m_builtInClassesProtos;
};