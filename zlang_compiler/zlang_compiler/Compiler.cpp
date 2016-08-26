// Project
//////////
#include "Common.h"
#include "node.h"
#include "SyntaxTreeProcessor.h"

// Flex & Bison staff
//////////
#include "yacc.tab.h"
extern node* getroot();
extern void setParseFile(FILE* fl);
extern int isParsingSuccessed();
extern char* getLexErrorText();
extern int getLexErrorLine();
extern char* getYaccErrorText();
extern int getYaccErrorLine();

// Project
//////////
#include "Compiler.h"
#include "Utils.h"
#include "ClassCodeGenerator.h"
#include "Classes.h"
#include "GlobalSymbolEnumeration.h"
#include "ErrorReporter.h"

CCompiler::CCompiler(int argc, char* argv[]):
	m_projectGenerator(argc, argv)
{
	printf("Start compiler \n");

	m_inputPath = argv[1];
	m_outputPath = argv[2];
}

bool CCompiler::Compile()
{
	CProjectCompiler::GetInstance()->SetFolderPath(m_inputPath);
	if (!CProjectCompiler::GetInstance()->Process())
	{
		return false;
	}
	
	if (!RunLexerAndParser())
	{
		return false;
	}

	if (!GenerateProject())
	{
		return false;
	}

	if (!GenerateCode())
	{
		return false;
	}

	if (!m_projectGenerator.PostProcessProject())
	{		
		return false;
	}
		
	if (!CompileGeneratedProject())
	{
		return false;
	}

	return true;
}

bool CCompiler::RunLexerAndParser()
{
	FILE* inputFile = CProjectCompiler::GetInstance()->GetSingleSourceFile();	
	assert(inputFile);
	if (!inputFile)
	{
		return false;
	}

	setParseFile(inputFile);	
	yyparse();
	fclose(inputFile);

	if (getYaccErrorText() != NULL)
	{
		std::string errorTxt = CErrorReporter::GetInstance()->PatchYaccError(getYaccErrorText());
		CErrorReporter::GetInstance()->OnFatalError(YACC_ERROR, getYaccErrorLine(), errorTxt);
		return false;
	}

	if (isParsingSuccessed() == 0)
	{
		CErrorReporter::GetInstance()->OnFatalError(LEX_ERROR, getLexErrorLine(), getLexErrorText());		
		return false;
	}

	if (getroot() == NULL)
	{
		CErrorReporter::GetInstance()->OnFatalError(SYNTAX_TREE_BUILD_ERROR, 0xFFFFFFFF);					
		return false;
	}

	return true;
}

bool CCompiler::GenerateCode()
{
	FILE* outputFile = OpenFile(m_projectGenerator.GetMainFilePath(), "a");
	if (!outputFile)
	{
		CErrorReporter::GetInstance()->OnFatalError(OUTPUT_FILE_ERROR, 0xFFFFFFFF);		
		return false;
	}

	CSyntaxTreeProcessor syntaxTreeProcessor(getroot(), outputFile);
	syntaxTreeProcessor.Process();

	if (Classes::GetInstance()->GetClassCount() > 0)
	{
		std::string code = CClassCodeGenerator::GetGeneratedCodeForCurrentClass();
		fprintf(outputFile, "%s", code.c_str());		
	}
	
	fclose(outputFile);	
	
	return true;
}

bool CCompiler::GenerateProject()
{
	m_projectGenerator.SetInputPath((char*)m_inputPath.c_str());
	m_projectGenerator.SetOutputPath((char*)m_outputPath.c_str());
	switch (m_projectGenerator.GetErrorCode())
	{
	case CREATE_DIRECTORY_ERROR:
		CErrorReporter::GetInstance()->OnFatalError(OUTPUT_DIR_NOT_EXISTS, 0xFFFFFFFF);		
		return false;

	case NO_TEMPLATES_ERROR:
		CErrorReporter::GetInstance()->OnFatalError(NO_TEMPLATES, 0xFFFFFFFF);		
		return false;
            
    case SUCCESS:
    case FILES_COPY_ERROR:
        // shut up stupid clang
        break;
	}

	if (m_projectGenerator.Generate() != SUCCESS)
	{
		CErrorReporter::GetInstance()->OnFatalError(OUTPUT_DIR_NOT_WRITEABLE, 0xFFFFFFFF);				
		return false;
	}	

	return true;
}

bool CCompiler::CompileGeneratedProject()
{
	if (CErrorReporter::GetInstance()->IsAnyFatalErrorExists())
	{
		return false; 
	}

	return m_projectGenerator.Compile();
}

CCompiler::~CCompiler()
{	
	printf("End compiler \n");	
}
 