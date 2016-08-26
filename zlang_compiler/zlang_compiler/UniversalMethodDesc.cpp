
#include "Common.h"
#include "UniversalMethodDesc.h"

CUniversalMethodDesc::CUniversalMethodDesc()
{				
}

CUniversalMethodDesc::CUniversalMethodDesc(const unsigned int parametersCount, bool isPublic, const std::vector<bool>& isOutParameter)
{			
	SPossibleParameterCount desc(parametersCount, isOutParameter);
	m_possibleParametersCount[(int)isPublic].push_back(desc);
}

CUniversalMethodDesc::CUniversalMethodDesc(const CUniversalMethodDesc& other)
{
	m_possibleParametersCount[0] = other.m_possibleParametersCount[0];
	m_possibleParametersCount[1] = other.m_possibleParametersCount[1];	
}

void CUniversalMethodDesc::AddParameterCount(const unsigned int parametersCount, const bool canBePublic, const std::vector<bool>& isOutParameter)
{
	int index = (int)canBePublic;
	std::vector<SPossibleParameterCount>::iterator it = m_possibleParametersCount[index].begin();
	std::vector<SPossibleParameterCount>::iterator itEnd = m_possibleParametersCount[index].end();

	bool isFound = false;
	for ( ; it != itEnd; it++)
	if ((*it).parametersCount == parametersCount)	
	{
		isFound = true;
		for (unsigned int k = 0; k < isOutParameter.size(); k++)
		if (isOutParameter[k])
		{
			(*it).isOutParameter[k] = true;
		}
	}	

	if (!isFound)
	{
		SPossibleParameterCount desc(parametersCount, isOutParameter);
		m_possibleParametersCount[index].push_back(desc);
	}
}

bool CUniversalMethodDesc::IsParameterCountSupported(const unsigned int parametersCount, const bool canBePublic)const
{
	int index = (int)canBePublic;

	std::vector<SPossibleParameterCount>::const_iterator it = m_possibleParametersCount[index].begin();
	std::vector<SPossibleParameterCount>::const_iterator itEnd = m_possibleParametersCount[index].end();
	
	for ( ; it != itEnd; it++)
	if ((*it).parametersCount == parametersCount)	
	{
		return true;
	}

	return false;
}

bool CUniversalMethodDesc::CanBePublic()const
{
	return (m_possibleParametersCount[1].size() > 0);
}

bool CUniversalMethodDesc::IsOutMethodParameter(const unsigned int paramIndex)const
{
	std::vector<SPossibleParameterCount>::const_iterator it = m_possibleParametersCount[1].begin();
	std::vector<SPossibleParameterCount>::const_iterator itEnd = m_possibleParametersCount[1].end();
	
	for ( ; it != itEnd; it++)
	if ((paramIndex < (*it).parametersCount) && ((*it).isOutParameter[paramIndex]))
	{
		return true;
	}

	return false;
}