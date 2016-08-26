
#pragma once

#include "ClassInfo.h"

#include <map>
#include <string>

typedef std::map<std::string, CClassInfo*> ClassesMap;

#define CLASS_ON_CREATED_CALLBACK	"onCreated"

class Classes
{
public:
	static Classes* GetInstance();

	void ClearActiveClass();
	void OnEndPass();

	CClassInfo* AddClass(CClassInfo* classInfo);
	bool IsClassExists(const std::string& name)const;
	bool CheckSecondClassDeclaration(const std::string& name, const unsigned int line)const;
	
	unsigned int GetClassCount()const;
	unsigned int GetCompiledClassCount()const;

	CClassInfo* GetCurrentClass()const;
	CClassInfo* GetClassByName(const std::string& name)const;
	
	bool SaveInfo(const std::string& fileName)const;

	bool CreateClassDeclarations(const std::string& classDeclPath);
	bool CreateGlobalFunctionsFile(const std::string& filePath);

	void FinalizeClasses();
	void CheckValidators();

	bool IsBuiltInPublicClassExists(const std::string& name);

	void SetCurrentMethod(const std::string& name, const unsigned int line);
	std::string GetCurrentMethod()const;
	unsigned int GetCurrentMethodDeclarationLine()const;

	std::vector<std::string> GetClassesList()const;

private:
	Classes();
	Classes(const Classes& other);
	void operator =(const Classes& other);
	~Classes();

	std::string GetClassesDeclarationCode()const;
	std::string GetClassDeclarationCode(const CClassInfo* const classInfo)const;
	std::string GetClassesRedirectCode()const;

	ClassesMap m_classesMap;
	CClassInfo* m_currentClassInfo;

	std::string m_currentMethod;
	unsigned int m_currentMethodDeclarationLine;
};