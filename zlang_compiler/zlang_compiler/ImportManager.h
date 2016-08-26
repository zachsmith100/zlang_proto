
#pragma once

#include <string>
#include <map>
#include "CodeTemplates.h"
#include "CompileTarget.h"

enum PlatformType
{
	OS_ALL = 0,
	OS_LINUX,
	OS_WINDOWS,
	OS_MACOSX,
	OS_UNIX,
	OS_COUNT
};

class CImportManager
{
public:
	void AddInclude(const std::string& id, const PlatformType platformType);
	void AddImport(const std::string& id, const PlatformType platformType);
	void AddConditionalImport(const std::string& debugImport, const std::string& releaseImport, const PlatformType platformType);

	void AddIncludePath(const std::string& path, const PlatformType platformType);
	void AddImportPath(const std::string& path, const PlatformType platformType);

	void AddCompilerFlags(const std::string& id, const PlatformType platformType);

	static CImportManager* GetInstance();

	bool GenerateImportHeaders(const std::string& fileName);
	bool GenerateMakefile(const std::string& fileName);
	bool GenerateMSVCProject(const std::string& fileName);

	void SetCompilerTarget(const PlatformType os);

	void AddCppFile(const std::string& fileName);

	void SetEntryPoint(const std::string& functionName);
	std::string GetEntryPoint()const;
	
private:
	CImportManager();
	CImportManager(const CImportManager& other);
	void operator =(const CImportManager& other);
	~CImportManager();

	std::string GetHeadersText();
	std::string GetMakefileText();
	std::string GetMSVCProjectText();

	ValuesList GetStringsByOS(const std::map<std::string, int>* mp, const PlatformType os);
	void UnquoteValuesList(ValuesList& lst);

	bool GenerateFile(const std::string& fileName, const std::string& content);

	void AddToPlatformMap(std::map<std::string, int>* mp, const std::string& id, const PlatformType platformType);

	ValuesList GetCppFiles(const bool isNeedToUnquote)const;

	std::map<std::string, int> m_headers[OS_COUNT];
	std::map<std::string, int> m_libraries[OS_COUNT];
	std::map<std::string, int> m_compilerFlags[OS_COUNT];
	std::map<std::string, int> m_headerPaths[OS_COUNT];
	std::map<std::string, int> m_libraryPaths[OS_COUNT];
	std::map<std::string, int> m_cppFiles;
	std::map<std::string, int> m_debugLibraries;

	std::string m_entryPoint;

	PlatformType m_compilerTarget;	
};