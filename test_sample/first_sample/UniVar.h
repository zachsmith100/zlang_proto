
#pragma once

#include <string>
#include "SymbolEnums.h"
#include "json.h"
#include <regex>
#include "ArbitraryPrecisionFloat.h"
#include "CommonConfig.h"

#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

#ifndef WIN32	
	#define zrn std
#else
	#define zrn std::tr1	
	#define int64_t __int64
#endif

class CClassDataContainer;
class CArrayDataContainer;
class CHashtableDataContainer;
class CStreamDataContainer;
class CVectorConvertor;

enum UniVarType
{
	UV_CLEARED = -1,
	UV_UNKNOWN = 0,
	UV_INT,
	UV_SYMBOL,
	UV_STRING,
	UV_DOUBLE,
	UV_BOOLEAN,
	UV_CLASS,
	UV_ARRAY,
	UV_HASHTABLE,
	UV_STREAM,
	UV_FUNCTION,
	UV_ARB_DOUBLE,
	TYPES_COUNT
};

struct FunctionPointer
{
	std::string className;
	std::string name;
	int64_t instancePtr;
	int funcEnum;
	
	FunctionPointer(const std::string& name_)
	{
		name = name_;
		instancePtr = 0;
		funcEnum = 0;
	}

	FunctionPointer(const std::string& className_, const int64_t instancePtr_, const int funcEnum_)
	{		
		className = className_;
		instancePtr = instancePtr_;
		funcEnum = funcEnum_;
	}

	FunctionPointer(const FunctionPointer& other)
	{
		name = other.name;
		className = other.className;
		instancePtr = other.instancePtr;
		funcEnum = other.funcEnum;
	}

	std::string GetHash();	

	bool operator == (const FunctionPointer& other)
	{
		return ((name == other.name) || (className == other.className) ||
			(instancePtr == other.instancePtr) || (funcEnum == other.funcEnum));
	}
};

class CUniVar
{
public:
	CUniVar();
	CUniVar(const long int intVar);
	CUniVar(const int intVar);	
	CUniVar(const unsigned int intVar);
	CUniVar(const unsigned long intVar);	
	CUniVar(const char charVar);
	CUniVar(const std::string& strVar);
	CUniVar(const std::wstring& strVar);
	CUniVar(const wchar_t* strVar);
	CUniVar(const char* strVar);
	CUniVar(const CUniVar& other);
	CUniVar(const double doubleVar);
	CUniVar(const bool boolVar);	
	CUniVar(const int64_t intVar);	
	CUniVar(const void* ptrVar);	
	CUniVar(CUniVar* instance);
	CUniVar(CClassDataContainer* instance, const bool notControlInstance = false);
	CUniVar(CArrayDataContainer* instance);
	CUniVar(CHashtableDataContainer* instance);
	CUniVar(CStreamDataContainer* instance);
	CUniVar(const FunctionPointer& functionPtr);
	CUniVar(const CArbitraryPrecisionFloat& doubleVar);
	~CUniVar();	

	unsigned int GetLength()const;

	CUniVar* GetPtr();

	CUniVar* operator ->();

	bool operator <(const CUniVar var);
	bool operator <(const int var);	
	bool operator <(const double var);
	bool operator >(const CUniVar var);
	bool operator >(const int var);	
	bool operator >(const double var);

	bool operator <=(const CUniVar var);
	bool operator <=(const int var);	
	bool operator <=(const double var);
	bool operator >=(const CUniVar var);
	bool operator >=(const int var);	
	bool operator >=(const double var);

	bool operator==(const CUniVar var);
	bool operator==(const int var);
	bool operator==(const double var);
	bool operator==(const char var);
	bool operator==(const bool var);

	bool operator!=(const CUniVar var);
	bool operator!=(const int var);
	bool operator!=(const double var);
	bool operator!=(const char var);
	bool operator!=(const bool var);

	CUniVar operator++(int);
	CUniVar operator--(int);
	
	void operator =(const CUniVar& other);
	void operator +=(const CUniVar& other);
	void operator *=(const CUniVar& other);
	void operator /=(const CUniVar& other);
	void operator -=(const CUniVar& other);
	void operator |=(const CUniVar& other);
	void operator ^=(const CUniVar& other);
	void operator &=(const CUniVar& other);

	CUniVar operator +(const CUniVar& other);
	CUniVar operator *(const CUniVar& other);
	CUniVar operator /(const CUniVar& other);
	CUniVar operator -(const CUniVar& other);
	CUniVar operator |(const CUniVar& other);
	CUniVar operator ^(const CUniVar& other);
	CUniVar operator &(const CUniVar& other);

	CUniVar operator-();
	CUniVar operator+();
	
	CUniVar& operator[](const CUniVar& index);
	CUniVar& operator[](const char* index);

	std::string ToString()const;
	std::wstring ToWString()const;	
	int ToInt()const;
	void* ToPtr();
	int64_t ToInt64()const;
	unsigned int ToUInt()const;
	double ToDouble()const;
	CArbitraryPrecisionFloat ToArbDouble()const;
	bool ToBoolean()const;
	char ToSymbol()const;
	CVectorConvertor ToVector();
	FunctionPointer ToFunctionPointer()const;
	
	operator const int()const;
	operator const std::string()const;
	void operator %=(const CUniVar& other);

	virtual CUniVar CallMethod__(int count, ...);
	
	void UpdateType();

	CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	CUniVar& GetProperty__(const std::string& name);		

	bool UpdateStateFromJson(const std::string& json, bool needReplaceAll = false);
	Json::Value ToJsonValue()const;	

	UniVarType GetType()const;	
	std::string GetTypeAsString()const;

	static CUniVar IsConvertableTypeToType(const std::string& srcTypeName, const std::string& destTypeName);

	bool IsConvertableToTypeOf(const CUniVar* const other)const;
	bool IsConvertableToType(const int destType)const;
	CUniVar IsConvertableToType(const std::string& typeName)const;	

	bool ConvertToTypeOf(const CUniVar* const other);
	bool ConvertToType(const int destType);

	static bool IsJSONObject(const std::string& str);
	static bool IsJSONArray(const std::string& str);

	std::string GetHashAsString()const;	

	CStreamDataContainer* ToStream();
	CHashtableDataContainer* ToHashtable();
	CClassDataContainer* ToClass();
	CClassDataContainer* ToConstClass()const;
	CArrayDataContainer* ToArray();

	bool UpdateStateFromXml(void* rootElement, bool needReplaceAll = false);

	std::string ToXml()const;

	bool IsSet(CUniVar index)const;
	bool IsMethodExist(CUniVar name)const;
	bool IsPropertyExist(CUniVar name)const;

	CUniVar& GetProperty(const std::string& name);
	void Remove(const CUniVar index);
	void Push(const CUniVar elem);

	bool Call(const std::string& methodName);

	void SwitchToUnknownState();

	static void PermitChangeType();
	static void ProhibitChangeType();

	CUniVar& InitByDefaultIfNeeded();

private:	
	void Init(const bool boolVar);
	void Init(const double doubleVar);
	void Init(const int64_t intVar);
	void Init(const char charVar);
	void Init(const std::string strVar);
	void Init(CUniVar* instance);
	void Init(const CArbitraryPrecisionFloat& doubleVar);
	void Init(CClassDataContainer* instance, const bool notControlInstance = false);
	void Init(CArrayDataContainer* instance);
	void Init(CHashtableDataContainer* instance);
	void Init(CStreamDataContainer* instance);
	void Init(const FunctionPointer& instance);

	void InitWithSymbolPtr(char* charVar);
			
	void CleanUp(const UniVarType newType);

	std::string ToJson()const;	

	bool SetupRegexOptions(CUniVar& options, zrn::regex::flag_type& flags);
	std::string GetRegexErrorAsString(const std::regex_error& e);

	std::string ConvertToString(const std::wstring& strVar);

	void CorrectConvertableForSpecialCases(const int sourceType, const int destType, bool& res)const;

	void ConvertTypeIfNeeded(const CUniVar& other);
	void ConvertTypeIfNeeded(const UniVarType type);
	
	static bool IsObjectMethod(ClassMethodEnum method);

	UniVarType m_type;
	void* m_value;
	bool m_valueUsedByPtr;
};