
#pragma once

#include <string>

class CUniqueIdGenerator
{
public:
	static CUniqueIdGenerator* GetInstance();

	std::string GetId();

private:
	CUniqueIdGenerator();
	CUniqueIdGenerator(const CUniqueIdGenerator& other);
	void operator =(const CUniqueIdGenerator& other);
	~CUniqueIdGenerator();

	unsigned int m_currentNum;
};