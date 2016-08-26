COMPILE_PARAMS = 
CC = g++

all: zlangtests

zlangtests: 
	$(CC) -o zlangtests zlang_tests.cpp CompileExecutor.cpp Test.cpp TestManager.cpp ../../zlang_compiler/zlang_compiler/fmemopen.cpp ../../zlang_compiler/zlang_compiler/Utils.cpp ../../zlang_compiler/zlang_compiler/Path.cpp ../../zlang_compiler/zlang_compiler/FileFinder.cpp -I ../../zlang_compiler/zlang_compiler/

clean:
	rm zlangtests
