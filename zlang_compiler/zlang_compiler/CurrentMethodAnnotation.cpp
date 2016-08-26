
#include "CurrentMethodAnnotation.h"
#include "ErrorReporter.h"
#include <assert.h>

// special parameter to describe returned value
#define RETURN_SPECIAL_PARAM "return"

CCurrentMethodAnnotation::CCurrentMethodAnnotation()
{
}

CCurrentMethodAnnotation::CCurrentMethodAnnotation(const CCurrentMethodAnnotation& other)
{
	assert(false);
}

CCurrentMethodAnnotation* CCurrentMethodAnnotation::GetInstance()
{
	static CCurrentMethodAnnotation* obj = NULL;

	if (!obj)
	{
		obj = new CCurrentMethodAnnotation();
	}

	return obj;
}

std::string CCurrentMethodAnnotation::GetCurrentParameter()const
{
	if (m_parameters.size() > 0)
	{
		return m_parameters[m_parameters.size() - 1].paramName;
	}

	return "";
}

void CCurrentMethodAnnotation::SetCurrentParameter(const std::string& name, const unsigned int line)
{
	m_parameters.push_back(ParameterAnnotation(name, line));
}

bool CCurrentMethodAnnotation::SetProperty(const std::string& name, const std::string& value)
{
	assert(m_parameters.size() > 0);
	if (m_parameters.size() > 0)
	{
		ParameterAnnotation& pa = m_parameters[m_parameters.size() - 1];
		if (name == "editor")
		{
			pa.editor = value;
		}
		else if (name == "group")
		{
			pa.group = value;
		}
		else if (name == "description")
		{
			pa.description = value;
		}
		else if (name == "min")
		{
			pa.min = atoi(value.c_str());
		}
		else if (name == "max")
		{
			pa.max = atoi(value.c_str());
		}
		else if (name == "defaultField")
		{
			m_defaultReturnField = value;
		}
		else
		{
			return false;
		}

		return true;
	}
	
	return false;	
}

bool CCurrentMethodAnnotation::SetProperty(const std::string& name, const std::vector<std::string>& values)
{
	assert(m_parameters.size() > 0);
	if (m_parameters.size() > 0)
	{
		ParameterAnnotation& pa = m_parameters[m_parameters.size() - 1];
		if (name == "validators")
		{
			pa.validators = values;
		}
		else
		{
			return false;
		}

		return true;
	}
	
	return false;	
}

ParameterAnnotation* CCurrentMethodAnnotation::FindParameterAnnotation(const std::string& name)
{
	std::vector<ParameterAnnotation>::iterator it = m_parameters.begin();
	std::vector<ParameterAnnotation>::iterator itEnd = m_parameters.end();

	for ( ; it != itEnd; it++)
	if ((*it).paramName == name)
	{
		(*it).used = true;
		return &(*it);
	}

	return NULL;
}

void CCurrentMethodAnnotation::FindNonUsedParameters()
{
	std::vector<ParameterAnnotation>::iterator it = m_parameters.begin();
	std::vector<ParameterAnnotation>::iterator itEnd = m_parameters.end();

	for ( ; it != itEnd; it++)
	{
		if (((*it).used == false) && ((*it).paramName != RETURN_SPECIAL_PARAM))
		{
			CErrorReporter::GetInstance()->OnFatalError(ANNOTATION_FOR_NON_USED_PARAMETER, (*it).startLine, (*it).paramName);
		}
	}
}

void CCurrentMethodAnnotation::Reset()
{
	FindNonUsedParameters();

	m_parameters.clear();
	m_defaultReturnField = "";
}

bool CCurrentMethodAnnotation::IsArrayAsValueNeeded(const std::string& name)const
{
	return (name == "validators");
}

void CCurrentMethodAnnotation::operator =(const CCurrentMethodAnnotation& other)
{
	assert(false);
}

std::string CCurrentMethodAnnotation::GetDefaultReturnField()const
{
	return m_defaultReturnField;
}

CCurrentMethodAnnotation::~CCurrentMethodAnnotation()
{
}