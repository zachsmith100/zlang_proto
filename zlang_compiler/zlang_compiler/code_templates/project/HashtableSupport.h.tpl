
#pragma once

#include "UniVar.h"
#include "ClassSupport.h"

struct SHashtablePair
{
	CUniVar key;
	CUniVar value;

	SHashtablePair(const CUniVar& _key, const CUniVar& _value)
	{
		key = _key;
		value = _value;
	}
};

struct SShiftIndexData
{
	unsigned int index;
	unsigned int* shift;

	SShiftIndexData(const unsigned int _index, unsigned int* _shift)
	{
		index = _index;
		shift = _shift;
	}

	SShiftIndexData(const SShiftIndexData& other)
	{
		index = other.index;
		shift = other.shift;
	}
};

typedef std::map<std::string, SHashtablePair*> PairsMap;

class CHashtableValueContainer;

class CHashtableDataContainer : public CClassDataContainer
{
public:
	CHashtableDataContainer();
	~CHashtableDataContainer();

	unsigned int GetLength()const;

	CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	CUniVar& GetProperty__(const std::string& name);

	CUniVar CallMethod__(int count, ...);
	CUniVar CallMethod__(int count, va_list& vl);
	CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);
		
	CUniVar GetValue(const CUniVar& index);
	void SetValue(const CUniVar& index, const CUniVar& value);
		
	CUniVar RemoveKey(const CUniVar& index);
	bool KeyExists(const CUniVar& value);
	
	CUniVar Clone();

	void HandleJSONElement(Json::Value& val, const std::string& name);

	virtual Json::Value ToJsonValue();

	CUniVar& GetElement(const CUniVar& index);	

	bool IsEqual(CHashtableDataContainer& hdt);	

	CUniVar& GetElementByIndex(const unsigned int index);
	CUniVar& GetKeyByIndex(const unsigned int index);
	CUniVar& GetValueByIndex(const unsigned int index);

	void RegisterShiftVaribleForIndex(const unsigned int index, unsigned int* const shift);
	void UnregisterShiftVarible(unsigned int* const shift);

	void AddIterationObject(CHashtableValueContainer* obj);
	void RemoveIterationObject(CHashtableValueContainer* obj);

	void AddToXml(void* _doc, void* _root);

	void GetAssociativeContainers(std::vector<std::string>& names, std::vector<CUniVar *>& vars);
	void GetSimpleProperties(std::vector<std::string>& names, std::vector<CUniVar *>& vars);

	void Remove(const CUniVar index);

	static bool IsMethodExist(ClassMethodEnum method);

private:
	void Clear();	

	CUniVar GetValues()const;
	CUniVar GetKeys()const;

	CUniVar m_tempObj;

	PairsMap m_content;
	std::vector<SHashtablePair*> m_elements;

	std::vector<SShiftIndexData> m_shiftIndicesData;

	std::vector<CHashtableValueContainer*> m_iterationObjects;
};

#define Hashtable CHashtableDataContainer

enum HashtableValueStatus
{
	OK,
	VALUE_DELETED,
	HASHTABLE_DELETED
};

class CHashtableValueContainer : public CClassDataContainer
{
public:
	CHashtableValueContainer(const CUniVar& key, CUniVar* value, CHashtableDataContainer* parent);
	virtual ~CHashtableValueContainer();

	CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	CUniVar& GetProperty__(const std::string& name);

	CUniVar CallMethod__(int count, ...);
	CUniVar CallMethod__(int count, va_list& vl);
	CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);

	void OnValueDestroyed(CUniVar* value);
	void OnHashtableDestroyed();

	CUniVar& GetValue();

private:
	void PrintNoMethodError();

	CUniVar m_key;
	CUniVar* m_value;

	CHashtableDataContainer* m_parent;

	CUniVar m_tempVar;
	HashtableValueStatus m_status;	
};

template <class T1, class T2>
CUniVar ConvertTo(const std::map<T1, T2>& mp)
{
	CHashtableDataContainer* hdt = new CHashtableDataContainer();

	typename std::map<T1, T2>::const_iterator it = mp.begin();
	typename std::map<T1, T2>::const_iterator itEnd = mp.end();

	for ( ; it != itEnd; it++)
	{	
		hdt->SetValue((*it).first, (*it).second);		
	}

	return CUniVar(hdt);
}