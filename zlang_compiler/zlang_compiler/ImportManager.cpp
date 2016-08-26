
#include "ImportManager.h"
#include <assert.h>
#include "ErrorReporter.h"
#include "Utils.h"

CImportManager::CImportManager():
	m_compilerTarget(OS_LINUX)
{
}

CImportManager::CImportManager(const CImportManager& other):
	m_compilerTarget(OS_LINUX)
{
	assert(false);
}

void CImportManager::AddInclude(const std::string& id, const PlatformType platformType)
{
	AddToPlatformMap(m_headers, id, platformType);
}

void CImportManager::AddToPlatformMap(std::map<std::string, int>* mp, const std::string& id, const PlatformType platformType)
{	
	if (OS_UNIX == platformType)
	{
		mp[OS_LINUX].insert(std::map<std::string, int>::value_type(id, 1));
		mp[OS_MACOSX].insert(std::map<std::string, int>::value_type(id, 1));
	}
	else
	{
		mp[platformType].insert(std::map<std::string, int>::value_type(id, 1));
	}
}

void CImportManager::AddImport(const std::string& id, const PlatformType platformType)
{
	assert(platformType != OS_ALL);
	AddToPlatformMap(m_libraries, id, platformType);

	if (platformType == OS_WINDOWS)
	{
		m_debugLibraries.insert(std::map<std::string, int>::value_type(id, 1));
	}
}

void CImportManager::AddConditionalImport(const std::string& debugImport, const std::string& releaseImport, const PlatformType platformType)
{
	assert(platformType == OS_WINDOWS);	
	AddToPlatformMap(m_libraries, releaseImport, platformType);

	m_debugLibraries.insert(std::map<std::string, int>::value_type(debugImport, 1));
}

void CImportManager::AddCompilerFlags(const std::string& id, const PlatformType platformType)
{
	assert(platformType != OS_ALL);
	assert(platformType != OS_WINDOWS);
	AddToPlatformMap(m_compilerFlags, id, platformType);
}

void CImportManager::AddIncludePath(const std::string& path, const PlatformType platformType)
{	
	assert(platformType != OS_ALL);
	AddToPlatformMap(m_headerPaths, path, platformType);
}

void CImportManager::AddImportPath(const std::string& path, const PlatformType platformType)
{
	assert(platformType != OS_ALL);
	AddToPlatformMap(m_libraryPaths, path, platformType);
}

CImportManager* CImportManager::GetInstance()
{
	static CImportManager* obj = NULL;

	if (!obj)
	{
		obj = new CImportManager();
	}

	return obj;
}

void CImportManager::UnquoteValuesList(ValuesList& lst)
{
	ValuesList::iterator it = lst.begin();
	ValuesList::iterator itEnd = lst.end();

	for ( ; it != itEnd; it++)
	{
		*it = UnquoteString(*it);		
	}
}

void CImportManager::SetCompilerTarget(const PlatformType os)
{
	m_compilerTarget = os;
}

ValuesList CImportManager::GetCppFiles(const bool isNeedToUnquote)const
{
	ValuesList list;

	std::map<std::string, int>::const_iterator it = m_cppFiles.begin();
	std::map<std::string, int>::const_iterator itEnd = m_cppFiles.end();

	for ( ; it != itEnd; it++)	
	{
		list.push_back(isNeedToUnquote ? UnquoteString((*it).first) : (*it).first);
	}

	return list;
}

std::string CImportManager::GetMakefileText()
{
	ValuesList librariesList = GetStringsByOS(m_libraries, m_compilerTarget);	
	UnquoteValuesList(librariesList);

	ValuesList libraryPathsList = GetStringsByOS(m_libraryPaths, m_compilerTarget);	
	UnquoteValuesList(libraryPathsList);

	ValuesList headerPathsList = GetStringsByOS(m_headerPaths, m_compilerTarget);	
	UnquoteValuesList(headerPathsList);

	ValuesList compilerFlagsList = GetStringsByOS(m_compilerFlags, m_compilerTarget);	
	UnquoteValuesList(compilerFlagsList);
	
	USE_TPL(project, makefile);	
	USE_INDEX(makefile, libraries);
	USE_INDEX(makefile, libraryPaths);
	USE_INDEX(makefile, headerPaths);
	USE_INDEX(makefile, cppFiles);
	USE_INDEX(makefile, compilerFlags);
	makefileTpl->SetVariableValue(librariesIndex, librariesList);		
	makefileTpl->SetVariableValue(libraryPathsIndex, libraryPathsList);		
	makefileTpl->SetVariableValue(headerPathsIndex, headerPathsList);	
	makefileTpl->SetVariableValue(cppFilesIndex, GetCppFiles(true));
	makefileTpl->SetVariableValue(compilerFlagsIndex, compilerFlagsList);
		
	return makefileTpl->GetText();	
}

std::string CImportManager::GetMSVCProjectText()
{
	ValuesList libraryPathsList = GetStringsByOS(m_libraryPaths, OS_WINDOWS);	
	UnquoteValuesList(libraryPathsList);

	ValuesList headerPathsList = GetStringsByOS(m_headerPaths, OS_WINDOWS);	
	UnquoteValuesList(headerPathsList);

	ValuesList releaseLibsList = GetStringsByOS(m_libraries, OS_WINDOWS);	
	ValuesList debugLibsList;
	std::map<std::string, int>::iterator it = m_debugLibraries.begin();
	std::map<std::string, int>::iterator itEnd = m_debugLibraries.end();
	for ( ; it != itEnd; it++)
	{
		debugLibsList.push_back((*it).first);
	}	

	static CCodeTemplate* projectTpl = CCodeTemplates::GetInstance()->LoadTemplate("code_templates", "project", "project.vcxproj"); 
	assert(projectTpl);	
	USE_INDEX(project, libraryPathsDebug);
	USE_INDEX(project, headerPathsDebug);
	USE_INDEX(project, libraryPathsRelease);
	USE_INDEX(project, headerPathsRelease);
	USE_INDEX(project, cppFiles);
	USE_INDEX(project, projectType);
	USE_INDEX(project, librariesDebug);
	USE_INDEX(project, librariesRelease);
	
	projectTpl->SetVariableValue(librariesDebugIndex, debugLibsList);		
	projectTpl->SetVariableValue(librariesReleaseIndex, releaseLibsList);	
	projectTpl->SetVariableValue(libraryPathsDebugIndex, libraryPathsList);		
	projectTpl->SetVariableValue(headerPathsDebugIndex, headerPathsList);	
	projectTpl->SetVariableValue(libraryPathsReleaseIndex, libraryPathsList);		
	projectTpl->SetVariableValue(headerPathsReleaseIndex, headerPathsList);	
	projectTpl->SetVariableValue(projectTypeIndex, (CImportManager::GetInstance()->GetEntryPoint().size () > 0) ? "Windows" : "Console");	
	projectTpl->SetVariableValue(cppFilesIndex, GetCppFiles(false));	
		
	return projectTpl->GetText();	
}

ValuesList CImportManager::GetStringsByOS(const std::map<std::string, int>* mp, const PlatformType os)
{
	ValuesList lst;

	std::map<std::string, int>::const_iterator it = mp[os].begin();
	std::map<std::string, int>::const_iterator itEnd = mp[os].end();

	for ( ; it != itEnd; it++)	
	{
		lst.push_back((*it).first);
	}	

	return lst;
}

std::string CImportManager::GetHeadersText()
{
	ValuesList headersList[OS_COUNT];	

	for (unsigned int os = OS_ALL; os != OS_COUNT; os++)
	{
		std::map<std::string, int>::iterator it = m_headers[os].begin();
		std::map<std::string, int>::iterator itEnd = m_headers[os].end();

		for ( ; it != itEnd; it++)	
		{
			headersList[os].push_back((*it).first);
		}
	}
	
	ValuesList librariesList = GetStringsByOS(m_libraries, OS_WINDOWS);
	
	USE_TPL(project, headers);
	USE_INDEX(headers, allHeaders);
	USE_INDEX(headers, linuxHeaders);
	USE_INDEX(headers, windowsHeaders);
	USE_INDEX(headers, macosxHeaders);
	USE_INDEX(headers, windowsLibraries);
	headersTpl->SetVariableValue(allHeadersIndex, headersList[OS_ALL]);	
	headersTpl->SetVariableValue(linuxHeadersIndex, headersList[OS_LINUX]);	
	headersTpl->SetVariableValue(windowsHeadersIndex, headersList[OS_WINDOWS]);	
	headersTpl->SetVariableValue(macosxHeadersIndex, headersList[OS_MACOSX]);

	// add also libraries for Windows, because we can import libraries by using preprocesor directives
	headersTpl->SetVariableValue(windowsLibrariesIndex, librariesList);	
	
	return headersTpl->GetText();	
}

bool CImportManager::GenerateFile(const std::string& fileName, const std::string& content)
{
	FILE* fl = OpenFile(fileName, "w");
	if (!fl)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, fileName);
		return false;
	}

	fprintf(fl, "%s", content.c_str());
	fclose(fl);

	return true;
}

bool CImportManager::GenerateImportHeaders(const std::string& fileName)
{
	return GenerateFile(fileName, GetHeadersText());
}

bool CImportManager::GenerateMakefile(const std::string& fileName)
{
	return GenerateFile(fileName, GetMakefileText());
}

bool CImportManager::GenerateMSVCProject(const std::string& fileName)
{
	return GenerateFile(fileName, GetMSVCProjectText());
}

void CImportManager::AddCppFile(const std::string& fileName)
{
	m_cppFiles.insert(std::map<std::string, int>::value_type(fileName, 1));
}

void CImportManager::SetEntryPoint(const std::string& functionName)
{
	m_entryPoint = functionName;
}

std::string CImportManager::GetEntryPoint()const
{
	return m_entryPoint;
}

void CImportManager::operator =(const CImportManager& other)
{
	assert(false);
}

CImportManager::~CImportManager()
{
}
