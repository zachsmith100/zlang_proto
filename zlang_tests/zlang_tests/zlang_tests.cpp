
#include "Common.h"
#include "CompileExecutor.h"
#include <stdio.h>
#include "Test.h"
#include "TestManager.h"

int main(int argc, char* argv[])
{
	if ((argc != 2) && (argc != 4))
	{
		printf("Usage: zlang_tests.exe [compiler path]\n");
		printf("or\n");
		printf("Usage: zlang_tests.exe [compiler path] [test folder] [output folder]\n");
		return -1;
	}

	CCompileExecutor::GetInstance()->SetCompilerPath(argv[1]);

	if (argc == 4)
	{
		CTest test(argv[2], argv[3]);
		test.Execute();
		return 0;
	}
	
	const unsigned int TESTS_OFFSET = 0;
	/*const unsigned int TESTS_COUNT = 55;
	const char* tests[TESTS_COUNT] = {"testjson", "testoutparams#1correct", "testoutparams#2wrong", "testmethodsandpropsdiff", "testemptyobject", "testarray#1correct", "testarray#2edgecases", "testarray#3wrong", "testarray#4correct", "testhashtable#1correct", "testhashtable#2edgecases", "testhashtable#3wrong", "testconditions#1correct", "testconditions#2wrong", "testconditions#3correct", "testinnercalls#1correct", "teststrings#1correct", "teststrings#2edgecases", "testcomments", "testregexp#1correct", "testregexp#2edgecases", "testregexp#3incorrect", "testclass#1correct", "testfiles#1correct", "testfiles#2edgecases", "testfiles#3correct", "testfiles#4edgecases", "testcpp#1correct", "testcpp#2correct", "testcpp#3correct", "testproblem#1", "testproblem#2", "testcycles#1correct", "testcycles#2wrong", "testfunctions#1correct", "testfunctions#2wrong", "testclass#2correct", "testclass#3wrong", "testfunctionpointers#1correct", "testfunctionpointers#2wrong", "testfunctionpointers#4correct", "testconvertable", "testmath#1correct", "testmath#2edgecases", "testmath#3wrong", "testconditions#4correct", "testconditions#5wrong", "testlazyinit#1correct", "testlazyinit#2wrong", "testfloatingpoint#1correct", "testdifferentfloats#1correct", "testdifferentfloats#2wrong", "testfloatingpoint#2edgecases", "testfloatingpoint#3wrong", "testfloatingpoint#4wrong"};*/

	// for fast debug one test	
	const unsigned int TESTS_COUNT = 1;
	const char* tests[TESTS_COUNT] = {"testfloatingpoint#2edgecases"};

	/*const unsigned int TESTS_COUNT = 2;
	const char* tests[TESTS_COUNT] = {"testoutparams#2wrong", "testproblem#2"};*/

	CTestManager testManager((char**)&tests[TESTS_OFFSET], TESTS_COUNT - TESTS_OFFSET);	
	testManager.ExecuteAll();	

	return 0;
}
