
#include "UniVar.h"
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include "ArraySupport.h"
#include <memory>
#include "Log.h"
#include "LogManager.h"
#include "HashtableSupport.h"
#include "StreamSupport.h"
#include "SymbolEnums.h"
#include "StandardFuncs.h"
#include "StreamSupport.h"
#include <regex>
#include "Utils.h"

#ifndef WIN32
#include <cmath>
#include <cfloat>
#endif

#ifdef WIN32
#define sprintf sprintf_s
#endif

#define JSON_LEVEL_LIMIT	100

#define _INT(v) (*(int64_t*)v)
#define _DOUBLE(v) (*(double*)v)
#define _CHAR(v) (*(char*)v)
#define _STRING(v) (*(std::string*)v)
#define _BOOL(v) (*(bool*)v)
#define _ARB_DOUBLE(v) (*(CArbitraryPrecisionFloat*)v)
#define _CLASS(v) (*(std::shared_ptr<CClassDataContainer>*)v)
#define _ARRAY(v) (*(std::shared_ptr<CArrayDataContainer>*)v)
#define _HASHTABLE(v) (*(std::shared_ptr<CHashtableDataContainer>*)v)
#define _STREAM(v) (*(std::shared_ptr<CStreamDataContainer>*)v)
#define _FUNC(v) (*(FunctionPointer*)v)

static bool permitChangeType = false;

const char* typeNames[TYPES_COUNT] = {"unknown", "integer", "symbol", "string", "double", "boolean", "class", "array", "hashtable", "stream", "function", "arbitrary precision float"};

bool convertMatrix[TYPES_COUNT][TYPES_COUNT] = { 
	// unk, INT, SYMBOL, STRING, DOUBLE, BOOL, CLASS, ARRAY, HASH, STREAM, FUNC, ARB_DBL
	{false, false, false, false, false, false, false, false, false, false, false, false},  // unk
	{false, true , true , false, true , true , false, false, false, false, false, true },  // INT
	{false, true , true , false, false, false, false, false, false, false, false, false},  // SYMBOL
	{false, true , true , true , true , true , true , true , true , false, false, true },  // STRING
	{false, true , true , false, true , true , false, false, false, false, false, true },  // DOUBLE
	{false, true , true , false, true , true , false, false, false, false, false, true },  // BOOLEAN
	{false, false, false, false, false, false, true , false, false, false, false, false},  // CLASS
	{false, false, false, false, false, false, false, true , false, false, false, false},  // ARRAY
	{false, false, false, false, false, false, false, false, true , false, false, false},  // HASHTABLE
	{false, false, false, false, false, false, false, false, false, true , false, false},  // STREAM
	{false, false, false, false, false, false, false, false, false, false, true , false},  // FUNC
	{false, true , false, false, true , false, false, false, false, false, false, true }   // ARB DOUBLE
};

std::string FunctionPointer::GetHash()
{
	std::string hash = name;
	hash += "_!_";
	hash += className;
	hash += "_!_";
	hash += CUniVar(instancePtr).ToString();
	hash += "_!_";
	hash += CUniVar(funcEnum).ToString();
	return hash;
}

template <class T>
bool IsEqual(void* value1, void* value2)
{
	return (*(T*)value1 == *(T*)value2);
}

template <class T>
bool Compare(void* value1, void* value2)
{
	return (*(T*)value1 < *(T*)value2);
}

template <class T>
bool CompareExt(void* value1, void* value2)
{
	return (*(T*)value1 <= *(T*)value2);
}

template <class T>
void Free(void* value)
{
	T* val = (T*)value;
	delete val;
}

bool IsValueEqual(UniVarType varType, void* value1, void* value2)
{
	switch (varType)
	{
	case UV_INT:
		return IsEqual<int>(value1, value2);
		break;

	case UV_SYMBOL:
		return IsEqual<char>(value1, value2);
		break;

	case UV_STRING:
		return IsEqual<std::string>(value1, value2);
		break;

	case UV_DOUBLE:
		return IsEqual<double>(value1, value2);
		break;

	case UV_ARB_DOUBLE:
		return IsEqual<CArbitraryPrecisionFloat>(value1, value2);
		break;

	case UV_BOOLEAN:
		return IsEqual<bool>(value1, value2);
		break;

	case UV_CLASS:
		return false;		
		break;

	case UV_ARRAY:		
		return _ARRAY(value1)->IsEqual(*_ARRAY(value2).get());	

	case UV_HASHTABLE:
		return _HASHTABLE(value1)->IsEqual(*_HASHTABLE(value2).get());	
		break;

	case UV_STREAM:
		return _STREAM(value1).get() == _STREAM(value2).get();	
		break;

	case UV_FUNCTION:
		return IsEqual<FunctionPointer>(value1, value2);
		break;
	
	default:
		return false;
		break;
	}

	return false;
}

bool CompareValue(UniVarType varType, void* value1, void* value2)
{
	switch (varType)
	{
	case UV_INT:
		return Compare<int>(value1, value2);
		break;

	case UV_SYMBOL:
		return Compare<char>(value1, value2);
		break;

	case UV_STRING:
		return Compare<std::string>(value1, value2);
		break;

	case UV_DOUBLE:
		return Compare<double>(value1, value2);
		break;

	case UV_ARB_DOUBLE:
		return Compare<CArbitraryPrecisionFloat>(value1, value2);
		break;

	case UV_BOOLEAN:
		return Compare<bool>(value1, value2);
		break;

	default:
		return false;
		break;
	}

	return false;
}

bool CompareValueExt(UniVarType varType, void* value1, void* value2)
{
	switch (varType)
	{
	case UV_INT:
		return CompareExt<int>(value1, value2);
		break;

	case UV_SYMBOL:
		return CompareExt<char>(value1, value2);
		break;

	case UV_STRING:
		return CompareExt<std::string>(value1, value2);
		break;

	case UV_DOUBLE:
		return CompareExt<double>(value1, value2);
		break;

	case UV_ARB_DOUBLE:
		return CompareExt<CArbitraryPrecisionFloat>(value1, value2);
		break;

	case UV_BOOLEAN:
		return CompareExt<bool>(value1, value2);
		break;

	default:
		return false;
	}

	return false;
}

CUniVar::CUniVar():
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{	
}

CUniVar::CUniVar(const void* ptrVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)ptrVar);	
}

CUniVar::CUniVar(const int64_t intVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)intVar);
}

CUniVar::CUniVar(const long int intVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)intVar);
}

CUniVar::CUniVar(const int intVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)intVar);
}

CUniVar::CUniVar(const unsigned int intVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)intVar);
}

CUniVar::CUniVar(const unsigned long intVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init((int64_t)intVar);
}

CUniVar::CUniVar(const double doubleVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(doubleVar);
}

CUniVar::CUniVar(const CArbitraryPrecisionFloat& doubleVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(doubleVar);
}

CUniVar::CUniVar(const char charVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(charVar);
}

CUniVar::CUniVar(const std::string& strVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(strVar);
}

CUniVar::CUniVar(const FunctionPointer& functionPtr):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(functionPtr);
}

std::string CUniVar::ConvertToString(const std::wstring& strVar)
{
	int size = wcslen(strVar.c_str()) + 1;
	char* result = new char[size];
	int resultSize = 0;
#ifndef WIN32
	wcstombs(result, strVar.c_str(), size);
#else
	wcstombs_s((size_t*)&resultSize, result, size, strVar.c_str(), size);
#endif
	result[size - 1] = '\0';	
	std::string str = result;
	delete[] result;
	return str;
}

CUniVar::CUniVar(const wchar_t* strVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(ConvertToString(strVar));
}

CUniVar::CUniVar(const std::wstring& strVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(ConvertToString(strVar));
}

CUniVar::CUniVar(const char* strVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	std::string str(strVar);
	Init(str);
}

CUniVar::CUniVar(const bool boolVar):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(boolVar);
}

CUniVar::CUniVar(CUniVar* instance):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(instance);
}

CUniVar::CUniVar(CClassDataContainer* instance, const bool notControlInstance):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{		
	Init(instance, notControlInstance);
}

CUniVar::CUniVar(CArrayDataContainer* instance):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(instance);
}

CUniVar::CUniVar(CHashtableDataContainer* instance):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(instance);
}

CUniVar::CUniVar(CStreamDataContainer* instance):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	Init(instance);
}

void CUniVar::Init(const int64_t intVar)
{
	CleanUp(UV_INT);

	m_type = UV_INT;
	m_value = new int64_t;
	m_valueUsedByPtr = false; 
	_INT(m_value) = intVar;
}

void CUniVar::Init(const double doubleVar)
{
	CleanUp(UV_DOUBLE);

	m_type = UV_DOUBLE;
	m_valueUsedByPtr = false; 
	m_value = new double;
	_DOUBLE(m_value) = doubleVar;
}

void CUniVar::Init(const CArbitraryPrecisionFloat& doubleVar)
{
	CleanUp(UV_ARB_DOUBLE);

	m_type = UV_ARB_DOUBLE;
	m_valueUsedByPtr = false; 
	m_value = new CArbitraryPrecisionFloat(doubleVar);
}

void CUniVar::Init(const char charVar)
{
	if ((m_valueUsedByPtr) && (m_type == UV_SYMBOL))
	{
		m_valueUsedByPtr = false; 
		_CHAR(m_value) = charVar;
	}
	else
	{
		CleanUp(UV_SYMBOL);

		m_type = UV_SYMBOL;
		m_value = new char;
		m_valueUsedByPtr = false; 
		_CHAR(m_value) = charVar;
	}
}

void CUniVar::InitWithSymbolPtr(char* charVar)
{
	CleanUp(UV_SYMBOL);

	m_valueUsedByPtr = true;
	m_type = UV_SYMBOL;
	m_value = charVar;
}

void CUniVar::Init(const bool boolVar)
{
	CleanUp(UV_BOOLEAN);
	
	m_type = UV_BOOLEAN;
	m_valueUsedByPtr = false; 
	m_value = new bool;
	_BOOL(m_value) = boolVar;
}

void CUniVar::Init(const std::string strVar)
{
	CleanUp(UV_STRING);

	m_type = UV_STRING;
	m_value = new std::string;
	m_valueUsedByPtr = false; 
	_STRING(m_value) = strVar;
}

void CUniVar::Init(CUniVar* instance)
{
	if (instance->GetType() == UV_CLASS)
	{
		bool valueUsedByPtr = instance->m_valueUsedByPtr;
		if (!valueUsedByPtr)
		{
			std::shared_ptr<CClassDataContainer> cdt = *(std::shared_ptr<CClassDataContainer>*)instance->m_value;
			CleanUp(UV_CLASS);
			m_type = UV_CLASS;
			m_value = new std::shared_ptr<CClassDataContainer>(cdt);						
		}
		else
		{
			CleanUp(UV_CLASS);
			m_type = UV_CLASS;
			m_value = instance->m_value;			
		}
		m_valueUsedByPtr = valueUsedByPtr;
	}
	else if (instance->GetType() == UV_ARRAY)
	{
		std::shared_ptr<CArrayDataContainer> adt = *(std::shared_ptr<CArrayDataContainer>*)instance->m_value;
		CleanUp(UV_ARRAY);
		m_type = UV_ARRAY;
		m_value = new std::shared_ptr<CArrayDataContainer>(adt);	
		m_valueUsedByPtr = false; 
	}
	else if (instance->GetType() == UV_HASHTABLE)
	{
		std::shared_ptr<CHashtableDataContainer> hdt = *(std::shared_ptr<CHashtableDataContainer>*)instance->m_value;
		CleanUp(UV_HASHTABLE);
		m_type = UV_HASHTABLE;
		m_value = new std::shared_ptr<CHashtableDataContainer>(hdt);	
		m_valueUsedByPtr = false; 
	}
	else if (instance->GetType() == UV_STREAM)
	{
		std::shared_ptr<CStreamDataContainer> sdt = *(std::shared_ptr<CStreamDataContainer>*)instance->m_value;
		CleanUp(UV_STREAM);
		m_type = UV_STREAM;
		m_value = new std::shared_ptr<CStreamDataContainer>(sdt);	
		m_valueUsedByPtr = false; 
	}
	else if (instance->GetType() == UV_INT)
	{
		int64_t a = instance->ToInt64();
		CleanUp(UV_INT);
		Init(a);
	}
	else if (instance->GetType() == UV_DOUBLE)
	{
		double a = instance->ToDouble();
		CleanUp(UV_DOUBLE);
		Init(a);
	}
	else if (instance->GetType() == UV_ARB_DOUBLE)
	{
		CArbitraryPrecisionFloat a = instance->ToArbDouble();
		CleanUp(UV_ARB_DOUBLE);
		Init(a);
	}
	else if (instance->GetType() == UV_BOOLEAN)
	{
		bool a = instance->ToBoolean();
		CleanUp(UV_BOOLEAN);
		Init(a);
	}
	else if (instance->GetType() == UV_STRING)
	{
		std::string a = instance->ToString();
		CleanUp(UV_STRING);
		Init(a);
	}
	else if (instance->GetType() == UV_SYMBOL)
	{
		char a = instance->ToSymbol();
		CleanUp(UV_SYMBOL);
		Init(a);
	}
	else if (instance->GetType() == UV_FUNCTION)
	{
		FunctionPointer a = instance->ToFunctionPointer();
		CleanUp(UV_FUNCTION);
		Init(a);
	}
}

void CUniVar::Init(CClassDataContainer* instance, const bool notControlInstance)
{
	CleanUp(UV_CLASS);

	m_type = UV_CLASS;
	if (!notControlInstance)
	{
		m_value = new std::shared_ptr<CClassDataContainer>(instance);
		m_valueUsedByPtr = false; 
	}
	else
	{
		m_value = instance;
		m_valueUsedByPtr = true;
	}
}

void CUniVar::Init(CArrayDataContainer* instance)
{
	CleanUp(UV_ARRAY);

	m_type = UV_ARRAY;
	m_value = new std::shared_ptr<CArrayDataContainer>(instance);
	m_valueUsedByPtr = false; 
}

void CUniVar::Init(CHashtableDataContainer* instance)
{
	CleanUp(UV_HASHTABLE);

	m_type = UV_HASHTABLE;
	m_value = new std::shared_ptr<CHashtableDataContainer>(instance);
	m_valueUsedByPtr = false; 
}

void CUniVar::Init(CStreamDataContainer* instance)
{
	CleanUp(UV_STREAM);

	m_type = UV_STREAM;
	m_value = new std::shared_ptr<CStreamDataContainer>(instance);
	m_valueUsedByPtr = false; 
}

void CUniVar::Init(const FunctionPointer& instance)
{
	CleanUp(UV_FUNCTION);

	m_type = UV_FUNCTION;
	m_value = new FunctionPointer(instance);
	m_valueUsedByPtr = false; 
}

CUniVar::CUniVar(const CUniVar& other):
	m_valueUsedByPtr(false),
	m_type(UV_UNKNOWN),
	m_value(NULL)
{
	switch (other.m_type)
	{
	case UV_INT:
		Init(_INT(other.m_value));
		break;
	case UV_SYMBOL:
		Init(_CHAR(other.m_value));
		break;
	case UV_STRING:
		Init(_STRING(other.m_value));
		break;
	case UV_DOUBLE:
		Init(_DOUBLE(other.m_value));
		break;
	case UV_ARB_DOUBLE:
		Init(_ARB_DOUBLE(other.m_value));
		break;
	case UV_BOOLEAN:
		Init(_BOOL(other.m_value));
		break;
	case UV_CLASS:
	case UV_ARRAY:
	case UV_HASHTABLE:
	case UV_STREAM:
		Init((CUniVar*)&other);		
		break;

	case UV_FUNCTION:
		Init(_FUNC(other.m_value));
		break;
		
	default:
		break;
	}	
}

void CUniVar::operator =(const CUniVar& other)
{	
	if ((m_type == UV_DOUBLE) && (other.m_type == UV_INT))
	{
		Init((double)_INT(other.m_value));
		return;
	}

	if ((m_type == UV_ARB_DOUBLE) && (other.m_type == UV_INT))
	{
		Init(CArbitraryPrecisionFloat((int)_INT(other.m_value), other.ToArbDouble().GetPrecision()));
		return;
	}

	switch (other.m_type)
	{
	case UV_INT:
		Init(_INT(other.m_value));
		break;
	case UV_SYMBOL:
		Init(_CHAR(other.m_value));
		break;
	case UV_STRING:
		Init(_STRING(other.m_value));
		break;
	case UV_DOUBLE:
		Init(_DOUBLE(other.m_value));
		break;
	case UV_ARB_DOUBLE:
		Init(_ARB_DOUBLE(other.m_value));
		break;
	case UV_BOOLEAN:
		Init(_BOOL(other.m_value));
		break;
	case UV_CLASS:
	case UV_ARRAY:
	case UV_HASHTABLE:
	case UV_STREAM:
		Init((CUniVar*)&other);		
		break;
	case UV_FUNCTION:
		Init(_FUNC(other.m_value));
		break;
	case UV_CLEARED:
	case UV_UNKNOWN:
		CleanUp(UV_UNKNOWN);
		m_type = UV_UNKNOWN;
		break;
	default:
		break;
	}	
}

bool CUniVar::operator <(const int var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) < var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) < var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) < var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) < var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) < var;
	}

	throw std::runtime_error("Failed to convert for '<' comparison between variable and integer");	
	return false;
}

bool CUniVar::operator <(const double var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) < var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) < var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) < var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) < var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) < var;
	}

	throw std::runtime_error("Failed to convert for '<' comparison between variable and double");	
	return false;
}


bool CUniVar::operator <(const CUniVar var)
{
	InitByDefaultIfNeeded();
	if (m_type == var.m_type)
	{
		return CompareValue(m_type, m_value, var.m_value);
	}
		
	if (var.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = var;
		tmp.ConvertToTypeOf(this);
		return operator <(tmp);
	}
	else
	{
		throw std::runtime_error("Failed to convert for '>' comparison between variable and " + GetTypeAsString());
		return false;
	}
}

bool CUniVar::operator >(const CUniVar var)
{
	InitByDefaultIfNeeded();
	if (m_type == var.m_type)
	{
		return CompareValue(m_type, var.m_value, m_value);
	}

	if (var.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = var;
		tmp.ConvertToTypeOf(this);
		return operator >(tmp);
	}
	else
	{
		throw std::runtime_error("Failed to convert for '>' comparison  between variable and " + GetTypeAsString());
		return false;
	}
}

bool CUniVar::operator >(const int var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) > var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) > var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) > var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) > var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) > var;
	}
	
	throw std::runtime_error("Failed to convert for '>' comparison between variable and integer");	
	return false;
}

bool CUniVar::operator >(const double var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) > var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) > var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) > var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) > var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) > var;
	}
	
	throw std::runtime_error("Failed to convert for '>' comparison between variable and double");	
	return false;
}

bool CUniVar::operator <=(const int var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) <= var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) <= var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) <= var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) <= var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) <= var;
	}

	throw std::runtime_error("Failed to convert for '<=' comparison between variable and integer");	
	return false;
}

bool CUniVar::operator <=(const double var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) <= var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) <= var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) <= var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) <= var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) <= var;
	}

	throw std::runtime_error("Failed to convert for '<=' comparison between variable and double");	
	return false;
}


bool CUniVar::operator <=(const CUniVar var)
{
	InitByDefaultIfNeeded();
	if (m_type == var.m_type)
	{
		return CompareValueExt(m_type, m_value, var.m_value);
	}
	
	if (var.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = var;
		tmp.ConvertToTypeOf(this);
		return operator <=(tmp);
	}
	else
	{
		throw std::runtime_error("Failed to convert for '>=' comparison between variable and " + GetTypeAsString());
		return false;
	}
}

bool CUniVar::operator >=(const CUniVar var)
{
	InitByDefaultIfNeeded();
	if (m_type == var.m_type)
	{
		return CompareValueExt(m_type, var.m_value, m_value);
	}

	if (var.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = var;
		tmp.ConvertToTypeOf(this);
		return operator >=(tmp);
	}
	else
	{
		throw std::runtime_error("Failed to convert for '>=' comparison between variable and " + GetTypeAsString());
		return false;
	}
}

bool CUniVar::operator >=(const int var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) >= var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) >= var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) >= var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) >= var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) >= var;
	}
	
	throw std::runtime_error("Failed to convert for '>=' comparison between variable and integer");	
	return false;
}

bool CUniVar::operator >=(const double var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) >= var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) >= var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) >= var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) >= var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) >= var;
	}
	
	throw std::runtime_error("Failed to convert for '>=' comparison between variable and double");	
	return false;
}

bool CUniVar::operator!=(const CUniVar var)
{
	return !operator==(var);
}

bool CUniVar::operator!=(const int var)
{
	return !operator==(var);
}

bool CUniVar::operator!=(const double var)
{
	return !operator==(var);
}

bool CUniVar::operator!=(const char var)
{
	return !operator==(var);
}

bool CUniVar::operator!=(const bool var)
{
	return !operator==(var);
}

bool CUniVar::operator==(const CUniVar var)
{
	InitByDefaultIfNeeded();
	if (m_type == var.m_type)
	{
		return IsValueEqual(m_type, m_value, var.m_value);
	}

	if (var.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = var;
		tmp.ConvertToTypeOf(this);
		return IsValueEqual(m_type, m_value, tmp.m_value);
	}		
	
	return false;
}

bool CUniVar::operator==(const char var)
{
	InitByDefaultIfNeeded();
	if (convertMatrix[UV_INT][m_type])
	{
		return operator==((int)var);
	}
	
	return false;
}

bool CUniVar::operator==(const bool var)
{
	InitByDefaultIfNeeded();
	if (convertMatrix[UV_BOOLEAN][m_type])
	{
		return (var == ToBoolean());
	}
	
	return false;	
}

bool CUniVar::operator==(const int var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) == var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) == var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) == var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) == var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) == var;
	}	

	return false;
}

bool CUniVar::operator==(const double var)
{
	InitByDefaultIfNeeded();
	if (m_type == UV_INT)
	{
		return _INT(m_value) == var;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) == var;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) == var;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) == var;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value) == var;
	}	

	return false;
}

CUniVar& CUniVar::operator[](const char* index)
{	
	return operator[]((index == NULL) ? CUniVar(0) : CUniVar(index));
}

CUniVar& CUniVar::operator[](const CUniVar& _index)
{
	if (m_type == UV_UNKNOWN)
	{
		ConvertTypeIfNeeded(UV_HASHTABLE);
	}

	if (m_type == UV_STRING)
	{
		unsigned int index = _index.ToInt();
		unsigned int size = _STRING(m_value).size();
		if (index < size)
		{	
			char* ch = (char*)(_STRING(m_value).c_str() + index);			

			static CUniVar globalSymbolVar[2];
			static int counter = 0;
			counter = (counter + 1) % 2;
			globalSymbolVar[counter].InitWithSymbolPtr(ch);
			return globalSymbolVar[counter];
		}
		else
		{
			throw std::runtime_error("Attempt to use out of range index for access symbol in string");
			
			static CUniVar globalNullVar;
			globalNullVar.SwitchToUnknownState();
			return globalNullVar;
		}
	}
	else if (m_type == UV_ARRAY)
	{
		return _ARRAY(m_value)->GetElement(_index.ToInt());
	}
	else if (m_type == UV_HASHTABLE)
	{
		return _HASHTABLE(m_value)->GetElement(_index);
	}
	else if (m_type == UV_CLASS)
	{
		return ToClass()->GetProperty__(_index);
	}

	throw std::runtime_error("Attempt to use indexed access for variable of simple type");

	static CUniVar globalNullVar;
	globalNullVar.SwitchToUnknownState();
	return globalNullVar;
}

CUniVar::operator const int()const
{
	if (m_type == UV_INT)
	{
		return (int)_INT(m_value);
	}
	else if (m_type == UV_SYMBOL)
	{
		return (int)_CHAR(m_value);
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value);
	}
	else if (m_type == UV_STRING)
	{
		if (IsStringConvertableToInt(_STRING(m_value)))
		{			
			return atoi(_STRING(m_value).c_str());
		}
	}

	throw std::runtime_error("Failed to convert to int");

	return 0;
}

CUniVar::operator const std::string()const
{
	return ToString();
}

CUniVar CUniVar::operator++(int)
{
	CUniVar temp = *this;
	if (m_type == UV_INT)
	{
		_INT(m_value)++;
	}
	else if (m_type == UV_SYMBOL)
	{
		_CHAR(m_value)++;
	}
	else if (m_type == UV_DOUBLE)
	{
		_DOUBLE(m_value)++;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		(_ARB_DOUBLE(m_value))++;
	}
	else if (m_type == UV_UNKNOWN)
	{
		Init((int64_t)1);
	}
	else
	{
		throw std::runtime_error("Failed to increment because of non-incrementable value of variable");
	}
	return temp;
}

CUniVar CUniVar::operator--(int)
{
	CUniVar temp = *this;
	if (m_type == UV_INT)
	{
		_INT(m_value)--;
	}
	else if (m_type == UV_SYMBOL)
	{
		_CHAR(m_value)--;
	}
	else if (m_type == UV_DOUBLE)
	{
		_DOUBLE(m_value)--;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		_ARB_DOUBLE(m_value)--;
	}
	else if (m_type == UV_UNKNOWN)
	{
		Init((int64_t)-1);
	}
	else
	{
		throw std::runtime_error("Failed to decrement because of non-incrementable value of variable");				
	}
	return temp;	
}

unsigned int CUniVar::ToUInt()const
{
	return (unsigned int)ToInt();
}

void* CUniVar::ToPtr()
{
	if (m_type != UV_INT)
	{
		// throw std::runtime_error("Failed to convert to int while return as pointer");
		// we will return NULL instead of exception because it is function for C++ interop only
		return NULL;
	}
#ifdef ENV32BIT
	int* a = (int*)&_INT(m_value);
	return (void*)*a;
#else
	return (void*)_INT(m_value);
#endif
}

int CUniVar::ToInt()const
{
	if (m_type == UV_INT)
	{
		return (int)_INT(m_value);
	}
	else if (m_type == UV_SYMBOL)
	{
		return (int)_CHAR(m_value);
	}
	else if (m_type == UV_DOUBLE)
	{
		return (int)_DOUBLE(m_value);
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value).ToInt();
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value);
	}	
	else if (m_type == UV_STRING)
	{
		if (IsStringConvertableToInt(_STRING(m_value)))
		{
			return atoi(_STRING(m_value).c_str());
		}
	}	

	throw std::runtime_error("Failed to convert to int");
	return 0;
}

int64_t CUniVar::ToInt64()const
{
	if (m_type == UV_INT)
	{
		return _INT(m_value);
	}
	else if (m_type == UV_SYMBOL)
	{
		return (int)_CHAR(m_value);
	}
	else if (m_type == UV_DOUBLE)
	{
		return (int)_DOUBLE(m_value);
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value).ToInt();
	}
	else if (m_type == UV_BOOLEAN)
	{
		return (int)_BOOL(m_value);
	}
	else if (m_type == UV_STRING)
	{
		if (IsStringConvertableToInt(_STRING(m_value)))
		{
			return atoi(_STRING(m_value).c_str());
		}
	}	
	
	throw std::runtime_error("Failed to convert to int");
	return 0;
}

bool CUniVar::ToBoolean()const
{
	if (m_type == UV_INT)
	{
		return _INT(m_value) != 0;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value) != 0;
	}
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value) != 0.0;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value) != 0.0;
	}
	else if (m_type == UV_BOOLEAN)
	{
		return _BOOL(m_value);
	}
	else if (m_type == UV_STRING)
	{
		if (_STRING(m_value) == "true")
		{
			return true;
		}
		else if (_STRING(m_value) == "false")
		{
			return false;
		}
		else if (IsStringConvertableToInt(_STRING(m_value)))
		{
			return atoi(_STRING(m_value).c_str()) > 0;
		}
	}

	throw std::runtime_error("Failed to convert to boolean");
	return 0;
}

char CUniVar::ToSymbol()const
{
	if (m_type == UV_INT)
	{
		return (_INT(m_value)%256 + 256)%256;
	}
	else if (m_type == UV_SYMBOL)
	{
		return _CHAR(m_value);
	}
	else if (m_type == UV_DOUBLE)
	{
		return (((int)_DOUBLE(m_value))%256 + 256)%256;
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return ((_ARB_DOUBLE(m_value).ToInt())%256 + 256)%256;
	}

	throw std::runtime_error("Failed to convert to symbol");
	return 0;
}

double CUniVar::ToDouble()const
{
	if (m_type == UV_INT)
	{
		return (double)_INT(m_value);
	}
	else if (m_type == UV_SYMBOL)
	{
		return (double)_CHAR(m_value);
	}	
	else if (m_type == UV_DOUBLE)
	{
		return _DOUBLE(m_value);
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value).ToDouble();
	}
	else if (m_type == UV_BOOLEAN)
	{
		return _BOOL(m_value);
	}
	else if (m_type == UV_STRING)
	{
		if (IsStringConvertableToFloat(_STRING(m_value)))
		{
			return atof(_STRING(m_value).c_str());
		}
	}	

	throw std::runtime_error("Failed to convert to double");
	return 0.0;
}

CArbitraryPrecisionFloat CUniVar::ToArbDouble()const
{
	if (m_type == UV_INT)
	{
		return CArbitraryPrecisionFloat((int)_INT(m_value), DEFAULT_PRECISION);
	}
	else if (m_type == UV_SYMBOL)
	{
		return CArbitraryPrecisionFloat((int)_CHAR(m_value), DEFAULT_PRECISION);
	}	
	else if (m_type == UV_DOUBLE)
	{
		return CArbitraryPrecisionFloat(_DOUBLE(m_value), DEFAULT_PRECISION);
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value);
	}
	else if (m_type == UV_BOOLEAN)
	{
		return CArbitraryPrecisionFloat((int)_BOOL(m_value), DEFAULT_PRECISION);
	}
	else if (m_type == UV_STRING)
	{
		if (IsStringConvertableToFloat(_STRING(m_value)))
		{
			return CArbitraryPrecisionFloat(_STRING(m_value), DEFAULT_PRECISION);
		}
	}	

	throw std::runtime_error("Failed to convert to double");
	return CArbitraryPrecisionFloat(0.0, DEFAULT_PRECISION);
}

std::string CUniVar::ToString()const
{
	if (m_type == UV_INT)
	{
		char tmp[20];
		sprintf(tmp, "%d", (int)_INT(m_value));
		return tmp;
	}
	else if (m_type == UV_DOUBLE)
	{		
#ifdef WIN32
		if (_isnan(_DOUBLE(m_value)))
#else
		if (std::isnan(_DOUBLE(m_value)))
#endif
		{
			return "nan";
		}
#ifdef WIN32
		else if (!_finite(_DOUBLE(m_value)))
#else
		else if (std::isinf(_DOUBLE(m_value)))
#endif
		{
			return "inf";
		}
		else
		{
			char tmp[50];
			sprintf(tmp, "%f", _DOUBLE(m_value));
			return tmp;
		}
	}
	else if (m_type == UV_ARB_DOUBLE)
	{
		return _ARB_DOUBLE(m_value).ToString();		
	}
	else if (m_type == UV_SYMBOL)
	{
		char tmp[2];
		tmp[0] = _CHAR(m_value);
		tmp[1] = 0;
		return tmp;
	}
	else if (m_type == UV_STRING)
	{
		return _STRING(m_value);
	}
	else if (m_type == UV_BOOLEAN)
	{
		return _BOOL(m_value) ? "true" : "false";
	}
	else if ((m_type == UV_CLASS) || (m_type == UV_ARRAY) || (m_type == UV_HASHTABLE))
	{		
		return ToJson();
	}
	else if (m_type == UV_STREAM)	
	{
		return _STREAM(m_value)->ToString();
	}
	else if (m_type == UV_FUNCTION)	
	{		
		return "function";		
	}
	else if (m_type == UV_UNKNOWN)	
	{
		return "{}";		
	}
	else
	{
		assert(false);
	}

	throw std::runtime_error("Failed to convert to string");
	return "";
}

std::wstring CUniVar::ToWString()const
{
	std::string str = ToString();

	int size = str.size() + 1;

	wchar_t* result = new wchar_t[size];
#ifdef WIN32
	mbstowcs_s((size_t*)&size, result, size, str.c_str(), size);
#else
	mbstowcs(result, str.c_str(), size);
#endif
	std::wstring res = result;
	delete[] result;

	return res;
}

void CUniVar::operator +=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if ((((m_type == UV_INT) && ((other.m_type == UV_DOUBLE) || (other.m_type == UV_ARB_DOUBLE))) ||
		((m_type == UV_DOUBLE) && (other.m_type == UV_ARB_DOUBLE))) && (m_type != other.m_type))
	{
		bool oldPermitChangeType = permitChangeType;
		CUniVar tmp = other;
		tmp.operator +=(*this);
		permitChangeType = oldPermitChangeType;
		if (permitChangeType)
		{	
			*this = tmp;
		}
		else if (m_type == UV_INT)
		{
			*this = tmp.ToInt();
		}
		else
		{
			*this = tmp.ToDouble();
		}
		return;
	}
	
	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);
	
		if (m_type == UV_INT)
		{
			_INT(m_value) += other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) += other.ToSymbol();
		}
		else if (m_type == UV_STRING)
		{
			_STRING(m_value) += other.ToString();
		}
		else if (m_type == UV_DOUBLE)
		{
			_DOUBLE(m_value) += other.ToDouble();
		}
		else if (m_type == UV_ARB_DOUBLE)
		{
			_ARB_DOUBLE(m_value) += other.ToArbDouble();
		}
		else if (m_type == UV_BOOLEAN)
		{
			_BOOL(m_value) |= other.ToBoolean();
		}
		else if (m_type == UV_ARRAY)
		{
			assert(false);
		}
		else
		{
			throw std::runtime_error("Addition is not supported for first operand type " + GetTypeAsString());
		}
	}
	else
	{
		throw std::runtime_error("Addition error. Failed to convert to type " + GetTypeAsString());
	}
}

void CUniVar::operator *=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if ((((m_type == UV_INT) && ((other.m_type == UV_DOUBLE) || (other.m_type == UV_ARB_DOUBLE))) ||
		((m_type == UV_DOUBLE) && (other.m_type == UV_ARB_DOUBLE))) && (m_type != other.m_type))
	{
		bool oldPermitChangeType = permitChangeType;
		CUniVar tmp = other;
		tmp.operator *=(*this);
		permitChangeType = oldPermitChangeType;
		if (permitChangeType)
		{	
			*this = tmp;
		}
		else if (m_type == UV_INT)
		{
			*this = tmp.ToInt();
		}
		else
		{
			*this = tmp.ToDouble();
		}
		return;
	}	

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);

		if (m_type == UV_INT)
		{
			_INT(m_value) *= other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) *= other.ToSymbol();
		}		
		else if (m_type == UV_DOUBLE)
		{
			_DOUBLE(m_value) *= other.ToDouble();
		}
		else if (m_type == UV_ARB_DOUBLE)
		{
			_ARB_DOUBLE(m_value) *= other.ToArbDouble();
		}
		else if (m_type == UV_BOOLEAN)
		{
			_BOOL(m_value) &= other.ToBoolean();
		}
		else
		{
			throw std::runtime_error("Multiplication is not supported for first operand type " + GetTypeAsString());
		}
	}
	else	 
	{
		throw std::runtime_error("Multiplication error. Failed to convert to type " + GetTypeAsString());
	}
}

// concat of strings
void CUniVar::operator %=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	Init(ToString() + other.ToString());
}

void CUniVar::operator /=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if ((((m_type == UV_INT) && ((other.m_type == UV_DOUBLE) || (other.m_type == UV_ARB_DOUBLE))) ||
		((m_type == UV_DOUBLE) && (other.m_type == UV_ARB_DOUBLE))) && (m_type != other.m_type))
	{
		bool oldPermitChangeType = permitChangeType;
		CUniVar tmp;
		if (other.m_type == UV_DOUBLE)
		{
			tmp.Init(ToDouble());
		}
		else if (other.m_type == UV_ARB_DOUBLE)
		{
			tmp.Init(CArbitraryPrecisionFloat(ToDouble(), other.ToArbDouble().GetPrecision()));
		}
		tmp.operator /=(other);
		permitChangeType = oldPermitChangeType;
		if (permitChangeType)
		{
			*this = tmp;
		}		
		else if (m_type == UV_INT)
		{
			*this = tmp.ToInt();
		}
		else
		{
			*this = tmp.ToDouble();
		}
		return;
	}	

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);
		bool divideError = false;
		if (m_type == UV_INT)
		{			
			int a = other.ToInt();
			if (a != 0)
			{
				_INT(m_value) /= a;
			}
			else
			{
				divideError = true;
			}
		}
		else if (m_type == UV_SYMBOL)
		{
			char a = other.ToSymbol();
			if (a != 0)
			{
				_CHAR(m_value) /= a;
			}
			else
			{
				divideError = true;
			}
		}		
		else if (m_type == UV_DOUBLE)
		{
			double a = other.ToDouble();
			if (a != 0)
			{
				_DOUBLE(m_value) /= a;
			}
			else
			{
				divideError = true;
			}
		}
		else if (m_type == UV_ARB_DOUBLE)
		{
			CArbitraryPrecisionFloat a = other.ToArbDouble();
			if (a != 0)
			{
				_ARB_DOUBLE(m_value) /= a;
			}
			else
			{
				divideError = true;
			}
		}
		else if (m_type == UV_BOOLEAN)
		{
			bool a = other.ToBoolean();
			if (a != false)
			{
				_BOOL(m_value) &= a;
			}
			else
			{
				divideError = true;
			}
		}
		else
		{
			throw std::runtime_error("Division is not supported for first operand type " + GetTypeAsString());
		}

		if (divideError)
		{
			throw std::runtime_error("Attempt to divide by zero");
		}
	}
	else	
	{
		throw std::runtime_error("Division error. Failed to convert to type " + GetTypeAsString());
	}
}

void CUniVar::operator |=(const CUniVar& other)
{	
	ConvertTypeIfNeeded(other);

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);

		if (m_type == UV_INT)
		{
			_INT(m_value) |= other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) |= other.ToSymbol();
		}		
		else if (m_type == UV_BOOLEAN)
		{
			_BOOL(m_value) |= other.ToBoolean();
		}
		else
		{
			throw std::runtime_error("Logical OR is not supported for first operand type " + GetTypeAsString());
		}
	}
	else	
	{
		throw std::runtime_error("Logical OR error. Failed to convert to type " + GetTypeAsString());
	}
}

void CUniVar::operator ^=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);

		if (m_type == UV_INT)
		{
			_INT(m_value) ^= other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) ^= other.ToSymbol();
		}		
		else if (m_type == UV_BOOLEAN)
		{
			_BOOL(m_value) ^= other.ToBoolean();
		}
		else
		{
			throw std::runtime_error("Logical XOR is not supported for first operand type " + GetTypeAsString());
		}
	}
	else	
	{
		throw std::runtime_error("Logical XOR error. Failed to convert to type " + GetTypeAsString());
	}
}

void CUniVar::operator &=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);

		if (m_type == UV_INT)
		{
			_INT(m_value) &= other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) &= other.ToSymbol();
		}		
		else if (m_type == UV_BOOLEAN)
		{
			_BOOL(m_value) &= other.ToBoolean();
		}
		else
		{
			throw std::runtime_error("Logical AND is not supported for first operand type " + GetTypeAsString());
		}
	}
	else	
	{
		throw std::runtime_error("Logical AND error. Failed to convert to type " + GetTypeAsString());
	}
}

CUniVar CUniVar::operator-() 
{
	CUniVar newVar = *this;
	newVar.operator *=(-1.0);	
	return newVar;
}

CUniVar CUniVar::operator+() 
{	
	return *this;
}

void CUniVar::operator -=(const CUniVar& other)
{
	ConvertTypeIfNeeded(other);

	if ((((m_type == UV_INT) && ((other.m_type == UV_DOUBLE) || (other.m_type == UV_ARB_DOUBLE))) ||
		((m_type == UV_DOUBLE) && (other.m_type == UV_ARB_DOUBLE))) && (m_type != other.m_type))
	{
		bool oldPermitChangeType = permitChangeType;
		CUniVar tmp = other;
		tmp.operator -=(*this);
		tmp.operator *=(-1.0);
		permitChangeType = oldPermitChangeType;
		if (permitChangeType)
		{	
			*this = tmp;
		}
		else if (m_type == UV_INT)
		{
			*this = tmp.ToInt();
		}
		else
		{
			*this = tmp.ToDouble();
		}
		return;
	}

	if ((m_type == UV_INT) && (other.m_type == UV_ARB_DOUBLE))
	{
		bool oldPermitChangeType = permitChangeType;
		CUniVar tmp = other;
		tmp.operator -=(*this);
		tmp.operator *=(-1.0);
		permitChangeType = oldPermitChangeType;
		if (permitChangeType)
		{	
			*this = tmp;
		}
		else
		{
			*this = tmp.ToArbDouble().ToInt();
		}
		return;
	}

	if (other.IsConvertableToTypeOf(this))
	{
		CUniVar tmp = other;
		tmp.ConvertToTypeOf(this);

		if (m_type == UV_INT)
		{
			_INT(m_value) -= other.ToInt();
		}
		else if (m_type == UV_SYMBOL)
		{
			_CHAR(m_value) -= other.ToSymbol();
		}		
		else if (m_type == UV_DOUBLE)
		{
			_DOUBLE(m_value) -= other.ToDouble();
		}
		else if (m_type == UV_ARB_DOUBLE)
		{
			_ARB_DOUBLE(m_value) -= other.ToArbDouble();
		}
		else if (m_type == UV_BOOLEAN)
		{
			if (_BOOL(other.m_value))
			{
				_BOOL(m_value) = false;
			}
		}
		else
		{
			throw std::runtime_error("Substraction is not supported for first operand type " + GetTypeAsString());
		}
	}
	else	
	{
		throw std::runtime_error("Substraction error. Failed to convert to type " + GetTypeAsString());
	}
}

CUniVar CUniVar::operator +(const CUniVar& other)
{
	CUniVar res = *this;
	PermitChangeType();
	res += other;
	ProhibitChangeType();
	return res;
}

CUniVar CUniVar::operator *(const CUniVar& other)
{
	CUniVar res = *this;
	PermitChangeType();
	res *= other;
	ProhibitChangeType();
	return res;
}

CUniVar CUniVar::operator /(const CUniVar& other)
{
	CUniVar res = *this;
	PermitChangeType();
	res /= other;
	ProhibitChangeType();
	return res;
}

CUniVar CUniVar::operator -(const CUniVar& other)
{
	CUniVar res = *this;
	PermitChangeType();
	res -= other;
	ProhibitChangeType();
	return res;
}

CUniVar CUniVar::operator |(const CUniVar& other)
{
	CUniVar res = *this;
	res |= other;
	return res;
}

CUniVar CUniVar::operator ^(const CUniVar& other)
{
	CUniVar res = *this;
	res ^= other;
	return res;
}

CUniVar CUniVar::operator &(const CUniVar& other)
{
	CUniVar res = *this;
	res &= other;
	return res;
}

bool CUniVar::IsObjectMethod(ClassMethodEnum method)
{
	if ((method == __FUNC_ENUM__clone) || (method == __FUNC_ENUM__convertable))	
	{	
		return true;
	}

	return false;
}

CUniVar CUniVar::CallMethod__(int count, ...)
{
	va_list vl; 
	va_start(vl, count); 

	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) > 0); 
	int callStatus = va_arg(vl, int);  

	if (m_type == UV_UNKNOWN)
	{
		bool isObject = IsObjectMethod(method);
		bool isArray = CArrayDataContainer::IsMethodExist(method);
		bool isHashtable = CHashtableDataContainer::IsMethodExist(method);		
		if (isObject)
		{
			ConvertTypeIfNeeded(UV_CLASS);
		}
		else if (isArray)
		{
			ConvertTypeIfNeeded(UV_ARRAY);
		}
		else if (isHashtable)
		{
			ConvertTypeIfNeeded(UV_HASHTABLE);
		}
	}

	if ((m_type == UV_CLASS) || (m_type == UV_ARRAY) || (m_type == UV_HASHTABLE))
	{
		CUniVar result = ToClass()->CallMethod__(count, vl, method, callStatus, defaultResultNeeded);
		va_end(vl);
		return result;
	}
	else
	{
		CUniVar result = getResultObject(false);		
		
		if ((method == __FUNC_ENUM__push) && (count == 4)) 
		{
			if (m_type != UV_ARRAY)
			{
				CUniVar* arg0 = va_arg(vl, CUniVar*); 
				CArrayDataContainer* adt = new CArrayDataContainer();
				adt->Push(arg0);
				Init(adt);
			}
		}
		else if ((method == __FUNC_ENUM__convertable) && (count == 4)) 
		{
			CUniVar* arg0 = va_arg(vl, CUniVar*); 
			result = IsConvertableToType(arg0->ToString());
		}		
		else
		{
			Log.Warnings.AddLine3s("Unknown method %s call after line %s in %s", methodNames[method], currentLine, getCurrentMethodName());
		}
		va_end(vl);	 

		if (defaultResultNeeded) 
		{ 
			result.PermitChangeType();
			result = result.GetProperty__(1, __PROP_ENUM__result); 
			result.ProhibitChangeType();
		}

		return result;
	}

	return false;
}

CUniVar* CUniVar::GetPtr()
{
	return this;
}

void CUniVar::UpdateType()
{
	permitChangeType = true;
	std::string a = ToString();
	if ((a == "true") || (a == "false"))
	{
		Init((a == "true"));
	}
	else 
	{
		if (IsStringConvertableToInt(a))
		{
			int n = atoi(a.c_str());
			Init((int64_t)n);			
		}
		else if (IsStringConvertableToFloat(a))
		{
			double d = atof(a.c_str());
			Init(d);
		}
		else if (a.size() == 1)
		{
			Init(a[0]);
		}
		else
		{
			Init(a);
		}		
	}
	permitChangeType = false;
}

void CUniVar::CleanUp(const UniVarType newType)
{
	if (((m_type != newType) && (m_type != UV_UNKNOWN) && (newType != UV_CLEARED)) && (!permitChangeType))
	{
		std::string errorTxt = "change type of variable (from ";
		errorTxt += GetTypeAsString();
		errorTxt += " to ";
		errorTxt += typeNames[(int)newType];
		errorTxt += ") is prohibited";
		throw std::runtime_error(errorTxt.c_str());
		return;
	}

	if (!m_valueUsedByPtr)
	if (m_value)
	{
		switch (m_type)
		{
		case UV_INT:
			Free<int64_t>(m_value);
			break;
		case UV_SYMBOL:
			Free<char>(m_value);
			break;
		case UV_STRING:
			Free<std::string>(m_value);
			break;
		case UV_DOUBLE:
			Free<double>(m_value);
			break;
		case UV_ARB_DOUBLE:
			Free<CArbitraryPrecisionFloat>(m_value);
			break;
		case UV_BOOLEAN:
			Free<bool>(m_value);
			break;
		case UV_CLASS:
			Free<std::shared_ptr<CClassDataContainer>>(m_value);
			break;
		case UV_ARRAY:
			Free<std::shared_ptr<CArrayDataContainer>>(m_value);
			break;
		case UV_HASHTABLE:
			Free<std::shared_ptr<CHashtableDataContainer>>(m_value);
			break;
		case UV_STREAM:
			Free<std::shared_ptr<CStreamDataContainer>>(m_value);
			break;
		case UV_FUNCTION:
			Free<FunctionPointer>(m_value);
			break;
		default:
			break;
		}		
	}
	m_value = NULL;
	m_valueUsedByPtr = false;
}

CUniVar& CUniVar::GetProperty__(const int callStatus, const ClassPropertyEnum propId)
{
	if (m_type == UV_UNKNOWN)
	{
		ConvertTypeIfNeeded(UV_CLASS);
	}

	if ((m_type == UV_CLASS) || (m_type == UV_ARRAY) || (m_type == UV_HASHTABLE) || (m_type == UV_STREAM))
	{
		return ToClass()->GetProperty__(callStatus, propId);
	}
	else if ((m_type == UV_STRING) && (propId == __PROP_ENUM__length))
	{
		static CUniVar lenVar;
		lenVar = _STRING(m_value).size();
		return lenVar;
	}
	else
	{
		static CUniVar globalNullVar;
		globalNullVar.SwitchToUnknownState();
		return globalNullVar;
	}
}

CUniVar& CUniVar::GetProperty__(const std::string& name)
{
	if ((m_type == UV_CLASS) || (m_type == UV_ARRAY) || (m_type == UV_HASHTABLE) || (m_type == UV_STREAM))
	{
		return ToClass()->GetProperty__(name);
	}
	else
	{
		static CUniVar globalNullVar;
		globalNullVar.SwitchToUnknownState();
		return globalNullVar;
	}
}

CUniVar* CUniVar::operator ->()
{
	return this;
}

bool CUniVar::IsJSONObject(const std::string& str)
{
	int pos = str.size() - 1;
	while ((pos > 0) && ((str[pos] == 10) || (str[pos] == 13)))
	{
		pos--;
	}
	return (str.size() >= 2) && ((str[0] == '{') && (str[pos] == '}'));
}

bool CUniVar::IsJSONArray(const std::string& str)
{
	int pos = str.size() - 1;
	while ((pos > 0) && ((str[pos] == 10) || (str[pos] == 13)))
	{
		pos--;
	}
	return (str.size() >= 2) && ((str[0] == '[') && (str[pos] == ']'));
}

void CUniVar::PermitChangeType()
{
	permitChangeType = true;
}

void CUniVar::ProhibitChangeType()
{
	permitChangeType = false;
}

bool CUniVar::UpdateStateFromJson(const std::string& json, bool needReplaceAll)
{	
	permitChangeType = true;
	bool isArray = IsJSONArray(json);

	if (needReplaceAll)
	{		
		CClassDataContainer* cdt = isArray ? new CArrayDataContainer() : new CClassDataContainer();
		if (cdt->UpdateStateFromJson(json))
		{
			SwitchToUnknownState();
			if (isArray)
			{
				CArrayDataContainer * adt = (CArrayDataContainer *)cdt;
				Init(adt);
			}
			else
			{
				Init(cdt);
			}
			permitChangeType = false;
			return true;
		}
		else
		{
			delete cdt;
			permitChangeType = false;
			return false;
		}		
	}

	if (m_type == UV_UNKNOWN)
	{
		Init(isArray ? new CArrayDataContainer() : new CClassDataContainer());
	}

	if (m_type == UV_CLASS)
	{
		permitChangeType = false;
		return ToClass()->UpdateStateFromJson(json);
	}
	else if (m_type == UV_ARRAY)
	{
		permitChangeType = false;
		return _ARRAY(m_value)->UpdateStateFromJson(json);
	}
	else if (m_type == UV_HASHTABLE)
	{
		permitChangeType = false;
		return _HASHTABLE(m_value)->UpdateStateFromJson(json);
	}

	permitChangeType = false;
	return false;
}

bool CUniVar::UpdateStateFromXml(void* rootElement, bool needReplaceAll)
{
	if (needReplaceAll)
	{		
		CClassDataContainer* cdt = new CClassDataContainer();
		if (cdt->UpdateStateFromXml(rootElement))
		{
			SwitchToUnknownState();
			Init(cdt);			
			return true;
		}
		else
		{
			delete cdt;
			return false;
		}		
	}

	if (m_type == UV_UNKNOWN)
	{
		Init(new CClassDataContainer());
	}

	if (m_type == UV_CLASS)
	{
		return ToClass()->UpdateStateFromXml(rootElement);
	}	

	return false;
}

std::string CUniVar::ToJson()const
{
	if (m_type == UV_CLASS)
	{
		return ToConstClass()->ToJson();
	}
	else if (m_type == UV_ARRAY)
	{
		return _ARRAY(m_value)->ToJson();
	}
	else if (m_type == UV_HASHTABLE)
	{
		return _HASHTABLE(m_value)->ToJson();
	}
	
	return "";
}

std::string CUniVar::ToXml()const
{
	if (m_type == UV_CLASS)
	{
		return ToConstClass()->ToXml();
	}
	else if (m_type == UV_HASHTABLE)
	{
		return _HASHTABLE(m_value)->ToXml();
	}
	
	return "";
}

class SimpleCounter
{
public:
	SimpleCounter(int* a)
	{
		m_a = a;
		(*m_a)++;
	}

	~SimpleCounter()
	{
		(*m_a)--;
	}

private:
	int* m_a;
};

Json::Value CUniVar::ToJsonValue()const
{
	static int jsonLevel = 0;
	SimpleCounter cnt(&jsonLevel);

	if (jsonLevel < JSON_LEVEL_LIMIT)
	{
		switch (m_type)
		{
		case UV_INT:
			return Json::Value(_INT(m_value));		
		case UV_SYMBOL:
			return Json::Value(_CHAR(m_value));		
		case UV_STRING:
			return Json::Value(_STRING(m_value));				
		case UV_DOUBLE:
			return Json::Value(_DOUBLE(m_value));				
		case UV_ARB_DOUBLE:
			assert(false);
			return Json::Value(_DOUBLE(m_value));				
		case UV_BOOLEAN:
			return Json::Value(_BOOL(m_value));		
		case UV_CLASS:
			return ToConstClass()->ToJsonValue();		
		case UV_ARRAY:
			return _ARRAY(m_value)->ToJsonValue();		
		case UV_HASHTABLE:
			return _HASHTABLE(m_value)->ToJsonValue();	
		case UV_STREAM:
		case UV_FUNCTION:
		case UV_UNKNOWN:
		case UV_CLEARED:
			return "";	
		default:
			break;
		}
	}	
	else
	{
		throw std::runtime_error("Limit of json tree level is reached. Probably you have circle references in your object");		
	}

	Json::Value root(0);	
	return root;
}

UniVarType CUniVar::GetType()const
{
	return m_type;
}

void CUniVar::SwitchToUnknownState()
{
	CleanUp(UV_CLEARED);
	m_type = UV_UNKNOWN;
	m_value = NULL;
}

int GetTypeByName(const std::string& name)
{
	static bool isInited = false;
	static std::map<std::string, int> typeMap;

	if (!isInited)
	{
		isInited = true;
		
		for (int t = UV_INT; t < TYPES_COUNT; t++)
		{
			typeMap.insert(std::map<std::string, int>::value_type(typeNames[t], t));
		}		
	}

	std::map<std::string, int>::const_iterator it = typeMap.find(name);
	if (it == typeMap.end())
	{
		return UV_UNKNOWN;
	}

	return (*it).second;
}

CUniVar CUniVar::IsConvertableTypeToType(const std::string& srcTypeName, const std::string& destTypeName)
{
	int sourceType = GetTypeByName(srcTypeName);
	int destType = GetTypeByName(destTypeName);
	
	bool res = convertMatrix[destType][sourceType];

	return getFullResultObject(res);
}

bool CUniVar::IsConvertableToType(const int destType)const
{
	int sourceType = GetType();
	return convertMatrix[destType][sourceType];	
}

void CUniVar::CorrectConvertableForSpecialCases(const int sourceType, const int destType, bool& res)const
{
	if (sourceType == UV_STRING)
	{
		if (destType == UV_INT)
		{
			if (IsStringConvertableToInt(_STRING(m_value)))
			{
				res = true;
			}
		}
		else if ((destType == UV_DOUBLE) || (destType == UV_ARB_DOUBLE))
		{
			if ((IsStringConvertableToInt(_STRING(m_value))) ||
				(IsStringConvertableToFloat(_STRING(m_value))))
			{
				res = true;
			}			
		}
		else if (destType == UV_BOOLEAN)
		{
			if ((_STRING(m_value) == "true") || (_STRING(m_value) == "false"))
			{
				res = true;
			}

			if (IsStringConvertableToInt(_STRING(m_value)))
			{
				res = true;
			}
		}
	}
}

CUniVar CUniVar::IsConvertableToType(const std::string& typeName)const
{
	int destType = GetTypeByName(typeName);
	int sourceType = GetType();
	bool res = convertMatrix[destType][sourceType];

	CorrectConvertableForSpecialCases(sourceType, destType, res);

	return getFullResultObject(res);
}

bool CUniVar::IsConvertableToTypeOf(const CUniVar* const other)const
{
	int sourceType = GetType();
	int destType = other->GetType();
	bool res = convertMatrix[destType][sourceType];

	CorrectConvertableForSpecialCases(sourceType, destType, res);

	return res;
}

bool CUniVar::ConvertToType(const int destType)
{
	int sourceType = GetType();	
	if (sourceType == destType)
	{
		return true;
	}

	bool isSpecialCase = ((sourceType == UV_STRING) && ((destType == UV_INT) || (destType == UV_DOUBLE) || (destType == UV_ARB_DOUBLE) || (destType == UV_BOOLEAN)));
	assert(convertMatrix[destType][sourceType] || isSpecialCase);
	if ((!convertMatrix[destType][sourceType]) && (!isSpecialCase))
	{
		return false;
	}

	permitChangeType = true;

	switch (destType)
	{
	case UV_INT:
		Init(ToInt64());
		break;

	case UV_STRING:
		Init(ToString());
		break;

	case UV_SYMBOL:
		Init(ToSymbol());
		break;

	case UV_DOUBLE:
		Init(ToDouble());
		break;

	case UV_ARB_DOUBLE:
		Init(ToArbDouble());
		break;

	case UV_BOOLEAN:
		Init(ToBoolean());
		break;
	
	default:
		assert(false);
	}

	permitChangeType = false;

	return true;
}

bool CUniVar::ConvertToTypeOf(const CUniVar* const other)
{
	int destType = other->GetType();
	return ConvertToType(destType);	
}

std::string CUniVar::GetTypeAsString()const
{
	return typeNames[(int)GetType()];
}

std::string CUniVar::GetHashAsString()const
{
	std::string hash = GetTypeAsString();
	hash += "::";

	switch (GetType())
	{
	case UV_INT:
	case UV_DOUBLE:
	case UV_ARB_DOUBLE:
	case UV_SYMBOL:
	case UV_STRING:
	case UV_BOOLEAN:
		hash += ToString();
		break;

	case UV_CLASS:
	case UV_ARRAY:
	case UV_HASHTABLE:
	case UV_STREAM:
		hash += ToConstClass()->GetUniqueID();
		break;

	case UV_CLEARED:
	case UV_UNKNOWN:
		hash += "{}";
		break;

	case UV_FUNCTION:
		hash += _FUNC(m_value).GetHash();
		break;

	default:
		assert(false);
	}	

	return hash;
}

CStreamDataContainer* CUniVar::ToStream()
{
	if (GetType() != UV_STREAM)
	{
		throw std::runtime_error("Failed to convert to stream");
		return NULL;
	}

	return _STREAM(m_value).get();
}

CHashtableDataContainer* CUniVar::ToHashtable()
{
	if (GetType() != UV_HASHTABLE)
	{
		throw std::runtime_error("Failed to convert to hashtable");
		return NULL;
	}

	return _HASHTABLE(m_value).get();
}

CClassDataContainer* CUniVar::ToClass()
{
	return (CClassDataContainer*)ToConstClass();
}

CClassDataContainer* CUniVar::ToConstClass()const
{
	if ((m_type != UV_CLASS) && (m_type != UV_ARRAY) && (m_type != UV_HASHTABLE) && (m_type != UV_STREAM))
	{
		throw std::runtime_error("Failed to convert to class instance");
		return NULL;
	}

	if (!m_valueUsedByPtr)
	{
		return _CLASS(m_value).get();
	}

	return (CClassDataContainer*)m_value;
}

CArrayDataContainer* CUniVar::ToArray()
{
	if (GetType() != UV_ARRAY)
	{
		throw std::runtime_error("Failed to convert to array");
		return NULL;
	}

	return _ARRAY(m_value).get();
}

unsigned int CUniVar::GetLength()const
{
	if (m_type == UV_ARRAY)
	{
		return _ARRAY(m_value)->GetLength();
	}
	else if (m_type == UV_HASHTABLE)
	{
		return _HASHTABLE(m_value)->GetLength();
	}
	else if (m_type == UV_STRING)
	{
		return  _STRING(m_value).size();
	}

	throw std::runtime_error("Attempt to get length of single element variable");
	return 0;
}

bool CUniVar::IsSet(CUniVar _index)const
{
	if ((m_type == UV_ARRAY) || (m_type == UV_STRING))
	{
		unsigned int index = _index.ToUInt();
		return (index < GetLength());
	}
	else if (m_type == UV_HASHTABLE)
	{		
		return _HASHTABLE(m_value).get()->KeyExists(_index);
	}
	else if (m_type == UV_CLASS)
	{
		return ToConstClass()->IsPropertyExist(_index);
	}

	throw std::runtime_error("Attempt to check element by index existance for non-indexed variable");
	
	return false;
}

bool CUniVar::IsMethodExist(CUniVar name)const
{
	if (m_type == UV_CLASS)
	{
		return ToConstClass()->IsMethodExist(name);
	}

	throw std::runtime_error("Attempt to check property existance for non-class and non-hashtable variable");
	return false;
}

bool CUniVar::IsPropertyExist(CUniVar name)const
{
	if ((m_type == UV_HASHTABLE) || (m_type == UV_CLASS))
	{		
		return IsSet(name);
	}	

	throw std::runtime_error("Attempt to check property existance for non-class and non-hashtable variable");
	return false;
}

CVectorConvertor CUniVar::ToVector()
{
	if (GetType() == UV_ARRAY)
	{
		return CVectorConvertor(ToArray());
	}

	throw std::runtime_error("Attempt to convert non-array to vector");
	return CVectorConvertor(NULL);
}

CUniVar& CUniVar::GetProperty(const std::string& name)
{
	return GetProperty__(name);
}

void CUniVar::Remove(const CUniVar index)
{
	if ((m_type == UV_HASHTABLE) || (m_type == UV_CLASS) || (m_type == UV_ARRAY))
	{
		ToClass()->Remove(index);
	}
	else
	{
		throw std::runtime_error("Attempt to remove element by index of non-indexed variable");
	}
}

void CUniVar::Push(const CUniVar elem)
{
	if (m_type == UV_ARRAY)
	{
		_ARRAY(m_value).get()->Push(elem);
	}
	else
	{
		throw std::runtime_error("Attempt to push element to non-array");
	}
}

bool CUniVar::Call(const std::string& methodName)
{
	if (m_type == UV_CLASS)
	{
		return ToClass()->Call(methodName);
	}
	else
	{
		throw std::runtime_error("Attempt to call method of non-class instance");
	}

	return false;
}

FunctionPointer CUniVar::ToFunctionPointer()const
{
	if (m_type != UV_FUNCTION)
	{
		throw std::runtime_error("Failed to convert to function pointer");
	}

	return _FUNC(m_value);
}

void CUniVar::ConvertTypeIfNeeded(const CUniVar& other)
{		
	if (m_type == UV_UNKNOWN)
	{
		ConvertTypeIfNeeded(other.GetType());
	}
}

void CUniVar::ConvertTypeIfNeeded(const UniVarType type)
{
	switch (type)
	{
	case UV_INT:
		Init((int64_t)0);
		break;
	case UV_SYMBOL:
		Init((char)0);
		break;
	case UV_STRING:
		Init(std::string(""));
		break;
	case UV_DOUBLE:
		Init(0.0f);
		break;
	case UV_ARB_DOUBLE:
		Init(CArbitraryPrecisionFloat(0.0, DEFAULT_PRECISION));
		break;
	case UV_BOOLEAN:
		Init(false);
		break;
	case UV_CLASS:
		Init(new CClassDataContainer());
		break;
	case UV_ARRAY:
		Init(new CArrayDataContainer());
		break;
	case UV_HASHTABLE:
		Init(new CHashtableDataContainer());
		break;	
	default:
		assert(false);
		break;
	}
}

CUniVar& CUniVar::InitByDefaultIfNeeded()
{
	if (m_type == UV_UNKNOWN)
	{
		Init(new CClassDataContainer());
	}
	return *this;
}

CUniVar::~CUniVar()
{
	CleanUp(UV_CLEARED);
}
