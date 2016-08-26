
#include "Common.h"
#include "CodeTemplates.h"
#include "CodeTemplate.h"
#include "CompilerPath.h"
#include "Utils.h"
#include <assert.h>
#include <string.h>

#define PREDICTED_MAX_VAR_SIZE 20

CCodeTemplate::CCodeTemplate():
	m_isLoaded(false),
	m_cyclesPredictedResultTextSize(0),
	m_summaryPredictedResultTextSize(0)
{
}

bool CCodeTemplate::Load(const std::string& name)
{
	CFilePath path(CCompilerPath::Get());
	path.Add(name);
	FILE* tplFile = OpenFile(path, "rb");
	if (!tplFile)
	{
		return false;
	}

	m_fileName = name;
	ClearAll();
	const std::string& stringTemplate = LoadFile(tplFile);	
	if (!ParseStringTemplate(stringTemplate))
	{
		return false;
	}
		
	fclose(tplFile);
	return true;
}

bool CCodeTemplate::IsLoaded()const
{
	return m_isLoaded;
}

std::string CCodeTemplate::GetFileName()const
{
	return m_fileName;
}

std::string CCodeTemplate::GetFileFolder()const
{
	CPath path(m_fileName);
	return path.GetDirectory();
}

unsigned int CCodeTemplate::GetVariableIndex(const std::string& name)
{
	VariablesMap::const_iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		return 0xFFFFFFFF;
	}

	return (*it).second;
}

bool CCodeTemplate::IsVariableExists(const std::string& name)const
{
	VariablesMap::const_iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		return false;
	}

	return true;
}

bool CCodeTemplate::SetVariableValue(const std::string& name, const std::string& value)
{
	VariablesMap::iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		assert(false);
		return false;
	}

	return SetVariableValue((*it).second, value);	
}

bool CCodeTemplate::SetVariableValue(const unsigned int index, const std::string& value)
{
	assert(index < m_variableValues.size());
	if (index >= m_variableValues.size())
	{
		return false;
	}

	m_variableValues[index].stringValue = value;	
	return true;
}

bool CCodeTemplate::SetVariableValue(const std::string& name, const int value)
{
	VariablesMap::iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		assert(false);
		return false;
	}

	return SetVariableValue((*it).second, value);	
}

bool CCodeTemplate::SetVariableValue(const unsigned int index, const int value)
{
	assert(index < m_variableValues.size());
	if (index >= m_variableValues.size())
	{
		return false;
	}

	m_variableValues[index].stringValue = IntToStr(value);	
	return true;
}

bool CCodeTemplate::SetVariableValue(const std::string& name, const ValuesList& values)
{
	VariablesMap::iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		assert(false);
		return false;
	}

	return SetVariableValue((*it).second, values);	
}

bool CCodeTemplate::SetVariableValue(const unsigned int index, const ValuesList& values)
{
	assert(index < m_variableValues.size());
	if (index >= m_variableValues.size())
	{
		return false;
	}

	CCodeTemplate* tpl = m_variableValues[index].tpl;
	if (!tpl)
	{
		return false;
	}

	unsigned int predictedSize = InitStringAndGetPredictedSize(m_variableValues[index].stringValue, 0, values.size());
	
	ValuesList::const_iterator vit = values.begin();
	ValuesList::const_iterator vitEnd = values.end();
	for ( ; vit != vitEnd; vit++)
	{
		tpl->SetVariableValue(0, *vit);
		m_variableValues[index].stringValue += tpl->GetText();
	}

	m_cyclesPredictedResultTextSize += predictedSize;
	
	return true;
}

bool CCodeTemplate::SetVariableRange(const std::string& name, const int min, const int max, const bool isCommaSeparated)
{
	VariablesMap::iterator it = m_indexByVariableName.find(name);
	if (it == m_indexByVariableName.end())
	{
		assert(false);
		return false;
	}

	return SetVariableRange((*it).second, min, max, isCommaSeparated);	
}

unsigned int CCodeTemplate::InitStringAndGetPredictedSize(std::string& str, const int min, const int max)
{
	unsigned int predictedSize = (max - min + 1) * PREDICTED_MAX_VAR_SIZE;
	str = "";	
	str.reserve(predictedSize);
	return predictedSize;
}

bool CCodeTemplate::SetVariableRange(const unsigned int index, const int min, const int max, const bool isCommaSeparated)
{
	CCodeTemplate* tpl = m_variableValues[index].tpl;
	if (!tpl)
	{
		return false;
	}

	unsigned int predictedSize = InitStringAndGetPredictedSize(m_variableValues[index].stringValue, min, max);
		
	for (int i = min; i <= max; i++)
	{
		tpl->SetVariableValue(0, IntToStr(i) + (((isCommaSeparated) && (i != max)) ? ", " : ""));
		m_variableValues[index].stringValue += tpl->GetText();
	}

	m_cyclesPredictedResultTextSize += predictedSize;
	
	return true;
}

void CCodeTemplate::ClearValues()
{
	std::vector<SVariableValue>::iterator it = m_variableValues.begin();
	std::vector<SVariableValue>::iterator itEnd = m_variableValues.end();

	for ( ; it != itEnd; it++)
	{
		(*it).stringValue = "";		
	}
}

std::string CCodeTemplate::GetText()const
{
	assert(m_templateParts.size() == m_templateReplacements.size() + 1);

	std::string text;
	text.reserve(m_summaryPredictedResultTextSize + m_cyclesPredictedResultTextSize);

	std::vector<std::string>::const_iterator tit = m_templateParts.begin();
	
	std::vector<int>::const_iterator rit = m_templateReplacements.begin();
	std::vector<int>::const_iterator ritEnd = m_templateReplacements.end();
	
	for ( ; rit != ritEnd; rit++, tit++)
	{
		text += (*tit);
		text += m_variableValues[(*rit)].stringValue;		
	}

	text += (*tit);

	return text;
}

unsigned int CCodeTemplate::GetFileSize(FILE* templateFile)const
{
	fseek(templateFile, 0, 2);
	unsigned int size = ftell(templateFile);
	fseek(templateFile, 0, 0);
	return size;
}

std::string CCodeTemplate::LoadFile(FILE* templateFile)
{
	std::string result;
	result.reserve(GetFileSize(templateFile));

	char tmp[4096];	
	while ((!feof(templateFile)) && (fgets(tmp, sizeof(tmp) - 1, templateFile)))
	{
		for (unsigned int i = 0; i < strlen(tmp); i++)
		if (tmp[i] == 13)
		{
			tmp[i] = 32;
		}
		result += tmp;
	}

	return result;
}

size_t CCodeTemplate::FindDoubledSymbol(const std::string& str, const unsigned int offset, const char symbol)
{
	for (unsigned int i = offset; i < str.size() - 1; i++)
	if ((str[i] == symbol) && (str[i + 1] == symbol))
	{
		return i;
	}

	return std::string::npos;
}

bool CCodeTemplate::ParseStringTemplate(const std::string& stringTpl)
{
	int partCounter = 0;

	int lastPos = 0;
	int delimPos = FindDoubledSymbol(stringTpl, 0, '[');
	int checkPos = FindDoubledSymbol(stringTpl, 0, ']');
	if ((checkPos != std::string::npos) && (checkPos < delimPos))
	{
		printf("Invalid variable declaration in code template file %s", GetFileName().c_str());
		return false;
	}
	
	while (delimPos != std::string::npos)
	{
		const std::string& part = stringTpl.substr(lastPos, delimPos - lastPos);
		if (partCounter%2 == 0)
		{
			m_templateParts.push_back(part);
			m_summaryPredictedResultTextSize += part.size();
		}
		else if (partCounter%2 == 1)
		{
			HandleVariableDeclaration(part);
		}
		partCounter++;
		lastPos = delimPos + 2;
		delimPos = FindDoubledSymbol(stringTpl, lastPos, (partCounter%2 == 1) ? ']' : '[');
		checkPos = FindDoubledSymbol(stringTpl, lastPos, (partCounter%2 == 0) ? ']' : '[');
		if ((checkPos != std::string::npos) && (checkPos < delimPos))
		{
			printf("Invalid variable declaration in code template file %s", GetFileName().c_str());
			return false;
		}
	}

	if (partCounter%2 == 1)
	{
		printf("Invalid variable declaration in code template file %s", GetFileName().c_str());
		return false;
	}

	const std::string& part = stringTpl.substr(lastPos, stringTpl.size() - lastPos);
	m_templateParts.push_back(part);
		
	return true;
}

void CCodeTemplate::RegisterNewVariable(const std::string& name)
{	
	m_variableValues.push_back(SVariableValue());
	m_indexByVariableName.insert(VariablesMap::value_type(name, m_variableValues.size() - 1));
}

void CCodeTemplate::HandleVariableDeclaration(const std::string& name)
{
	if ((name.size() >= 6) && (name.substr(0, 6) == "cycle|"))
	{
		int pos = name.find_first_of("|", 7);
		std::string tpl = name.substr(pos + 1, name.size() - pos - 1);
		std::string varName = name.substr(6, pos - 6);
		RegisterNewVariable(varName);
		
		m_variableValues[m_variableValues.size() - 1].tpl = CCodeTemplates::GetInstance()->LoadTemplate(GetFileFolder() + tpl);
		m_templateReplacements.push_back(m_variableValues.size() - 1);
	}
	else
	{
		VariablesMap::iterator it = m_indexByVariableName.find(name);
		if (it == m_indexByVariableName.end())
		{
			RegisterNewVariable(name);
			m_templateReplacements.push_back(m_variableValues.size() - 1);
		}
		else
		{
			m_templateReplacements.push_back((*it).second);
		}
		m_summaryPredictedResultTextSize += PREDICTED_MAX_VAR_SIZE;
	}
}

void CCodeTemplate::ClearAll()
{
	m_indexByVariableName.clear();	
	m_variableValues.clear();
	m_templateParts.clear();
	m_templateReplacements.clear();
}

CCodeTemplate::~CCodeTemplate()
{
}
