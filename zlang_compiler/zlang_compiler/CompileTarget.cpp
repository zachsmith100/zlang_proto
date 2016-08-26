
#include "Common.h"
#include <stdlib.h>
#include <stdio.h>
#include "CompileTarget.h"
#include "Utils.h"
#include "ErrorReporter.h"
#include "ImportManager.h"
#include <assert.h>

CCompileTarget::CCompileTarget():	
	m_defaultIntegerType(VT_INTEGER),
	m_defaultDoubleType(VT_DOUBLE),
	m_isMethodsListNeeded(false),
	m_defaultPrecision(128)	
{	
}
	
CCompileTarget::CCompileTarget(const CCompileTarget& other)
{
	assert(false);
}

CCompileTarget* CCompileTarget::GetInstance()
{
	static CCompileTarget* obj = NULL;

	if (!obj)
	{
		obj = new CCompileTarget();
	}

	return obj;
}
	
void CCompileTarget::SetCommandLine(int argc, char* argv[])
{
	if (argc == 3)
	{
		// user is not choosed some target
		// we need set target by default
		m_target = TARGET_GCC;
	}
	else if (argc >= 4)
	{
		ParseTargetString(argv[3]);
	}
	
	if (argc > 4)
	{
		for (int i = 4; i < argc; i++)
		{
			ParseOptionalParameter(argv[i]);
		}
	}

	CImportManager::GetInstance()->SetCompilerTarget((m_target == TARGET_CLANG) ? OS_MACOSX : OS_LINUX);
}

void CCompileTarget::ParseOptionalParameter(const std::string& optionalParam)
{
	if (optionalParam == "--nofilenames")
	{
		CErrorReporter::GetInstance()->DisableShowFileNames();
	}
	else if (optionalParam == "--projecttree")
	{
		m_isMethodsListNeeded = true;		
	}
	else if (optionalParam.find("--save:") == 0)
	{
		CProjectCompiler::GetInstance()->SetSaveMode(optionalParam.substr(7, optionalParam.size() - 7));
	}
	else if (optionalParam.find("--load:") == 0)
	{
		CProjectCompiler::GetInstance()->LoadFromMode(optionalParam.substr(7, optionalParam.size() - 7));
	}
	else if (optionalParam.find("--default_integer:") == 0)
	{
		SetDefaultIntegerType(optionalParam.substr(18, optionalParam.size() - 18));
	}
	else if (optionalParam.find("--default_double:") == 0)
	{
		SetDefaultDoubleType(optionalParam.substr(17, optionalParam.size() - 17));
	}
	else if (optionalParam.find("--default_precision:") == 0)
	{
		SetDefaultPrecision(atoi(optionalParam.substr(20, optionalParam.size() - 20).c_str()));
	}
	else 
	{
		printf("Optional parameter %s cannot be parsed!", optionalParam.c_str());
	}
}

bool CCompileTarget::IsMethodsListNeeded()const
{
	return m_isMethodsListNeeded;
}

void CCompileTarget::ParseTargetString(const std::string& targetStr)
{
	if (targetStr == "gcc")
	{
		m_target = TARGET_GCC;
	}
	else if (targetStr == "msvc")
	{
		m_target = TARGET_VISUAL_STUDIO;
	}
	else if (targetStr == "clang")
	{
		m_target = TARGET_CLANG;
	}
	else if (targetStr == "all")
	{
		m_target = TARGET_ALL;
	}
	else 
	{
		m_target = TARGET_GCC;
	}
}

void CCompileTarget::AddTargetTemplatesFilenames(std::vector<std::string>& files, const CompileTarget target)
{
	if ((TARGET_CLANG == target) || (TARGET_GCC == target) || (TARGET_ALL == target))
	{
		files.push_back("makefile");
	}
	if ((TARGET_VISUAL_STUDIO == target) || (TARGET_ALL == target))
	{
		files.push_back("project.sln");
		files.push_back("project.vcxproj");
	}	
}

CompileTarget CCompileTarget::GetTarget()const
{
	return m_target;
}

bool CCompileTarget::GenerateFilesForCurrentTarget(const std::string& templatesPath, const std::string& outputDir)
{
	if ((TARGET_GCC == m_target) || (TARGET_CLANG == m_target))
	{
		CImportManager::GetInstance()->GenerateMakefile(outputDir + "makefile");
		return true;
	}

	std::vector<std::string> filesToCopy;
	AddTargetTemplatesFilenames(filesToCopy, m_target);

	std::vector<std::string>::iterator it = filesToCopy.begin();
	std::vector<std::string>::iterator itEnd = filesToCopy.end();

	for ( ; it != itEnd; it++)
	if (!FileCopy(templatesPath + (*it) + ".tpl", outputDir + *it))
	{
		return false;
	}	

	if (TARGET_VISUAL_STUDIO == m_target)
	{
		CImportManager::GetInstance()->GenerateMSVCProject(outputDir + "project.vcxproj");
		return true;
	}

	return true;
}

bool CCompileTarget::Compile(const std::string& outputDir)
{
	bool executeError = false;
	if ((TARGET_GCC == m_target) || (TARGET_CLANG == m_target) || (TARGET_ALL == m_target))
	{
		if (!ExecuteExternalCommand("cd \"" + outputDir + "\"", " >/dev/null 2>/dev/null && make clean >/dev/null 2>/dev/null || true && make all >/dev/null 2>/dev/null  && chmod u+x project >/dev/null 2>/dev/null"))
		{
			executeError = true;
		}
	}
	if ((TARGET_VISUAL_STUDIO == m_target) || (TARGET_ALL == m_target))
	{
		std::string msBuildPath = GetCppCompilerPath();		
		if (msBuildPath.size() == 0)
		{
			CErrorReporter::GetInstance()->OnFatalError(MSBUILD_NOT_FOUND, 0xFFFFFFFF);
		}
		if (!ExecuteExternalCommand(msBuildPath + "msbuild.exe", outputDir + "project.vcxproj /p:Configuration=Release"))
		{
			executeError = true;
		}
	}	
	if (executeError)
	{
		CErrorReporter::GetInstance()->OnFatalError(EXTERNAL_CPP_COMPILER_EXECUTE_ERROR, 0xFFFFFFFF);
	}
	return true;
}

VariableType CCompileTarget::GetVarTypeByString(const std::string& typeName)
{
	if (typeName == "integer")
	{
		return VT_INTEGER;
	}
	if (typeName == "arbitrary_integer")
	{
		return VT_ARBITRARY_INTEGER;
	}
	if (typeName == "double")
	{
		return VT_DOUBLE;
	}
	if (typeName == "arbitrary_float")
	{
		return VT_ARBITRARY_FLOAT_POINT;
	}
	if (typeName == "arbitrary_fixed")
	{
		return VT_ARBITRARY_FIXED_POINT;
	}	

	return VT_UNRECOGNIZED;
}

void CCompileTarget::SetDefaultIntegerType(const std::string& defIntType)
{
	m_defaultIntegerType = GetVarTypeByString(defIntType);
	if (VT_UNRECOGNIZED == m_defaultIntegerType)
	{
		m_defaultIntegerType = VT_INTEGER;
	}
}

VariableType CCompileTarget::GetDefaultIntegerType()const
{
	return m_defaultIntegerType;
}

void CCompileTarget::SetDefaultDoubleType(const std::string& defDblType)
{
	m_defaultDoubleType = GetVarTypeByString(defDblType);
	if (VT_UNRECOGNIZED == m_defaultDoubleType)
	{
		m_defaultDoubleType = VT_DOUBLE;
	}
}

VariableType CCompileTarget::GetDefaultDoubleType()const
{
	return m_defaultDoubleType;
}

void CCompileTarget::SetDefaultPrecision(const unsigned int precision)
{
	m_defaultPrecision = precision;
}

unsigned int CCompileTarget::GetDefaultPrecision()const
{
	return m_defaultPrecision;
}

ZTokenType CCompileTarget::NumericTypeToTokenType(const VariableType& varType)
{
	switch (varType)
	{
	case VT_INTEGER:
		return TT_INTEGER;
		break;

	case VT_ARBITRARY_INTEGER:
		assert(false);
		return TT_INTEGER;
		break;

	case VT_DOUBLE:
		return TT_FLOAT;
		break;	

	case VT_ARBITRARY_FLOAT_POINT:
		return TT_ARB_FLOAT;
		break;

	case VT_ARBITRARY_FIXED_POINT:
		assert(false);
		return TT_FLOAT;
		break;

	default:
		assert(false);
	}
	
	return TT_INTEGER;
}

ZTokenType CCompileTarget::RefineTokenType(const ZTokenType& tokenType)
{
	if ((tokenType == TT_INTEGER) || (tokenType == TT_HEX_INTEGER))
	{
		return NumericTypeToTokenType(m_defaultIntegerType);
	}

	if (tokenType == TT_FLOAT)
	{
		return NumericTypeToTokenType(m_defaultDoubleType);;
	}

	return tokenType;
}

void CCompileTarget::operator =(const CCompileTarget& other)
{
	assert(false);
}

CCompileTarget::~CCompileTarget()
{
}
