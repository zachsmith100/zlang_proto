COMPILE_PARAMS = 
CC = g++

all: project

project: 
	$(CC) -std=c++0x -o project main.cpp UniVar.cpp StandardFuncs.cpp LogManager.cpp Log.cpp ClassSupport.cpp ArraySupport.cpp jsoncpp.cpp HashtableSupport.cpp StreamSupport.cpp minIni.cpp tinyxml2.cpp GlobalMethods.cpp [[cycle|cppFiles|makefileCpp.tpl]] [[cycle|libraries|makefileLibrary.tpl]] [[cycle|libraryPaths|makefileLibraryPath.tpl]] [[cycle|headerPaths|makefileHeaderPath.tpl]] [[cycle|compilerFlags|makefileCompilerFlags.tpl]]

clean:
	rm project
