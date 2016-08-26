
#include "HashtableSupport.h"
#include "StandardFuncs.h"
#include "json.h"
#include <assert.h>
#include "LogManager.h"
#include <string.h>
#include "SymbolEnums.h"
#include "ArraySupport.h"
#include <algorithm>
#include "tinyxml2.h"

using namespace tinyxml2;

CHashtableDataContainer::CHashtableDataContainer()
{
}

void CHashtableDataContainer::Clear()
{
	m_content.clear();

	std::vector<SHashtablePair*>::iterator it = m_elements.begin();
	std::vector<SHashtablePair*>::iterator itEnd = m_elements.end();
	for ( ; it != itEnd; it++)
	{
		delete *it;
	}

	std::vector<SShiftIndexData>::iterator itSI = m_shiftIndicesData.begin();
	std::vector<SShiftIndexData>::iterator itSIEnd = m_shiftIndicesData.end();

	for ( ; itSI != itSIEnd; itSI++)
	{
		(*((*itSI).shift)) = (*itSI).index;
	}
	
	m_elements.clear();

	std::vector<CHashtableValueContainer*>::iterator ito = m_iterationObjects.begin();
	std::vector<CHashtableValueContainer*>::iterator itoEnd = m_iterationObjects.end();

	for ( ; ito != itoEnd; ito++)
	{
		(*ito)->OnValueDestroyed(NULL);
	}
}

CUniVar& CHashtableDataContainer::GetElement(const CUniVar& index)
{
	std::string hash = index.GetHashAsString();
	PairsMap::iterator it = m_content.find(hash);
	if (it != m_content.end())
	{
		return (*it).second->value;		
	}
	
	SetValue(index, CUniVar());
	return GetElement(index);	
}

CUniVar& CHashtableDataContainer::GetElementByIndex(const unsigned int index)
{	
	if (m_elements.size() > index)
	{		
		CClassDataContainer* cdt = new CHashtableValueContainer(m_elements[index]->key, &m_elements[index]->value, this);		
		m_tempObj.SwitchToUnknownState();
		m_tempObj = CUniVar(cdt);
		return m_tempObj;
	}

	m_tempObj.SwitchToUnknownState();
	return m_tempObj;
}

unsigned int CHashtableDataContainer::GetLength()const
{
	return m_elements.size();
}

CUniVar& CHashtableDataContainer::GetProperty__(const int callStatus, const ClassPropertyEnum propId)
{
	if (propId == __PROP_ENUM__length) 
	{
		m_tempObj.SwitchToUnknownState();
		m_tempObj = (unsigned int)m_elements.size();
		return m_tempObj;
	}

	return CClassDataContainer::GetProperty__(callStatus, propId);
}

CUniVar& CHashtableDataContainer::GetProperty__(const std::string& name)
{
	if (name == "length")
	{
		m_tempObj.SwitchToUnknownState();
		m_tempObj = (unsigned int)m_elements.size();
		return m_tempObj;
	}

	return CClassDataContainer::GetProperty__(name);
}

CUniVar CHashtableDataContainer::CallMethod__(int count, ...) 
{ 
	CUniVar result = getResultObject(false); 
	va_list vl; 
	va_start(vl, count); 
	result = CallMethod__(count, vl); 
	va_end(vl);	 
	return result; 
} 
 
CUniVar CHashtableDataContainer::CallMethod__(int count, va_list& vl) 
{ 
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) > 0);
	int callStatus = va_arg(vl, int);  
	return CallMethod__(count, vl, method, callStatus, defaultResultNeeded); 
}			 

CUniVar CHashtableDataContainer::GetValue(const CUniVar& index)
{	
	std::string hash = index.GetHashAsString();

	PairsMap::iterator it = m_content.find(hash);
	if (it != m_content.end())
	{
		return (*it).second->value;
	}
	
	m_tempObj.SwitchToUnknownState();
	return m_tempObj;
}

void CHashtableDataContainer::SetValue(const CUniVar& index, const CUniVar& value)
{	
	std::string hash = index.GetHashAsString();

	PairsMap::iterator it = m_content.find(hash);
	if (it == m_content.end())
	{
		SHashtablePair* pair = new SHashtablePair(index, value);
		m_content.insert(PairsMap::value_type(hash, pair));
		m_elements.push_back(pair);
	}
}

CUniVar CHashtableDataContainer::Clone()
{
	CHashtableDataContainer* adt = new CHashtableDataContainer();

	PairsMap::iterator it = m_content.begin();
	PairsMap::iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)
	{
		adt->SetValue((*it).second->key, (*it).second->value);
	}

	CUniVar res(adt);
	return getResultObject(true, res);
}

void CHashtableDataContainer::Remove(const CUniVar index)
{
	RemoveKey(index);
}

CUniVar CHashtableDataContainer::RemoveKey(const CUniVar& index)
{
	std::string hash = index.GetHashAsString();
	PairsMap::iterator it = m_content.find(hash);
	if (it == m_content.end())
	{
		return getFullResultObject(false);
	}

	std::vector<CHashtableValueContainer*>::iterator ito = m_iterationObjects.begin();
	std::vector<CHashtableValueContainer*>::iterator itoEnd = m_iterationObjects.end();

	for ( ; ito != itoEnd; ito++)
	{
		(*ito)->OnValueDestroyed(&((*it).second->value));
	}

	std::vector<SHashtablePair *>::iterator itV = find(m_elements.begin(), m_elements.end(), (*it).second);	
	assert(itV != m_elements.end());
	if (itV != m_elements.end())
	{
		unsigned int numericIndex = itV - m_elements.begin();
		
		std::vector<SShiftIndexData>::iterator itSI = m_shiftIndicesData.begin();
		std::vector<SShiftIndexData>::iterator itSIEnd = m_shiftIndicesData.end();

		for ( ; itSI != itSIEnd; itSI++)
		if (numericIndex <= (*itSI).index)
		{
			(*itSI).index--;
			(*((*itSI).shift))++;
		}

		m_elements.erase(itV);
	}

	delete (*it).second;
	m_content.erase(it);
		
	return getFullResultObject(true);
}

CUniVar CHashtableDataContainer::GetValues()const
{
	CArrayDataContainer* adt = new CArrayDataContainer();

	PairsMap::const_iterator it = m_content.begin();
	PairsMap::const_iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)	
	{
		adt->Push((*it).second->value);
	}

	return getResultObjectV(true, CUniVar(adt));	
}

CUniVar CHashtableDataContainer::GetKeys()const
{
	CArrayDataContainer* adt = new CArrayDataContainer();

	PairsMap::const_iterator it = m_content.begin();
	PairsMap::const_iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)	
	{
		adt->Push((*it).second->key);
	}

	return getResultObjectV(true, CUniVar(adt));
}

bool CHashtableDataContainer::KeyExists(const CUniVar& value)
{
	std::string hash = value.GetHashAsString();

	PairsMap::iterator it = m_content.find(hash);
	return it != m_content.end();
}
 
CUniVar CHashtableDataContainer::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded) 
{ 
	CUniVar result = getResultObject(false); 
	if ((method == __FUNC_ENUM__keyExists) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = getFullResultObject(KeyExists(*arg0)); 
	} 
	else if ((method == __FUNC_ENUM__removeKey) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = RemoveKey(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__clear) && (count == 3) && (callStatus >= 0)) 
	{ 		
		Clear(); 
		result = getFullResultObject(true);
	} 
	else if ((method == __FUNC_ENUM__convertable) && (count == 4) && (callStatus >= 0)) 
	{ 		
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = CUniVar::IsConvertableTypeToType("hashtable", arg0->ToString());		
	} 
	else if ((method == __FUNC_ENUM__clone) && (count == 3) && (callStatus >= 0)) 
	{ 		
		result = Clone(); 
	} 
	else if ((method == __FUNC_ENUM__getValues) && (count == 3) && (callStatus >= 0)) 
	{ 		
		result = GetValues(); 
	} 
	else if ((method == __FUNC_ENUM__getKeys) && (count == 3) && (callStatus >= 0)) 
	{ 		
		result = GetKeys(); 
	} 
	else if (count == 1) 
	{ 
		return GetProperty__(callStatus, (ClassPropertyEnum)method); 
	}			 
	else 
	{ 
		Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	} 
	if (defaultResultNeeded) 
	{ 
		result.PermitChangeType();
		result = result.GetProperty__(1, __PROP_ENUM__result); 
		result.ProhibitChangeType();
	} 
	return result; 
} 

bool CHashtableDataContainer::IsMethodExist(ClassMethodEnum method)
{
	if ((method == __FUNC_ENUM__keyExists) || (method == __FUNC_ENUM__removeKey) ||
		(method == __FUNC_ENUM__clear) || (method == __FUNC_ENUM__getValues) ||
		(method == __FUNC_ENUM__getKeys))
	{	
		return true;
	}

	return false;
}

void CHashtableDataContainer::HandleJSONElement(Json::Value& val, const std::string& name)
{
	if ((!val.isObject()) && (!val.isArray()))
	{
		CUniVar prop;
		SetPropertyByJsonValue(prop, val);		
		GetElement(name) = prop;
	}
	else if ((val.isObject()) || (val.isArray()))
	{				
		CUniVar prop;
		if (val.isObject())
		{
			prop = new CUniClass<CClassDataContainer>(false);
		}
		else
		{
			prop = new CUniClass<CArrayDataContainer>(false);
		}
		prop->UpdateStateFromJson(val.toStyledString());
		GetElement(name) = prop;
	}
	else
	{
		assert(false);
	}			
}

Json::Value CHashtableDataContainer::ToJsonValue()
{
	Json::Value root;	

	PairsMap::iterator it = m_content.begin();
	PairsMap::iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)
	{
		UniVarType type = (*it).second->key.GetType();
		if ((type == UV_CLASS) || (type == UV_HASHTABLE) || (type == UV_ARRAY) || (type == UV_FUNCTION))
		{
			Log.Warnings.AddLine3s("Element of hashtable is skipped in json conversion process because of unsupported type of key (%s) after line %s in %s", (*it).second->key.GetTypeAsString(), currentLine, getCurrentMethodName());
			continue;
		}

		root[(*it).second->key.ToString()] = (*it).second->value.ToJsonValue();
	}

	if (m_content.size() == 0)
	{
		// workaround to get empty JSON object (to avoid 'null' value)
		root["tmp"] = 1;
		root.removeMember("tmp");
	}	

	return root;
}

void CHashtableDataContainer::AddToXml(void* _doc, void* _root)
{
	PairsMap::iterator it = m_content.begin();
	PairsMap::iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)
	{
		UniVarType type = (*it).second->key.GetType();
		if ((type == UV_CLASS) || (type == UV_HASHTABLE) || (type == UV_ARRAY) || (type == UV_FUNCTION))
		{
			Log.Warnings.AddLine3s("Element of hashtable is skipped in XML conversion process because of unsupported type of key (%s) after line %s in %s", (*it).second->key.GetTypeAsString(), currentLine, getCurrentMethodName());
			continue;
		}

		type = (*it).second->value.GetType();
		if (type == UV_ARRAY)
		{
			Log.Warnings.AddLine3s("Element of hashtable is skipped in XML conversion process because of unsupported type of value (%s) after line %s in %s", (*it).second->value.GetTypeAsString(), currentLine, getCurrentMethodName());
			continue;
		}

		HandleXMLElement(_doc, _root, (*it).second->key, &(*it).second->value);		
	}	
}

bool CHashtableDataContainer::IsEqual(CHashtableDataContainer& hdt)
{
	if (m_content.size() != hdt.m_content.size())
	{
		return false;
	}

	PairsMap::iterator it1 = m_content.begin();
	PairsMap::iterator it1End = m_content.end();
	PairsMap::iterator it2 = hdt.m_content.begin();	
	for ( ; it1 != it1End; it1++, it2++)
	if ((*it1).first != (*it2).first)
	{
		return false;
	}
	else if ((*it1).second->key != (*it2).second->key)
	{
		return false;
	}
	else if ((*it1).second->value != (*it2).second->value)
	{
		return false;
	}

	return true;
}

void CHashtableDataContainer::RegisterShiftVaribleForIndex(const unsigned int index, unsigned int* const shift)
{
	assert(shift);
	if (shift)
	{
		SShiftIndexData data(index, shift);
		m_shiftIndicesData.push_back(data);
	}
}

void CHashtableDataContainer::UnregisterShiftVarible(unsigned int* const shift)
{
	std::vector<SShiftIndexData>::iterator it = m_shiftIndicesData.begin();
	std::vector<SShiftIndexData>::iterator itEnd = m_shiftIndicesData.end();

	for ( ; it != itEnd; it++)
	if ((*it).shift == shift)
	{
		m_shiftIndicesData.erase(it);
		break;
	}
}

void CHashtableDataContainer::AddIterationObject(CHashtableValueContainer* obj)
{
	m_iterationObjects.push_back(obj);
}

void CHashtableDataContainer::RemoveIterationObject(CHashtableValueContainer* obj)
{
	std::vector<CHashtableValueContainer*>::iterator it = find(m_iterationObjects.begin(), m_iterationObjects.end(), obj);
	if (it != m_iterationObjects.end())
	{
		m_iterationObjects.erase(it);
	}
}

void CHashtableDataContainer::GetAssociativeContainers(std::vector<std::string>& names, std::vector<CUniVar *>& vars)
{
	PairsMap::iterator it = m_content.begin();
	PairsMap::iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)
	{
		UniVarType keyType = (*it).second->key.GetType();
		
		if ((keyType != UV_CLASS) && (keyType != UV_HASHTABLE) && (keyType != UV_ARRAY))
		{
			UniVarType valueType = (*it).second->value.GetType();

			if ((valueType == UV_CLASS) || (valueType == UV_HASHTABLE))
			{
				names.push_back((*it).second->key.ToString());
				vars.push_back(&(*it).second->value);
			}
		}
	}	
}

void CHashtableDataContainer::GetSimpleProperties(std::vector<std::string>& names, std::vector<CUniVar *>& vars)
{
	PairsMap::iterator it = m_content.begin();
	PairsMap::iterator itEnd = m_content.end();

	for ( ; it != itEnd; it++)
	{
		UniVarType keyType = (*it).second->key.GetType();
		
		if ((keyType != UV_CLASS) && (keyType != UV_HASHTABLE) && (keyType != UV_ARRAY))
		{
			UniVarType valueType = (*it).second->value.GetType();

			if ((valueType != UV_CLASS) && (valueType != UV_HASHTABLE) && (valueType != UV_ARRAY))
			{
				names.push_back((*it).second->key.ToString());
				vars.push_back(&(*it).second->value);
			}
		}
	}	
}

CUniVar& CHashtableDataContainer::GetKeyByIndex(const unsigned int index)
{
	if (index < m_elements.size())
	{
		return m_elements[index]->key;
	}
	
	m_tempObj.SwitchToUnknownState();
	return m_tempObj;	
}

CUniVar& CHashtableDataContainer::GetValueByIndex(const unsigned int index)
{
	if (index < m_elements.size())
	{
		return m_elements[index]->value;
	}

	m_tempObj.SwitchToUnknownState();
	return m_tempObj;
}

CHashtableDataContainer::~CHashtableDataContainer()
{
	Clear();		

	std::vector<CHashtableValueContainer*>::iterator it = m_iterationObjects.begin();
	std::vector<CHashtableValueContainer*>::iterator itEnd = m_iterationObjects.end();

	for ( ; it != itEnd; it++)
	{
		(*it)->OnHashtableDestroyed();
	}	
}

CHashtableValueContainer::CHashtableValueContainer(const CUniVar& key, CUniVar* value, CHashtableDataContainer* parent):
	m_parent(parent),	
	m_value(value),
	m_status(OK),
	m_key(key)
{
	assert(parent);
	assert(value);

	parent->AddIterationObject(this);
}

CUniVar& CHashtableValueContainer::GetValue()
{
	if (m_status == OK)
	{
		return *m_value;
	}
	else
	{
		if (m_status == VALUE_DELETED)
		{
			Log.Warnings.AddLine2s("Attempt to use deleted hashtable value after line %s in %s", currentLine, getCurrentMethodName());
		}
		else
		{
			Log.Warnings.AddLine2s("Attempt to use value of deleted hashtable after line %s in %s", currentLine, getCurrentMethodName());
		}
		m_tempVar.SwitchToUnknownState();
		return m_tempVar;
	}
}

CUniVar& CHashtableValueContainer::GetProperty__(const int callStatus, const ClassPropertyEnum propId)
{
	if (propId == __PROP_ENUM__key)
	{		
		m_tempVar.SwitchToUnknownState();
		m_tempVar = m_key;
		return m_tempVar;
	}
	else if (propId == __PROP_ENUM__value)
	{
		return GetValue();
	}
	
	Log.Warnings.AddLine2s("Attempt to use unknown property of hashtable value variable after line %s in %s", currentLine, getCurrentMethodName());

	m_tempVar.SwitchToUnknownState();
	return m_tempVar;
}

CUniVar& CHashtableValueContainer::GetProperty__(const std::string& name)
{
	if (name == "key")
	{
		m_tempVar.SwitchToUnknownState();
		m_tempVar = m_key;
		return m_tempVar;
	}
	else if (name == "value")
	{
		return GetValue();
	}
	
	Log.Warnings.AddLine2s("Attempt to use unknown property of hashtable value variable after line %s in %s", currentLine, getCurrentMethodName());	

	m_tempVar.SwitchToUnknownState();
	return m_tempVar;
}

void CHashtableValueContainer::PrintNoMethodError()
{
	Log.Warnings.AddLine2s("Attempt to use unknown method of hashtable value variable after line %s in %s", currentLine, getCurrentMethodName());
}

CUniVar CHashtableValueContainer::CallMethod__(int count, ...)
{
	PrintNoMethodError();
	return getResultObject(false);
}

CUniVar CHashtableValueContainer::CallMethod__(int count, va_list& vl)
{
	PrintNoMethodError();
	return getResultObject(false);
}

CUniVar CHashtableValueContainer::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded)
{
	PrintNoMethodError();
	return getResultObject(false);
}

void CHashtableValueContainer::OnValueDestroyed(CUniVar* value)
{
	if ((m_value == value) || (value == NULL))
	{
		m_status = VALUE_DELETED;
		m_value = NULL;		
	}
}

void CHashtableValueContainer::OnHashtableDestroyed()
{
	m_status = HASHTABLE_DELETED;
	m_value = NULL;
}

CHashtableValueContainer::~CHashtableValueContainer()
{
	if (m_status != HASHTABLE_DELETED)
	{
		m_parent->RemoveIterationObject(this);
	}
}