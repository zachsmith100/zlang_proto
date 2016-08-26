
#include "UniqueIdGenerator.h"
#include <assert.h>
#include "Utils.h"

CUniqueIdGenerator::CUniqueIdGenerator():
	m_currentNum(0)
{
}

CUniqueIdGenerator::CUniqueIdGenerator(const CUniqueIdGenerator& other)
{
	assert(false);
}

CUniqueIdGenerator* CUniqueIdGenerator::GetInstance()
{
	static CUniqueIdGenerator* obj = NULL;

	if (!obj)
	{
		obj = new CUniqueIdGenerator();
	}

	return obj;
}

std::string CUniqueIdGenerator::GetId()
{
	m_currentNum++;
	std::string id = "__tmp__";
	id += IntToStr(m_currentNum);
	id += "__";
	return id;
}

void CUniqueIdGenerator::operator =(const CUniqueIdGenerator& other)
{
	assert(false);
}

CUniqueIdGenerator::~CUniqueIdGenerator()
{
}