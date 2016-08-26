
#pragma once

#include <string>
#include <map>

#define PROHIBITED_FOR_FUNCTIONS		2
#define PROHIBITED_FOR_CLASSES			4
#define PROHIBITED_FOR_FUNCTION_PARAMS	8
#define PROHIBITED_FOR_PROPERTIES		16
#define PROHIBITED_FOR_VARIABLES		32
#define PROHIBITED_FOR_ALL				(32 + 16 + 8 + 4 + 2)

class CReservedKeywords
{
public:
	static CReservedKeywords* GetInstance();
	
	bool CanBeSymbolUsedForFunctionName(const std::string& name)const;
	bool CanBeSymbolUsedForFunctionParameter(const std::string& name)const;
	bool CanBeSymbolUsedForClassName(const std::string& name)const;
	bool CanBeSymbolUsedForClassProperty(const std::string& name)const;
	bool CanBeSymbolUsedForVariable(const std::string& name)const;

private:
	CReservedKeywords();
	CReservedKeywords(const CReservedKeywords& other);
	void operator =(const CReservedKeywords& other);
	~CReservedKeywords();

	void AddKeyword(const std::string& name, const unsigned int flags);
	bool CheckPermissions(const std::string& name, const unsigned int flags)const;

	std::map<std::string, unsigned int> m_reservedSymbols;
};