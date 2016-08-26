
#pragma once

#include <string>
#include <map>

class CPredefinedVariables
{
public:
	bool IsPredefinedVariable(const std::string& id);

	static CPredefinedVariables* GetInstance();

private:
	CPredefinedVariables();
	CPredefinedVariables(const CPredefinedVariables& other);
	void operator =(const CPredefinedVariables& other);
	~CPredefinedVariables();

	std::map<std::string, int> m_vars;
};