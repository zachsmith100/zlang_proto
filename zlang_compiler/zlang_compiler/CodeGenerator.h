
#pragma once

#include <string>
#include "node.h"
#include "ClassInfo.h"
#include "ScopeLevel.h"
#include "INodeHandler.h"
#include "ICodeGenerator.h"
#include "ErrorReporter.h"

enum CodeGeneratorMode
{
	ANALYZING,
	GENERATING
};

#define SCOPE_OFFSET (-2)

class CCodeGenerator : public INodeHandler
{
public:
	CCodeGenerator(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree, FILE* const outputFile);
	~CCodeGenerator();	

	void ProcessNode(node* nd);

	std::string GetCodeForNode(node* const nd, CompileContext& context);

	CodeGeneratorMode GetMode()const;

	void CleanUp();

	void PurePrint(const char* const str);
	void PurePrintTabs(const int tabsCount);

	static std::string GetEndLineCode(const unsigned int line);
	
private:
	void Print(const char* const str);
	void Print(const char* const str, const char* const param);
	void PrintTabs(const int tabsCount);

	void Generate(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree);
	void Analyze(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree);

	void Register(ICodeGenerator* const codeGen, const bool needToDelete = true);	

	void RegisterGenerateHandlers();
	void RegisterAnalyzeHandlers();

	CSymbolTable* m_currentSymbolTable;

	FILE* m_outputFile;

	CompileContext m_compileContext;	

	std::vector<ICodeGenerator *> m_codeGenerators;
	std::map<int, ICodeGenerator*> m_tokenHandlers;

	const std::vector<CScopeLevel*>* m_scopeLevels;
	
	CClassInfo* m_currentClassInfo;	

	CodeGeneratorMode m_mode;
};