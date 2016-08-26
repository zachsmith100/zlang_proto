
#pragma once 

#include <string>
#include <vector>
#include <map>

typedef std::map<std::string, unsigned int> VariablesMap;
typedef std::vector<std::string> ValuesList;

class CCodeTemplate;

struct SVariableValue
{
	std::string stringValue;
	CCodeTemplate* tpl;

	SVariableValue()
	{		
		tpl = NULL;
	}

	SVariableValue(const SVariableValue& other)
	{	
		stringValue = other.stringValue;
		tpl = other.tpl;
	}
};

class CCodeTemplate
{
public:
	CCodeTemplate();
	~CCodeTemplate();

	bool Load(const std::string& name);
	bool IsLoaded()const;
	std::string GetFileName()const;	
	std::string GetFileFolder()const;

	unsigned int GetVariableIndex(const std::string& name);
	bool IsVariableExists(const std::string& name)const;

	bool SetVariableValue(const std::string& name, const std::string& value);
	bool SetVariableValue(const unsigned int index, const std::string& value);
	bool SetVariableValue(const std::string& name, const int value);
	bool SetVariableValue(const unsigned int index, const int value);

	bool SetVariableValue(const std::string& name, const ValuesList& values);
	bool SetVariableValue(const unsigned int index, const ValuesList& values);
	bool SetVariableRange(const std::string& name, const int min, const int max, const bool isCommaSeparated = false);
	bool SetVariableRange(const unsigned int index, const int min, const int max, const bool isCommaSeparated = false);	
	void ClearValues();

	std::string GetText()const;	

private:
	unsigned int InitStringAndGetPredictedSize(std::string& str, const int min, const int max);

	size_t FindDoubledSymbol(const std::string& str, const unsigned int offset, const char symbol);

	std::string LoadFile(FILE* templateFile);
	bool ParseStringTemplate(const std::string& stringTpl);	
	unsigned int GetFileSize(FILE* templateFile)const;

	void HandleVariableDeclaration(const std::string& name);
	void RegisterNewVariable(const std::string& name);

	void ClearAll();

	std::string m_fileName;
	bool m_isLoaded;

	VariablesMap m_indexByVariableName;

	std::vector<SVariableValue> m_variableValues;
	
	std::vector<std::string> m_templateParts;
	std::vector<int> m_templateReplacements;
		
	unsigned int m_summaryPredictedResultTextSize;
	unsigned int m_cyclesPredictedResultTextSize;
};