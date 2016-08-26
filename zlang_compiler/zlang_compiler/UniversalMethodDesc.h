
#pragma once

#include <vector>
#include <algorithm>

struct SPossibleParameterCount
{
	unsigned int parametersCount;
	std::vector<bool> isOutParameter;

	SPossibleParameterCount(const unsigned int _parametersCount, const std::vector<bool>& _isOutParameter)
	{
		parametersCount = _parametersCount;
		isOutParameter = _isOutParameter;
	}

	SPossibleParameterCount(const SPossibleParameterCount& other)
	{
		parametersCount = other.parametersCount;
		isOutParameter = other.isOutParameter;
	}
};

class CUniversalMethodDesc
{
public:
	CUniversalMethodDesc();
	CUniversalMethodDesc(const unsigned int parametersCount, bool isPublic, const std::vector<bool>& isOutParameter);
	CUniversalMethodDesc(const CUniversalMethodDesc& other);

	void AddParameterCount(const unsigned int parametersCount, const bool canBePublic, const std::vector<bool>& isOutParameter);
	bool IsParameterCountSupported(const unsigned int parametersCount, const bool canBePublic)const;	

	bool CanBePublic()const;

	bool IsOutMethodParameter(const unsigned int paramIndex)const;

private:	
	std::vector<SPossibleParameterCount> m_possibleParametersCount[2]; // for private/public use-cases		
};