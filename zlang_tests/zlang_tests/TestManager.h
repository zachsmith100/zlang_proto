
#pragma once

class CTestManager
{
public:
	CTestManager(char** tests, const unsigned int testCount);
	~CTestManager();

	void ExecuteAll();

private:
	char** m_tests;
	unsigned int m_testCount;
};