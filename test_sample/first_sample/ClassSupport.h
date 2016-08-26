#pragma once

#include "UniVar.h"
#include "SymbolEnums.h"
#include <map>
#include "json.h"

struct SPropertyStruct
{
	CUniVar* var;
	int callStatus;

	SPropertyStruct(CUniVar* const _var, const int _callStatus)
	{
		var = _var;
		callStatus = _callStatus;
	}

	SPropertyStruct(const SPropertyStruct& other)
	{
		var = other.var;
		callStatus = other.callStatus;
	}

	~SPropertyStruct()
	{
		delete var;
	}
};

class CClassDataContainer
{
public:
	CClassDataContainer();
	virtual ~CClassDataContainer(); 

	virtual CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	virtual CUniVar& GetProperty__(const std::string& name);
	bool IsPropertyExist(const std::string& name)const;

	virtual bool UpdateStateFromJson(const std::string& json);	
	std::string ToJson();

	bool UpdateStateFromXml(void* rootElement);
	std::string ToXml();

	unsigned int GetPropertyCount()const;		

	virtual Json::Value ToJsonValue();

	virtual CUniVar CallMethod__(int count, ...);
	virtual CUniVar CallMethod__(int count, va_list& vl);
	virtual CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);
	virtual CUniVar CallMethodByName__(const std::string& name, CArrayDataContainer* adt);
	
	virtual void HandleJSONElement(Json::Value& val, const std::string& name);

	std::string GetUniqueID()const;

	static bool CheckForBuiltInMethod(void* MY, ClassMethodEnum method, int count, va_list& vl, bool defaultResultNeeded, CUniVar& result);

	virtual void GetAssociativeContainers(std::vector<std::string>& names, std::vector<CUniVar *>& vars);
	virtual void GetSimpleProperties(std::vector<std::string>& names, std::vector<CUniVar *>& vars);

	virtual bool IsMethodExist(CUniVar name);

	CUniVar& GetProperty(const std::string& name);	

	virtual void Remove(const CUniVar index);
	bool IsSet(const std::string& name)const;

	bool Call(const std::string& methodName);

	static CUniVar onCreated(CClassDataContainer* MY);

	CUniVar& operator[](char* index);

private:
	CClassDataContainer(const CClassDataContainer& other);
	void operator =(const CClassDataContainer& other);

	CUniVar Clone();

protected:
	void SetPropertyByJsonValue(CUniVar& prop, Json::Value val);
	void HandleXMLElement(void* _doc, void* _root, const std::string& key, CUniVar* value)const;
	virtual void AddToXml(void* doc, void* root);

	std::map<ClassPropertyEnum, SPropertyStruct *> m_vars; 
	std::map<std::string, SPropertyStruct *> m_varsByName;
	std::map<std::string, FunctionPointer> m_functions;
	std::string m_uniqueID;
};

template <class T>
class CUniClass : public CUniVar
{
public:
	CUniClass(bool needCreateEvent = true):
		CUniVar(new T())
	{		
		if (needCreateEvent)
		{
			CallMethod__(3, __FUNC_ENUM__onCreated, false, 0); 
		}
	}
};

class ObjectProto : public CClassDataContainer
{
public:
	CUniVar CallMethod__(int count, va_list& vl);
	CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);
};

class Object : public CUniClass<ObjectProto>  
{ 
public:
	Object():
		CUniClass<ObjectProto>(false)
	{
	}
};
