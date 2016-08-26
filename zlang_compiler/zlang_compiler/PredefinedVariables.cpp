
#include "Common.h"
#include "PredefinedVariables.h"

CPredefinedVariables::CPredefinedVariables()
{
	m_vars.insert(std::map<std::string, int>::value_type("error", 1));
	m_vars.insert(std::map<std::string, int>::value_type("trace", 1));
	m_vars.insert(std::map<std::string, int>::value_type("info", 1));
	m_vars.insert(std::map<std::string, int>::value_type("warnings", 1));	
}

CPredefinedVariables* CPredefinedVariables::GetInstance()
{
	static CPredefinedVariables* obj = NULL;

	if (!obj)
	{
		obj = new CPredefinedVariables();
	}

	return obj;
}

bool CPredefinedVariables::IsPredefinedVariable(const std::string& id)
{
	std::map<std::string, int>::iterator it = m_vars.find(id);
	return (it != m_vars.end());
}

CPredefinedVariables::~CPredefinedVariables()
{
}