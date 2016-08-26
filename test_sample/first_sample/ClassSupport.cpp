
#include "ClassSupport.h"
#include "ArraySupport.h"
#include "HashtableSupport.h"
#include "StandardFuncs.h"
#include "json.h"
#include <assert.h>
#include "LogManager.h"
#include <string.h>
#include "SymbolEnums.h"
#include "tinyxml2.h"
#include "GlobalMethods.h"

using namespace tinyxml2;

CClassDataContainer::CClassDataContainer()
{
	static int globalIdCounter = 0;
	globalIdCounter++;

	m_uniqueID = CUniVar(globalIdCounter).ToString();
}

CClassDataContainer::CClassDataContainer(const CClassDataContainer& other)
{
	assert(false);
}

void CClassDataContainer::operator =(const CClassDataContainer& other)
{
	assert(false);
}

std::string CClassDataContainer::GetUniqueID()const
{
	return m_uniqueID;
}

CUniVar& CClassDataContainer::GetProperty__(const int callStatus, const ClassPropertyEnum propId) 
{ 
	if (propId == __PROP_ENUM__properties)
	{
		static CUniVar propsVar;

		CArrayDataContainer* adt = new CArrayDataContainer();

		std::map<std::string, SPropertyStruct*>::iterator it = m_varsByName.begin();
		std::map<std::string, SPropertyStruct*>::iterator itEnd = m_varsByName.end();
		for ( ; it != itEnd; it++)
		{
			adt->Push((*it).first);
		}

		propsVar = CUniVar(adt);
		
		return propsVar;
	}

	std::map<ClassPropertyEnum, SPropertyStruct*>::iterator it = m_vars.find(propId);  
	if (it != m_vars.end())  
	{  
		if (callStatus >= ((*it).second)->callStatus) 
		{ 
			return *(((*it).second)->var);  
		} 
		else 
		{ 
			std::string errorText = "Cannot find property ";
			if ((propId >= 0) && (propId < __PROP_ENUM__COUNT))
			{								
				errorText += propertyNames[propId];				
			}
			else 
			{
				ClassMethodEnum methodPropId = (ClassMethodEnum)propId;
				if ((methodPropId >= 0) && (methodPropId < __FUNC_ENUM__COUNT))
				{
					errorText += methodNames[methodPropId];				
				}
				else
				{
					assert(false);
				}
			}
			print2(warnings, CUniVar(errorText));
			static CUniVar nullVar;
			nullVar.SwitchToUnknownState(); 
			return nullVar; 
		} 
	}  
	else  
	{  
		SPropertyStruct* newProp = new SPropertyStruct(new CUniVar(), (callStatus == 2) ? 1 : 0); 
		m_vars.insert(std::map<ClassPropertyEnum, SPropertyStruct*>::value_type(propId, newProp));  
		m_varsByName.insert(std::map<std::string, SPropertyStruct*>::value_type(propertyNames[(int)propId], newProp));
		return *(newProp->var);  
	}  
} 

void CClassDataContainer::SetPropertyByJsonValue(CUniVar& prop, Json::Value val)
{
	if (val.isBool())
	{
		prop = val.asBool();
	}
	else if (val.isInt())
	{
		prop = val.asInt();
	}
	else if (val.isDouble())
	{
		prop = val.asDouble();
	}
	else 
	{
		prop = val.asString();
	}
}

void CClassDataContainer::HandleJSONElement(Json::Value& val, const std::string& name)
{
	if ((!val.isObject()) && (!val.isArray()))
	{
		SetPropertyByJsonValue(GetProperty__(name), val);		
	}
	else if ((val.isObject()) || (val.isArray()))
	{				
		GetProperty__(name);
		std::map<std::string, SPropertyStruct*>::iterator itV = m_varsByName.find(name);
		if (itV != m_varsByName.end())
		{
			delete (*itV).second->var;			
			if (val.isObject())
			{
				(*itV).second->var = new CUniClass<CClassDataContainer>(false);
			}
			else
			{
				(*itV).second->var = new CUniClass<CArrayDataContainer>(false);
			}
			(*itV).second->var->UpdateStateFromJson(val.toStyledString());
		}
		else
		{
			assert(false);
		}		
	}
}

bool CClassDataContainer::UpdateStateFromJson(const std::string& json)
{
	Json::Value root;
	Json::Reader reader;
	bool parseSuccess = reader.parse(json, root, false);

	if (!parseSuccess)
	{
		std::string errorText = reader.getFormattedErrorMessages();
		if (errorText.size() > 0)
		{
			Log.Warnings.AddLine2s("JSON parse error '" + errorText + "' for json string '" + json + "' after line %s in %s", currentLine, getCurrentMethodName());	
		}
		else
		{
			Log.Warnings.AddLine2s("Error on attempt to parse JSON string '" + json + "' after line %s in %s", currentLine, getCurrentMethodName());
		}
		return false;
	}

	Json::Value::Members members = root.getMemberNames();
	
	Json::Value::Members::iterator it = members.begin();
	Json::Value::Members::iterator itEnd = members.end();

	for ( ; it != itEnd; it++)
	{
		HandleJSONElement(root[*it], *it);
	}

	return true;
}

bool CClassDataContainer::UpdateStateFromXml(void* rootElement)
{
	XMLElement* element = (XMLElement*)rootElement;

	const XMLAttribute* attrib = element->FirstAttribute();
	
	while (attrib != NULL)
	{
		CUniVar& var = GetProperty__(attrib->Name());
		var = attrib->Value();		
		var.UpdateType();

		attrib = attrib->Next();
	} 

	XMLNode* child = element->FirstChild();

	while (child != NULL)
	{
		std::string name = child->ToElement()->Name();
		GetProperty__(name);
		std::map<std::string, SPropertyStruct*>::iterator itV = m_varsByName.find(name);
		if (itV != m_varsByName.end())
		{
			delete (*itV).second->var;			
			(*itV).second->var = new CUniClass<CClassDataContainer>(false);
			(*itV).second->var->UpdateStateFromXml(child);
		}
		else
		{
			assert(false);
		}		
		
		child = child->NextSibling();
	} 

	return true;
}

bool CClassDataContainer::IsMethodExist(CUniVar name)
{
	return false;
}

std::string CClassDataContainer::ToJson()
{
	return ToJsonValue().toStyledString();
}

bool CClassDataContainer::IsPropertyExist(const std::string& name)const
{
	std::map<std::string, SPropertyStruct*>::const_iterator it = m_varsByName.find(name);
	return (it != m_varsByName.end());
}

CUniVar& CClassDataContainer::GetProperty__(const std::string& name)
{
	std::map<std::string, SPropertyStruct*>::iterator it = m_varsByName.find(name);
	if (it != m_varsByName.end())
	{
		return *((*it).second->var);	
	}

	int foundIndex = -1;

	for (unsigned int k = 0; k < __PROP_ENUM__COUNT; k++)
	if (strcmp(propertyNames[k], name.c_str()) == 0)
	{
		foundIndex = k;
		break;
	}

	if (foundIndex != -1)
	{
		return GetProperty__(1, (ClassPropertyEnum)foundIndex);
	}

	SPropertyStruct* newProp = new SPropertyStruct(new CUniVar(), 0 /* public prop */);
	m_varsByName.insert(std::map<std::string, SPropertyStruct*>::value_type(name, newProp));
	return *newProp->var;	
}

unsigned int CClassDataContainer::GetPropertyCount()const
{
	return m_varsByName.size();
}

Json::Value CClassDataContainer::ToJsonValue()
{
	Json::Value root;		

	std::map<std::string, SPropertyStruct*>::const_iterator it = m_varsByName.begin();
	std::map<std::string, SPropertyStruct*>::const_iterator itEnd = m_varsByName.end();

	for ( ; it != itEnd; it++)
	if (((*it).second)->var->GetType() != UV_FUNCTION)
	{
		root[(*it).first] = ((*it).second)->var->ToJsonValue();
	}
		
	if (root.empty())
	{
		// workaround to get empty JSON object (to avoid 'null' value)
		root["tmp"] = 1;
		root.removeMember("tmp");
	}

	return root;
}

std::string CClassDataContainer::ToXml()
{
	XMLDocument xml;
	XMLNode* root = xml.LinkEndChild(xml.NewElement("root"));

	AddToXml(&xml, root);

	std::string result = "";
	XMLPrinter printer;	
	xml.Accept(&printer);
	result = printer.CStr();	

	return result;
}

void CClassDataContainer::HandleXMLElement(void* _doc, void* _root, const std::string& key, CUniVar* value)const
{
	XMLElement* root = (XMLElement*)_root;
	XMLDocument* doc = (XMLDocument*)_doc;

	if (value->GetType() == UV_CLASS)
	{
		XMLElement* elem = doc->NewElement(key.c_str());
		value->ToClass()->AddToXml(doc, elem);
		root->LinkEndChild(elem);
	}
	else if	(value->GetType() == UV_HASHTABLE)
	{
		XMLElement* elem = doc->NewElement(key.c_str());
		value->ToHashtable()->AddToXml(doc, elem);
		root->LinkEndChild(elem);
	}
	else if (value->GetType() == UV_ARRAY)
	{
		Log.Warnings.AddLine2s("Array was skipped during XML construction... after line %s in %s", currentLine, getCurrentMethodName());
	}
	else 
	{
		root->SetAttribute(key.c_str(), value->ToString().c_str());
	}
}

void CClassDataContainer::AddToXml(void* _doc, void* _root)
{
	std::map<std::string, SPropertyStruct*>::const_iterator it = m_varsByName.begin();
	std::map<std::string, SPropertyStruct*>::const_iterator itEnd = m_varsByName.end();

	for ( ; it != itEnd; it++)
	{
		HandleXMLElement(_doc, _root, (*it).first, (*it).second->var);
	}
}

CUniVar CClassDataContainer::CallMethod__(int count, ...) 
{ 
	CUniVar result = getResultObject(false); 
	va_list vl; 
	va_start(vl, count); 
	result = CallMethod__(count, vl); 
	va_end(vl);	 
	return result; 
} 
 
CUniVar CClassDataContainer::CallMethod__(int count, va_list& vl) 
{ 
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) > 0);
	int callStatus = va_arg(vl, int);
	CUniVar result;
	if (CheckForBuiltInMethod(this, method, count, vl, defaultResultNeeded, result))	
	{ 
		return result; 
	}
	Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	return getResultObject(false);
}	

CUniVar CClassDataContainer::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded)
{
	CUniVar result;
	if (CheckForBuiltInMethod(this, method, count, vl, defaultResultNeeded, result))	
	{ 
		return result; 
	}
	Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	return getResultObject(false);
}

CUniVar CClassDataContainer::CallMethodByName__(const std::string& name, CArrayDataContainer* adt)
{
	return getResultObject(false);
}

CUniVar CClassDataContainer::Clone()
{
	CClassDataContainer* cdt = new CClassDataContainer();

	std::map<std::string, SPropertyStruct *>::iterator it = m_varsByName.begin();
	std::map<std::string, SPropertyStruct *>::iterator itEnd = m_varsByName.end();

	for ( ; it != itEnd; it++)
	{
		cdt->GetProperty__((*it).first) = (*it).second->var;
	}

	return getResultObjectV(true, CUniVar(cdt));
}

bool CClassDataContainer::CheckForBuiltInMethod(void* MY_, ClassMethodEnum method, int count, va_list& vl, bool defaultResultNeeded, CUniVar& result)
{	
	CClassDataContainer* MY = (CClassDataContainer*)MY_;

	if (method == __FUNC_ENUM__clone)
	{
		result = MY->Clone();	
	}
	else if ((method == __FUNC_ENUM__convertable) && (count == 4))
	{
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = CUniVar::IsConvertableTypeToType("class", arg0->ToString());		
	}
	else
	{
		std::map<ClassPropertyEnum, SPropertyStruct *>::iterator it = MY->m_vars.find((ClassPropertyEnum)method);
		if (it != MY->m_vars.end())
		{
			if ((*it).second->var->GetType() == UV_FUNCTION)
			{
				FunctionPointer fp = (*it).second->var->ToFunctionPointer();
				if ((fp.funcEnum == method) && (fp.instancePtr == (int64_t)MY))
				{
					return false;
				}

				CArrayDataContainer adt;
				for (int i = 0; i < count - 3; i++)
				{
					CUniVar* arg0 = va_arg(vl, CUniVar*); 
					adt.Push(*arg0);
				}

				if (fp.funcEnum == 0)
				{
					result = CallGlobalFunctionByName__(MY, fp.name, &adt);
					return true;
				}
				else
				{
					result = ::CallMethodByName__(fp.className, MY, methodNames[fp.funcEnum], &adt);
					return true;	
				}
			}
			else
			{				
				result = getResultObject(false);
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	if (defaultResultNeeded) 
	{ 
		result.PermitChangeType();
		result = result.GetProperty__(1, __PROP_ENUM__result); 
		result.ProhibitChangeType();
	}	

	return true;
}

void CClassDataContainer::GetAssociativeContainers(std::vector<std::string>& names, std::vector<CUniVar *>& vars)
{
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator it = m_vars.begin(); 
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator itEnd = m_vars.end(); 

	for ( ; it != itEnd; it++) 
	if (((*it).second->var->GetType() == UV_CLASS) || ((*it).second->var->GetType() == UV_HASHTABLE))
	{
		names.push_back(propertyNames[(*it).first]);
		vars.push_back((*it).second->var);
	}	
}

void CClassDataContainer::GetSimpleProperties(std::vector<std::string>& names, std::vector<CUniVar *>& vars)
{
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator it = m_vars.begin(); 
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator itEnd = m_vars.end(); 

	for ( ; it != itEnd; it++) 
	if (((*it).second->var->GetType() != UV_CLASS) && ((*it).second->var->GetType() != UV_HASHTABLE)
			&& ((*it).second->var->GetType() != UV_ARRAY))
	{
		names.push_back(propertyNames[(*it).first]);
		vars.push_back((*it).second->var);
	}
}

CUniVar& CClassDataContainer::GetProperty(const std::string& name)
{
	return GetProperty__(name);
}

void CClassDataContainer::Remove(const CUniVar index)
{
	std::map<std::string, SPropertyStruct *>::iterator vit = m_varsByName.find(index);

	if (vit != m_varsByName.end())
	{
		std::map<ClassPropertyEnum, SPropertyStruct *>::iterator it = m_vars.begin(); 
		std::map<ClassPropertyEnum, SPropertyStruct *>::iterator itEnd = m_vars.end(); 

		for ( ; it != itEnd; it++) 
		if ((*vit).second == (*it).second)
		{
			m_vars.erase(it);						
			break;
		}

		delete (*vit).second;
		m_varsByName.erase(vit);
	}
}

bool CClassDataContainer::IsSet(const std::string& name)const
{
	return IsPropertyExist(name);
}

bool CClassDataContainer::Call(const std::string& methodName)
{
	for (unsigned int i = 0; i < __FUNC_ENUM__COUNT; i++)
	if (strcmp(methodNames[i], methodName.c_str()) == 0)
	{		
		va_list vl;
		CallMethod__(3, vl, (ClassMethodEnum)i, 1, 0);
		return true;
	}

	return false;
}

CUniVar CClassDataContainer::onCreated(CClassDataContainer* MY)
{
	return getResultObject(true);
}

CUniVar& CClassDataContainer::operator[](char* index)
{
	return GetProperty(index);
}

CClassDataContainer::~CClassDataContainer() 
{ 
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator it = m_vars.begin(); 
	std::map<ClassPropertyEnum, SPropertyStruct *>::iterator itEnd = m_vars.end(); 

	for ( ; it != itEnd; it++) 
	{ 			
		delete (*it).second; 
	} 
} 

CUniVar ObjectProto::CallMethod__(int count, va_list& vl) 
{ 
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) > 0);
	int callStatus = va_arg(vl, int);
	CUniVar result;
	if (CheckForBuiltInMethod(this, method, count, vl, defaultResultNeeded, result))	
	{ 
		return result; 
	} 	
	if ((method == __FUNC_ENUM__onCreated) && (count == 3) && (callStatus >= 0)) 
	{ 

	} 
	else
	{
		Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	}
	return false;
}	

CUniVar ObjectProto::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded)
{
	CUniVar result;
	if (CheckForBuiltInMethod(this, method, count, vl, defaultResultNeeded, result))	
	{ 
		return result; 
	} 	
	if ((method == __FUNC_ENUM__onCreated) && (count == 3) && (callStatus >= 0)) 
	{ 

	} 
	else
	{
		Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	}
	return false;
}