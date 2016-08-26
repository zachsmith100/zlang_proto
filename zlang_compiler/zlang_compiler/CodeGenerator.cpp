
#include "Utils.h"
#include "Common.h"
#include "CodeGenerator.h"
#include "ConditionsCodeGenerator.h"
#include "FunctionsCodeGenerator.h"
#include "VariablesCodeGenerator.h"
#include "CompareCodeGenerator.h"
#include "CyclesCodeGenerator.h"
#include "MathCodeGenerator.h"
#include "ClassCodeGenerator.h"
#include "ClassCodeAnalyzer.h"
#include "FunctionsCodeAnalyzer.h"
#include "CommentCodeGenerator.h"
#include "ComplexStatementsManager.h"
#include "CppInsertCodeGenerator.h"
#include "CodeTemplates.h"
#include "UnorderedVarDeclarations.h"

CCodeGenerator::CCodeGenerator(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree, FILE* const outputFile):
	m_outputFile(outputFile),
	m_mode(ANALYZING)
{
	if (!syntaxTree)
	{
		return;
	}

	m_scopeLevels = &scopeLevels;
	CComplexStatementsManager::GetInstance()->SetCodeGenerator(this);

	assert(outputFile);

	RegisterAnalyzeHandlers();
	Analyze(scopeLevels, syntaxTree);

	Classes::GetInstance()->FinalizeClasses();

	RegisterGenerateHandlers();
	Generate(scopeLevels, syntaxTree);	

	Classes::GetInstance()->CheckValidators();
}

void CCodeGenerator::RegisterAnalyzeHandlers()
{
	CleanUp();
	Register(new CFunctionsCodeAnalyzer(this));	
	Register(CVariablesCodeGenerator::GetInstance(this), false);	
	Register(new CClassCodeAnalyzer(this));
}

void CCodeGenerator::RegisterGenerateHandlers()
{
	CleanUp();
	Register(new CFunctionsCodeGenerator(this));
	Register(new CMathCodeGenerator(this));
	Register(new CCompareCodeGenerator(this));
	Register(new CCyclesCodeGenerator(this));
	Register(CVariablesCodeGenerator::GetInstance(this), false);
	Register(new CConditionsCodeGenerator(this));
	Register(new CClassCodeGenerator(this));
	Register(new CCommentCodeGenerator(this));
	Register(new CCppInsertCodeGenerator(this));
}

CodeGeneratorMode CCodeGenerator::GetMode()const
{
	return m_mode;
}

void CCodeGenerator::Analyze(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree)
{
	m_mode = ANALYZING;

	CErrorReporter::GetInstance()->EnableErrorsHandling(false);

	std::vector<CScopeLevel*>::const_iterator it = scopeLevels.begin();
	std::vector<CScopeLevel*>::const_iterator itEnd = scopeLevels.end();

	for ( ; it != itEnd; it++)
	{		
		m_compileContext.currentSymbolTable = (*it)->GetSymbolTable();		
		m_compileContext.currentSymbolTable->SaveState();
		(*it)->Process(syntaxTree, this);
		m_compileContext.currentSymbolTable->ResetState();
	}

	CErrorReporter::GetInstance()->EnableErrorsHandling(true);
	Classes::GetInstance()->OnEndPass();
}

void CCodeGenerator::Generate(const std::vector<CScopeLevel*>& scopeLevels, node* const syntaxTree)
{
	m_mode = GENERATING;

	std::vector<CScopeLevel*>::const_iterator it = scopeLevels.begin();
	std::vector<CScopeLevel*>::const_iterator itEnd = scopeLevels.end();

	int tabLevel = 0;
	int openBlockCount = 0;

	bool mainFunctionExists = false;

	for ( ; it != itEnd; it++)
	{		
		if (tabLevel > (*it)->GetTabLevel())
		{
			for (int tl = tabLevel; tl > (*it)->GetTabLevel(); tl--)
			{
				bool bracketsNeeded = ((tl > 2) || (Classes::GetInstance()->GetCurrentClass() == NULL));
				if (bracketsNeeded)
				{
					PrintTabs(tl - 1);
				}
				if (openBlockCount == 1)
				{
					Print(CFunctionsCodeGenerator::GetEndFunctionCode().c_str());					
					PrintTabs(tl - 1);
				}
				if (bracketsNeeded)
				{
					Print("}\n");
					openBlockCount--;
				}
			}
		}
		if (tabLevel < (*it)->GetTabLevel())
		{				
			if (((*it)->GetTabLevel() > 2) || (Classes::GetInstance()->GetCurrentClass() == NULL))
			{
				PrintTabs((*it)->GetTabLevel() - 1);
				Print("{\n");
				if ( ( ((*it)->GetTabLevel() == 3) && (Classes::GetInstance()->GetCurrentClass())) ||
					((*it)->GetTabLevel() == 1))
				{
					PrintTabs((*it)->GetTabLevel());
					Print(CFunctionsCodeGenerator::GetStartFunctionCode().c_str());
					(*it)->GetSymbolTable()->AddSymbol("result");
					(*it)->GetSymbolTable()->MarkSymbolAsUsed("result");
				}
				openBlockCount++;
			}
		}
		m_compileContext.currentSymbolTable = (*it)->GetSymbolTable();
		(*it)->Process(syntaxTree, this);
		tabLevel = (*it)->GetTabLevel();		
	}

	for (int i = 0; i < openBlockCount; i++)
	{
		int offset = 1;
		if (!Classes::GetInstance()->GetCurrentClass())
		{
			offset = -1;
		}
		
		PrintTabs(openBlockCount - i + offset);
		if (i == openBlockCount - 1)
		{	
			Print(CFunctionsCodeGenerator::GetEndFunctionCode().c_str());			
			PrintTabs(openBlockCount - i + offset);
		}
		Print("}\n");
	}

	if (!Classes::GetInstance()->IsClassExists("EntryPoint"))
	{
		CErrorReporter::GetInstance()->OnFatalError(NO_ENTRY_POINT, 0xFFFFFFFF, "");
	}
	else
	{
		CClassInfo* entryPoint = Classes::GetInstance()->GetClassByName("EntryPoint");
		if (!entryPoint->IsMethodExist("run"))
		{
			CErrorReporter::GetInstance()->OnFatalError(INVALID_ENTRY_POINT, 0xFFFFFFFF, "");
		}
	}

	CComplexStatementsManager::GetInstance()->OnEndProgram();

	CErrorReporter::GetInstance()->PrintCompileStatus();
}

void CCodeGenerator::PurePrintTabs(const int tabsCount)
{
	if (m_mode == ANALYZING)
	{
		return;
	}

	int correctedTabsCount = Classes::GetInstance()->GetCurrentClass() ? tabsCount + SCOPE_OFFSET : tabsCount;

	if (m_outputFile)
	for (int i = 0; i < correctedTabsCount; i++)
	{
		fprintf(m_outputFile, "\t");
	}
}

void CCodeGenerator::PrintTabs(const int tabsCount)
{
	if (m_mode == ANALYZING)
	{
		return;
	}

	CComplexStatementsManager::GetInstance()->OnNewTabLevel(tabsCount); 

	int correctedTabsCount = Classes::GetInstance()->GetCurrentClass() ? tabsCount + SCOPE_OFFSET : tabsCount;

	if (m_outputFile)
	for (int i = 0; i < correctedTabsCount; i++)
	{
		fprintf(m_outputFile, "\t");
	}
}

void CCodeGenerator::PurePrint(const char* const str)
{
	if (m_mode == ANALYZING)
	{
		return;
	}

	if (m_outputFile) 
	{		
		fprintf(m_outputFile, "%s", str);
	}
}

void CCodeGenerator::Print(const char* const str)
{
	if (m_mode == ANALYZING)
	{
		return;
	}	
	
	if (m_outputFile) 
	{		
		if (strstr(str, "\n") != NULL)
		{
			CComplexStatementsManager::GetInstance()->OnBeforeNewLine();
		}		

		fprintf(m_outputFile, "%s", str);

		if (strstr(str, "\n") != NULL)
		{
			CComplexStatementsManager::GetInstance()->OnAfterNewLine();
		}		
	}
}

void CCodeGenerator::Print(const char* const str, const char* const param)
{
	if (m_mode == ANALYZING)
	{
		return;
	}	

	if (m_outputFile) 
	{
		if ((strstr(str, "\n") != NULL) || (strstr(param, "\n") != NULL))
		{
			CComplexStatementsManager::GetInstance()->OnBeforeNewLine();
		}

		if (!CUnorderedVarDeclarations::GetInstance()->IsEmpty())
		{
			fprintf(m_outputFile, "%s", CUnorderedVarDeclarations::GetInstance()->GetDeclarationsString().c_str());
			CUnorderedVarDeclarations::GetInstance()->ClearAll();
		}

		fprintf(m_outputFile, str, param);		

		if ((strstr(str, "\n") != NULL) || (strstr(param, "\n") != NULL))
		{
			CComplexStatementsManager::GetInstance()->OnAfterNewLine();
		}
	}
}

std::string CCodeGenerator::GetCodeForNode(node* const nd, CompileContext& context)
{
	assert(nd);
	if (!nd)
	{
		return "";
	}

	nd->processed = m_mode + 1;
	if (context.highLevelCounter > 0)
	{
		context.highLevelCounter--;
	}

	std::map<int, ICodeGenerator *>::iterator it = m_tokenHandlers.find(nd->cmd);
	if (it != m_tokenHandlers.end())
	{
		return (*it).second->GenerateCodeForNode(nd, context);
	}
	
	if (m_mode == GENERATING)
	{
		assert((nd->cmd == OP_NEWLINE) || (nd->cmd == TAB3));		
	}
	return "";
}

void CCodeGenerator::ProcessNode(node* nd)
{
	if (nd)
	if ((nd->cmd > 0) && (nd->processed == m_mode) && (nd->cmd != OP_NEWLINE) && (nd->cmd != TAB3))
	{
		m_compileContext.highLevelCounter = 2;		
		std::string text = GetCodeForNode(nd, m_compileContext);
		if (text.size() > 0)
		{
			PrintTabs(nd->tabLevel);	
			Print("%s\n", (char *)text.c_str());		
		}		
	}
}

void CCodeGenerator::Register(ICodeGenerator* const codeGen, const bool needToDelete)
{
	if (needToDelete)
	{
		m_codeGenerators.push_back(codeGen);
	}

	std::vector<int> tokens = codeGen->GetListOfSupportedTokens();
	std::vector<int>::iterator it = tokens.begin();
	std::vector<int>::iterator itEnd = tokens.end();
	for ( ; it != itEnd; it++)
	{
		m_tokenHandlers.insert(std::map<int, ICodeGenerator*>::value_type(*it, codeGen));
	}
}

void CCodeGenerator::CleanUp()
{
	std::vector<ICodeGenerator *>::iterator it = m_codeGenerators.begin();
	std::vector<ICodeGenerator *>::iterator itEnd = m_codeGenerators.end();

	for ( ; it != itEnd; it++)	
	{
		delete (*it);
	}

	m_tokenHandlers.clear();
	m_codeGenerators.clear();
}

std::string CCodeGenerator::GetEndLineCode(const unsigned int line)
{
	unsigned int lineInFile = line;
	CErrorReporter::GetInstance()->GetErrorLineInfo(lineInFile, NULL);

	USE_TPL(functions, endLine);
	USE_INDEX(endLine, line);

	endLineTpl->SetVariableValue(lineIndex, IntToStr(lineInFile));
	
	return endLineTpl->GetText();			
}

CCodeGenerator::~CCodeGenerator()
{
	CleanUp();
}
