
#pragma once	

#include "BaseCodeGenerator.h"
#include <map>

enum TypeOfMethodCall
{
	CALL_OUTSIDE_OBJECT,
	CALL_INSIDE_OBJECT,
	CALL_TO_BASE_OBJECT
};

struct SCurrentFunction
{
	int parametersCount;
	std::string name;

	SCurrentFunction()
	{
		parametersCount = 0;
	}

	SCurrentFunction(const SCurrentFunction& other)
	{
		parametersCount = other.parametersCount;
		name = other.name;
	}
};

class CFunctionsCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CFunctionsCodeGenerator)

	DECLARE_HANDLER(ProcessFunctionDeclaration);
	DECLARE_HANDLER(ProcessGlobalFunctionDeclaration);
	DECLARE_HANDLER(ProcessFunctionParameters);
	DECLARE_HANDLER(ProcessTRUE);
	DECLARE_HANDLER(ProcessFALSE);
	DECLARE_HANDLER(ProcessReturn);
	DECLARE_HANDLER(ProcessReturnProperties);
	DECLARE_HANDLER(ProcessMethodCall);
	DECLARE_HANDLER(ProcessFunctionCall);
	DECLARE_HANDLER(ProcessFunctionCallParameters);
	DECLARE_HANDLER(ProcessFunctionAllCallParameters);
	DECLARE_HANDLER(ProcessPropMethodCall);
	DECLARE_HANDLER(ProcessBaseMethodCall);
	DECLARE_HANDLER(ProcessOutParameter);
	DECLARE_HANDLER(ProcessInnerPropertiesForCalls);
	DECLARE_HANDLER(ProcessParamAnnotation);
	DECLARE_HANDLER(ProcessParamAnnotationProperty);
	DECLARE_HANDLER(ProcessParamAnnotationPropertyValue);

public:
	static std::string GetStartFunctionCode();
	static std::string GetEndFunctionCode();

private:
	SCurrentFunction& GetCurrentFunctionInfo(const unsigned int functionCounterID);
	std::string GetResultPropertyInitCode(node* nd, CompileContext& context);

	std::string GetMethodName(node* nd, CompileContext& context);
	void FixObjectNameIfNeeded(node* nd, CompileContext& context, std::string& objectName, CClassInfo*& classInfo);
	std::string GetObjectName(node* nd, CompileContext& context, const TypeOfMethodCall typeOfCall);
	void HandlePossibleErrors(node* nd, std::string& methodName, CClassInfo* classInfo, MethodCallValidateError error);
	std::string GetInnerProperty(node* nd, CompileContext& context);
	
	std::string GetFunctionName(node* nd, CompileContext& context);
	std::string ProcessMethodCallImpl(node* nd, CompileContext& context, const TypeOfMethodCall typeOfCall);	

	std::string GetFunctionParameterCode(node* nd, CompileContext& context, const int parameterNum, bool anywayCheck = false);

	bool IsMathExpression(const int cmd)const;
	std::string GetReturnValue(node* nd, CompileContext& context);
	
	unsigned int m_parametersCount;

	std::vector<bool> m_outParamStatus;
	bool m_outParamDeclared;
	CClassInfo* m_classOfMethod;
	std::string m_methodName;
	bool m_isStandardFunctionCompiling;

	std::map<unsigned int, SCurrentFunction> m_parameters;	
	unsigned int m_maxUsedFunctionId;

	std::vector<MethodParameter> m_parametersDescription;
	std::vector<std::string> m_annotationPropertyValues;
};