
#pragma once

#include <string>
#include <vector>

class CUnorderedVarDeclarations
{
public:
	static CUnorderedVarDeclarations* GetInstance();

	void SetScopeLevel(const unsigned int scopeLevel);
	
	void AddVariable(const std::string& varName);
	void ClearAll();

	bool IsEmpty()const;

	std::string GetDeclarationsString()const;	

private:
	CUnorderedVarDeclarations();
	CUnorderedVarDeclarations(const CUnorderedVarDeclarations& other);
	void operator =(const CUnorderedVarDeclarations& other);
	~CUnorderedVarDeclarations();

	std::string GetScopeLevelString()const;

	std::vector<std::string> m_variables;
	unsigned int m_scopeLevel;
};