
#include "TestManager.h"
#include "Test.h"
#include <stdio.h>
           
CTestManager::CTestManager(char** tests, const unsigned int testCount):
	m_tests(tests),
	m_testCount(testCount)
{
}
	
void CTestManager::ExecuteAll()
{
	std::vector<std::string> failedTests;

	unsigned int successTestCount = 0;	

	for (unsigned int i = 0; i < m_testCount; i++)
	{
		CTest test(m_tests[i]);
		if (test.Execute())
		{
			successTestCount++;
		}
		else
		{
			failedTests.push_back(m_tests[i]);
		}
	}	

	printf("\n\n==============================================\n");
	printf("Successful tests: %d\n", successTestCount);
	printf("Failed tests: %d\n", (int)failedTests.size());
	
	std::vector<std::string>::iterator it = failedTests.begin();
	std::vector<std::string>::iterator itEnd = failedTests.end();
	for ( ; it != itEnd; it++)
	{
		printf("Failed test: %s\n", (*it).c_str());
	}

	printf((0 == failedTests.size()) ? "ALL IS OK" : "SOMETHING WRONG");	
	printf("\n\n");
}

CTestManager::~CTestManager()
{
}