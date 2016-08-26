
#include "Common.h"
#include "Classes.h"
#include <assert.h>
#include "CodeTemplates.h"
#include "ErrorReporter.h"
#include "Utils.h"
#include "GlobalFunctions.h"

Classes::Classes():
	m_currentClassInfo(NULL),
	m_currentMethodDeclarationLine(0)
{
	std::vector<MethodParameter> parameters;
	std::vector<bool> outStatus;

	CClassInfo* workaroundsObjectClass = new CClassInfo("WorkaroundsObject", 0, CLASS_TYPE_NORMAL);
	workaroundsObjectClass->AddProperty("commandLineParams", 0, "");
	workaroundsObjectClass->AddProperty("processPath", 0, "");
	workaroundsObjectClass->AddProperty("processName", 0, "");
	workaroundsObjectClass->AddProperty("processFolder", 0, "");
	workaroundsObjectClass->AddProperty("result", 0, "");
	workaroundsObjectClass->AddProperty("success", 0, "");
	workaroundsObjectClass->AddProperty("properties", 0, "");
	workaroundsObjectClass->AddMethod(CLASS_ON_CREATED_CALLBACK, 0, 0, outStatus, parameters, false, "");	
	workaroundsObjectClass->SetBuiltInStatus(true);
	workaroundsObjectClass->SetPublicStatus(false);
	outStatus.push_back(false);
	workaroundsObjectClass->AddMethod("convertable", 1, 0, outStatus, parameters, false, "");
	outStatus.clear();
	AddClass(workaroundsObjectClass);

	CClassInfo* emptyObjectClass = new CClassInfo("Object", 0, CLASS_TYPE_NORMAL);	
	emptyObjectClass->SetBuiltInStatus(true);
	emptyObjectClass->SetPublicStatus(true);
	AddClass(emptyObjectClass);

	CClassInfo* arrayClass = new CClassInfo("Array", 0, CLASS_TYPE_NORMAL);
	arrayClass->SetBuiltInStatus(true);
	arrayClass->SetPublicStatus(true);
	arrayClass->AddMethod("removeAll", 0, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("clone", 0, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	arrayClass->AddMethod("resize", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("remove", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("push", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("indexOf", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("indexFirstOf", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("indexLastOf", 1, 0, outStatus, parameters, false, "");	
	arrayClass->AddMethod("implode", 1, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("binarizeString", 1, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	arrayClass->AddMethod("cut", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("remove", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("paste", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddProperty("length", 0, "");
	arrayClass->AddMethod("extractNumeric", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("extractString", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("extractBoolean", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("extractFloat", 2, 0, outStatus, parameters, false, "");	
	arrayClass->AddMethod("binarizeNumeric", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("binarizeBoolean", 2, 0, outStatus, parameters, false, "");
	arrayClass->AddMethod("binarizeFloat", 2, 0, outStatus, parameters, false, "");	
	AddClass(arrayClass);

	CClassInfo* hashtableClass = new CClassInfo("Hashtable", 0, CLASS_TYPE_NORMAL);
	hashtableClass->SetBuiltInStatus(true);
	hashtableClass->SetPublicStatus(true);
	outStatus.clear();
	hashtableClass->AddMethod("clear", 0, 0, outStatus, parameters, false, "");
	hashtableClass->AddMethod("clone", 0, 0, outStatus, parameters, false, "");
	hashtableClass->AddMethod("getValues", 0, 0, outStatus, parameters, false, "");
	hashtableClass->AddMethod("getKeys", 0, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	hashtableClass->AddMethod("removeKey", 1, 0, outStatus, parameters, false, "");
	hashtableClass->AddMethod("keyExists", 1, 0, outStatus, parameters, false, "");
	hashtableClass->AddProperty("length", 0, "");
	// placeholders for hashtable element object properties
	hashtableClass->AddProperty("key", 0, "");
	hashtableClass->AddProperty("value", 0, "");
	AddClass(hashtableClass);

	CClassInfo* stringClass = new CClassInfo("String", 0, CLASS_TYPE_SINGLETON);
	stringClass->SetBuiltInStatus(true);
	stringClass->SetPublicStatus(false);
	outStatus.clear();
	outStatus.push_back(false);
	stringClass->AddMethod("Trim", 1, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("LeftTrim", 1, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RightTrim", 1, 0, outStatus, parameters, false, "");	
	outStatus.push_back(false);
	stringClass->AddMethod("Explode", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("Find", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("FindFirstOf", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("FindLastOf", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("Erase", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RegexMatch", 2, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RegexSearch", 2, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	stringClass->AddMethod("Replace", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("Insert", 3, 0, outStatus, parameters, false, "");	
	stringClass->AddMethod("SubString", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("Find", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("FindFirstOf", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("FindLastOf", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("Erase", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RegexMatch", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RegexSearch", 3, 0, outStatus, parameters, false, "");
	stringClass->AddMethod("RegexReplace", 3, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	stringClass->AddMethod("RegexReplace", 4, 0, outStatus, parameters, false, "");
	AddClass(stringClass);

	CClassInfo* streamClass = new CClassInfo("Stream", 0, CLASS_TYPE_NORMAL);
	streamClass->SetBuiltInStatus(true);
	streamClass->SetPublicStatus(false);
	streamClass->AddProperty("writeable", 0, "");
	streamClass->AddProperty("opened", 0, "");
	streamClass->AddProperty("path", 0, "");
	streamClass->AddProperty("type", 0, "");
	streamClass->AddProperty("size", 0, "");
	streamClass->AddProperty("position", 0, "");
	AddClass(streamClass);

	CClassInfo* mathClass = new CClassInfo("Number", 0, CLASS_TYPE_SINGLETON);
	mathClass->SetBuiltInStatus(true);
	outStatus.clear();
	mathClass->AddMethod("PI", 0, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	mathClass->AddMethod("Abs", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Sin", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Cos", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Tan", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Ctg", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Log", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Log10", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Exp", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Sqrt", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Asin", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Acos", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Atan", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Actg", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Round", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Truncate", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Ceil", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Floor", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("Fraction", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("GetPrecision", 1, 0, outStatus, parameters, false, "");
	mathClass->AddMethod("PI", 1, 0, outStatus, parameters, false, "");
	outStatus.push_back(false);
	stringClass->AddMethod("Pow", 2, 0, outStatus, parameters, false, "");
	AddClass(mathClass);
}
	
Classes::Classes(const Classes& other):
	m_currentClassInfo(NULL),
	m_currentMethodDeclarationLine(0)
{
	assert(false);
}

Classes* Classes::GetInstance()
{
	static Classes* obj = NULL;

	if (!obj)
	{
		obj = new Classes();
	}

	return obj;
}

unsigned int Classes::GetClassCount()const
{
	return m_classesMap.size();
}

bool Classes::IsClassExists(const std::string& name)const
{
	ClassesMap::const_iterator it = m_classesMap.find(name);
	if (it == m_classesMap.end())
	{
		return false;
	}

	return true;
}

bool Classes::IsBuiltInPublicClassExists(const std::string& name)
{
	ClassesMap::const_iterator it = m_classesMap.find(name);
	if (it == m_classesMap.end())
	{
		return false;
	}

	CClassInfo* classInfo = (*it).second;
	return (classInfo->IsBuiltIn()) && (classInfo->IsPublic());
}

bool Classes::CheckSecondClassDeclaration(const std::string& name, const unsigned int line)const
{
	ClassesMap::const_iterator it = m_classesMap.find(name);
	if (it == m_classesMap.end())
	{
		return false;
	}

	return ((*it).second->GetDeclarationLine() != line);
}

void Classes::operator =(const Classes& other)
{
	assert(false);
}

CClassInfo* Classes::AddClass(CClassInfo* classInfo)
{
	if (!CheckSecondClassDeclaration(classInfo->GetName(), classInfo->GetDeclarationLine()))
	{				
		ClassesMap::const_iterator it = m_classesMap.find(classInfo->GetName());
		if (it != m_classesMap.end())
		{
			delete classInfo;
			classInfo = (*it).second;
		}
		else
		{
			classInfo->SetID(m_classesMap.size());
			m_classesMap.insert(ClassesMap::value_type(classInfo->GetName(), classInfo));
		}
		m_currentClassInfo = classInfo;		
	}

	return classInfo;
}

void Classes::ClearActiveClass()
{
	m_currentClassInfo = NULL;
}

CClassInfo* Classes::GetCurrentClass()const
{
	return m_currentClassInfo;
}

CClassInfo* Classes::GetClassByName(const std::string& name)const
{
	if (MY_ID == name)
	{
		return m_currentClassInfo;
	}
	else if (BASE_ID == name)
	{
		return m_currentClassInfo->GetBaseClass();
	}

	ClassesMap::const_iterator it = m_classesMap.find(name);
	if (it == m_classesMap.end())
	{
		return NULL;
	}

	return (*it).second;
}

bool Classes::SaveInfo(const std::string& fileName)const
{
	Json::Value classes;
	classes.resize(GetCompiledClassCount() + 1);

	classes[0] = CGlobalFunctions::GetInstance()->GetInfoAsJSONObject();

	ClassesMap::const_iterator it = m_classesMap.begin();
	ClassesMap::const_iterator itEnd = m_classesMap.end();

	for (int i = 0; it != itEnd; it++)
	if (!(*it).second->IsBuiltIn())
	{
		Json::Value classInfo = (*it).second->GetInfoAsJSONObject();
		classes[i + 1] = classInfo; 
		i++;
	}

	std::string json = classes.toStyledString();

	FILE* fl = OpenFile(fileName, "w");
	if (!fl)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, fileName);
		return false;
	}
	fprintf(fl, "%s\n", json.c_str());
	fclose(fl);

	return true;
}

unsigned int Classes::GetCompiledClassCount()const
{
	unsigned int count = 0;

	ClassesMap::const_iterator it = m_classesMap.begin();
	ClassesMap::const_iterator itEnd = m_classesMap.end();

	for ( ; it != itEnd; it++)
	if ((*it).second->IsCompiled())
	{
		count++;
	}

	return count;
}

void Classes::OnEndPass()
{
	ClearActiveClass();
}

std::string Classes::GetClassDeclarationCode(const CClassInfo* const classInfo)const
{
	USE_TPL(classes, classDeclaration);
	USE_INDEX(classDeclaration, className);
	USE_INDEX(classDeclaration, baseClassName);
	USE_INDEX(classDeclaration, classContents);
	USE_INDEX(classDeclaration, singletonCode);
	if (classInfo->GetType() == CLASS_TYPE_SINGLETON)
	{
		USE_TPL(classes, singleton);
		USE_INDEX(singleton, className);
		singletonTpl->SetVariableValue(classNameIndex, classInfo->GetName());		
		classDeclarationTpl->SetVariableValue(singletonCodeIndex, singletonTpl->GetText());
	}
	else
	{
		classDeclarationTpl->SetVariableValue(singletonCodeIndex, "");
	}
	classDeclarationTpl->SetVariableValue(classNameIndex, classInfo->GetName());			
	classDeclarationTpl->SetVariableValue(classContentsIndex, classInfo->GetClassDeclarationCode());			
	classDeclarationTpl->SetVariableValue(baseClassNameIndex, classInfo->GetBaseProtoClassName());			

	return classDeclarationTpl->GetText();
}

std::string Classes::GetClassesDeclarationCode()const
{
	std::string code = "";
	ValuesList classesList;

	ADD_FROM_TPL(classes, forwardDeclarationHeader, code);

	code += CGlobalFunctions::GetInstance()->GetForwardDeclarationCode();
	
	ClassesMap::const_iterator it = m_classesMap.begin();
	ClassesMap::const_iterator itEnd = m_classesMap.end();

	std::vector<CClassInfo*> classes;
	
	for ( ; it != itEnd; it++)
	if (!(*it).second->IsBuiltIn())
	{
		classes.push_back((*it).second);		
		classesList.push_back((*it).second->GetName());
	}

	while (classes.size() > 0)
	{
		std::vector<CClassInfo*>::iterator cit = classes.begin();		
		for ( ; cit != classes.end(); )
		{
			bool needDelete = (((*cit)->GetBaseClass() == NULL) || (find(classes.begin(), classes.end(), (*cit)->GetBaseClass()) == classes.end()));
			if (needDelete)
			{
				code += GetClassDeclarationCode(*cit);
				cit = classes.erase(cit);
			}
			else
			{
				cit++;
			}
		}		
	}

	USE_TPL(classes, classFactory);
	USE_INDEX(classFactory, classes);
	classFactoryTpl->SetVariableValue(classesIndex, classesList);		
	code += classFactoryTpl->GetText();

	return code;
}

bool Classes::CreateClassDeclarations(const std::string& classDeclPath)
{
	FILE* classDeclarationsFile = OpenFile(classDeclPath, "w");
	if (!classDeclarationsFile)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, classDeclPath);
		return false;
	}	
	fprintf(classDeclarationsFile, "%s\n", GetClassesDeclarationCode().c_str());

	fclose(classDeclarationsFile);
	return true;
}

bool Classes::CreateGlobalFunctionsFile(const std::string& filePath)
{
	FILE* fl = OpenFile(filePath, "w");
	if (!fl)
	{
		CErrorReporter::GetInstance()->OnFatalError(FILE_WRITE_ERROR, 0xFFFFFFFF, filePath);
		return false;
	}	
	fprintf(fl, "%s\n", CGlobalFunctions::GetInstance()->GetNamedGlobalFunctionsSupportCode().c_str());
	fprintf(fl, "%s\n", GetClassesRedirectCode().c_str());
	fclose(fl);
	return true;
}	

void Classes::FinalizeClasses()
{
	std::vector<MethodParameter> parameters;
	std::vector<bool> outStatus;

	ClassesMap::iterator it = m_classesMap.begin();
	ClassesMap::iterator itEnd = m_classesMap.end();

	for ( ; it != itEnd; it++)
	{
		if ((!(*it).second->IsChildClass()) && (!(*it).second->IsBuiltIn()))
		if (!(*it).second->IsMethodExist(CLASS_ON_CREATED_CALLBACK))
		{		 
			(*it).second->AddMethod(CLASS_ON_CREATED_CALLBACK, 0, 0, outStatus, parameters, true, "");
		}
	}
}

void Classes::CheckValidators()
{
	ClassesMap::iterator it = m_classesMap.begin();
	ClassesMap::iterator itEnd = m_classesMap.end();

	for ( ; it != itEnd; it++)
	{
		(*it).second->CheckValidators();
	}
}

std::string Classes::GetClassesRedirectCode()const
{
	ValuesList classes;
	ClassesMap::const_iterator it = m_classesMap.begin();
	ClassesMap::const_iterator itEnd = m_classesMap.end();
	
	for ( ; it != itEnd; it++)
	if (!(*it).second->IsBuiltIn())
	{
		classes.push_back((*it).second->GetName());
	}

	USE_TPL(classes, functionCallByClass);
	USE_INDEX(functionCallByClass, classes);	
	
	functionCallByClassTpl->SetVariableValue(classesIndex, classes);			
	
	return functionCallByClassTpl->GetText();	
}

void Classes::SetCurrentMethod(const std::string& name, const unsigned int line)
{
	m_currentMethod = name;
	m_currentMethodDeclarationLine = line;
}

std::string Classes::GetCurrentMethod()const
{
	return m_currentMethod;
}

unsigned int Classes::GetCurrentMethodDeclarationLine()const
{
	return m_currentMethodDeclarationLine;
}

std::vector<std::string> Classes::GetClassesList()const
{
	std::vector<std::string> list;
	list.resize(m_classesMap.size());

	ClassesMap::const_iterator it = m_classesMap.begin();
	ClassesMap::const_iterator itEnd = m_classesMap.end();

	for ( ; it != itEnd; it++)
	{
		list[(*it).second->GetID()] = (*it).second->GetName();
	}

	return list;
}

Classes::~Classes()
{
	ClassesMap::iterator it = m_classesMap.begin();
	ClassesMap::iterator itEnd = m_classesMap.end();

	for ( ; it != itEnd; it++)
	{
		delete (*it).second;
	}
}