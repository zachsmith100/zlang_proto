
#include "Common.h"
#include "ProjectGenerator.h"
#include "Utils.h"
#include "GlobalSymbolEnumeration.h"
#include "ErrorReporter.h"
#include "Classes.h"
#include "CompilerPath.h"
#include "ImportManager.h"

CProjectGenerator::CProjectGenerator(int argc, char* argv[]):	
	m_projectTemplatesPath(CCompilerPath::Get()),	
	m_errorCode(SUCCESS),
	m_outputDir(""),
	m_inputDir(""),
	m_rootPath("")
{
	CCompileTarget::GetInstance()->SetCommandLine(argc, argv);

	m_projectTemplatesPath.Add("code_templates");
	m_projectTemplatesPath.Add("project");	

	m_projectFiles.push_back("main.cpp");
	m_projectFiles.push_back("UniVar.cpp");
	m_projectFiles.push_back("UniVar.h");
	m_projectFiles.push_back("StandardFuncs.cpp");
	m_projectFiles.push_back("StandardFuncs.h");	
	m_projectFiles.push_back("ClassSupport.cpp");
	m_projectFiles.push_back("ClassSupport.h");
	m_projectFiles.push_back("ArraySupport.cpp");
	m_projectFiles.push_back("ArraySupport.h");
	m_projectFiles.push_back("ZMath.cpp");
	m_projectFiles.push_back("ZMath.h");
	m_projectFiles.push_back("ZString.cpp");
	m_projectFiles.push_back("ZString.h");	
	m_projectFiles.push_back("HashtableSupport.cpp");
	m_projectFiles.push_back("HashtableSupport.h");
	m_projectFiles.push_back("StreamSupport.cpp");
	m_projectFiles.push_back("StreamSupport.h");
	m_projectFiles.push_back("Log.cpp");
	m_projectFiles.push_back("Log.h");
	m_projectFiles.push_back("LogManager.cpp");
	m_projectFiles.push_back("LogManager.h");
	m_projectFiles.push_back("jsoncpp.cpp");
	m_projectFiles.push_back("json.h");
	m_projectFiles.push_back("json-forwards.h");
	m_projectFiles.push_back("tinyxml2.cpp");
	m_projectFiles.push_back("tinyxml2.h");
	m_projectFiles.push_back("minIni.cpp");
	m_projectFiles.push_back("minIni.h");
	m_projectFiles.push_back("Utils.cpp");
	m_projectFiles.push_back("Utils.h");
	m_projectFiles.push_back("GlobalMethods.h");
	m_projectFiles.push_back("ArbitraryPrecisionFloat.cpp");
	m_projectFiles.push_back("ArbitraryPrecisionFloat.h");	
	m_projectFiles.push_back("gmp.h");
	m_projectFiles.push_back("gmpxx.h");
	m_projectFiles.push_back("mpfr.h");
	m_projectFiles.push_back("mpfr.lib");
	m_projectFiles.push_back("gmp-mparam.h");
	m_projectFiles.push_back("config.h");
	m_projectFiles.push_back("mpfr.dll");
	m_projectFiles.push_back("mpir.dll");
	CCompileTarget::GetInstance()->AddTargetTemplatesFilenames(m_projectFiles);

	CheckProjectFilesTemplates();
}

std::string CProjectGenerator::GetProjectTemplatesPath()const
{
	return m_projectTemplatesPath;
}

void CProjectGenerator::SetInputPath(const CFolderPath& inputDirectory)
{
	m_inputDir = inputDirectory;
}

void CProjectGenerator::SetOutputPath(const CFolderPath& outputDirectory)
{
	m_outputDir = outputDirectory;

	if (!ForceDirectory(m_outputDir))
	{
		m_errorCode = CREATE_DIRECTORY_ERROR;
	}
}

void CProjectGenerator::CheckProjectFilesTemplates()
{
	std::vector<std::string>::iterator it = m_projectFiles.begin();
	std::vector<std::string>::iterator itEnd = m_projectFiles.end();

	for ( ; it != itEnd; it++)
	if (!IsFileExists(GetProjectTemplatesPath() + (*it) + ".tpl"))
	{
		CFilePath path(m_projectTemplatesPath);
		path.Add((*it) + ".tpl");
		printf("Not found: %s\n", path.ToString());
		m_errorCode = NO_TEMPLATES_ERROR;
	}
}

ProjectGeneratorError CProjectGenerator::GetErrorCode()const
{
	return m_errorCode;
}

ProjectGeneratorError CProjectGenerator::Generate()
{	
	std::vector<std::string>::iterator it = m_projectFiles.begin();
	std::vector<std::string>::iterator itEnd = m_projectFiles.end();

	for ( ; it != itEnd; it++)
	if (!FileCopy(GetProjectTemplatesPath() + (*it) + ".tpl", GetFilePath(*it)))
	{
		return FILES_COPY_ERROR;
	}	

	return SUCCESS;
}

std::string CProjectGenerator::GetFilePath(const std::string& nameFile)const
{
	CFilePath path(m_outputDir);
	path.Add(nameFile);
	return path;
}

std::string CProjectGenerator::GetMainFilePath()const
{
	return GetFilePath("main.cpp");
}

void CProjectGenerator::CopyFolder(const std::string& folderName)
{
	m_rootPath = m_inputDir;
	m_rootPath.Add(folderName);	
	FindFiles(m_rootPath);
}

bool CProjectGenerator::CopyRoot()
{	
	m_rootCopySuccess = true;

	CopyFolder(CProjectCompiler::GetInstance()->GetRootFolder());
	CopyFolder(CProjectCompiler::GetInstance()->GetPlatformRootFolder());

	return m_rootCopySuccess;
}

bool CProjectGenerator::HandleFile(const std::string& path, const std::string& name)
{
	CFilePath inputPath = path + name;
	std::string relativeOutputPath = path;
	StringReplace(relativeOutputPath, m_rootPath, "");
	std::string outputDirPath = m_outputDir;
	outputDirPath += relativeOutputPath;
	std::string outputFilePath = outputDirPath;	
	outputFilePath += name;

	if (relativeOutputPath.size() != 0)
	if (!ForceDirectories(m_outputDir, relativeOutputPath))
	{
		m_rootCopySuccess = false;
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, outputDirPath);
	}

	if (!FileCopy(inputPath, outputFilePath))
	{
		m_rootCopySuccess = false;
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, outputFilePath);
	}

	return true;
}

std::string CProjectGenerator::GetEntryPointCreationCode()const
{
	CClassInfo* entryPoint = Classes::GetInstance()->GetClassByName("EntryPoint");
	if ((entryPoint != NULL) && (entryPoint->GetType() == CLASS_TYPE_SINGLETON))
	{
		RETURN_FROM_TPL(project, mainEndSingletonEntryPoint);
	}
	else
	{
		RETURN_FROM_TPL(project, mainEndNormalEntryPoint);		
	}
}

bool CProjectGenerator::PostProcessProject()
{
	if (!CopyRoot())
	{
		return false;
	}

	std::string symbolsEnumsPath = GetFilePath("SymbolEnums.h");
	if (!CGlobalSymbolEnumeration::GetInstance()->WriteCodeToFile(symbolsEnumsPath))
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, symbolsEnumsPath);
		return false;
	}

	if (!CCompileTarget::GetInstance()->GenerateFilesForCurrentTarget(GetProjectTemplatesPath(), m_outputDir))
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, GetProjectTemplatesPath());
		return false;
	}

	FILE* output = OpenFile(GetMainFilePath(), "a");
	if (!output)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, GetMainFilePath());
		return false;
	}

	USE_TPL(project, mainEnd);
	USE_INDEX(mainEnd, entryPointCreatingCode);
	USE_INDEX(mainEnd, entryPointName);
	mainEndTpl->SetVariableValue(entryPointCreatingCodeIndex, GetEntryPointCreationCode());
	mainEndTpl->SetVariableValue(entryPointNameIndex, (CImportManager::GetInstance()->GetEntryPoint().size() > 0) ? UnquoteString(CImportManager::GetInstance()->GetEntryPoint()) : "main");

	fputs(mainEndTpl->GetText().c_str(), output);		

	fclose(output);

	if (!Classes::GetInstance()->CreateClassDeclarations(GetFilePath("ClassDeclarations.h")))
	{
		return false;
	}

	if (!Classes::GetInstance()->CreateGlobalFunctionsFile(GetFilePath("GlobalMethods.cpp")))
	{
		return false;
	}

	if (!CImportManager::GetInstance()->GenerateImportHeaders(GetFilePath("ImportHeaders.h")))
	{
		return false;
	}

	if (CCompileTarget::GetInstance()->IsMethodsListNeeded())
	{
		if (!Classes::GetInstance()->SaveInfo(GetFilePath("ProjectTree.json")))
		{			
			return false;
		}		
	}

	if (!GenerateCommonConfig())
	{
		return false;
	}

	return true;
}

bool CProjectGenerator::Compile()
{
	return CCompileTarget::GetInstance()->Compile(m_outputDir);
}

std::string CProjectGenerator::NumericTypeToString(const VariableType& varType)
{
	if (varType == VT_INTEGER)
	{
		return "UV_INT";
	}
	else if (varType == VT_DOUBLE)
	{
		return "UV_DOUBLE";
	}
	else if (varType == VT_ARBITRARY_FLOAT_POINT)
	{
		return "UV_ARB_DOUBLE";
	}

	assert(false);
	return "";
}

bool CProjectGenerator::GenerateCommonConfig()
{
	std::string path = GetFilePath("CommonConfig.h");

	FILE* output = OpenFile(path, "w");
	if (!output)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, path);
		return false;
	}

	USE_TPL(project, commonConfig);	
	USE_INDEX(commonConfig, defaultPrecision);
	USE_INDEX(commonConfig, defaultIntegerType);	
	USE_INDEX(commonConfig, defaultDoubleType);	

	commonConfigTpl->SetVariableValue(defaultPrecisionIndex, CCompileTarget::GetInstance()->GetDefaultPrecision());
	commonConfigTpl->SetVariableValue(defaultIntegerTypeIndex, NumericTypeToString(CCompileTarget::GetInstance()->GetDefaultIntegerType()));	
	commonConfigTpl->SetVariableValue(defaultDoubleTypeIndex, NumericTypeToString(CCompileTarget::GetInstance()->GetDefaultDoubleType()));	
	
	fprintf(output, "%s\n", commonConfigTpl->GetText().c_str());
	fclose(output);

	return true;
}

CProjectGenerator::~CProjectGenerator()
{
}
