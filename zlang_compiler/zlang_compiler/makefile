COMPILE_PARAMS = -std=c++11
CC = g++

all: zlangcompiler

zlangcompiler: 
	$(CC) $(COMPILE_PARAMS) -o zlangcompiler fmemopen.cpp lex.yy.cpp yacc.tab.cpp zlang_compiler.cpp CodeGenerator.cpp ScopeLevel.cpp SymbolTable.cpp SymbolTableBuilder.cpp SyntaxTreeProcessor.cpp Compiler.cpp FunctionsCodeGenerator.cpp ConditionsCodeGenerator.cpp MathCodeGenerator.cpp VariablesCodeGenerator.cpp CyclesCodeGenerator.cpp CompareCodeGenerator.cpp ProjectGenerator.cpp Utils.cpp UnorderedVarDeclarations.cpp BaseCodeGenerator.cpp StandardFunctions.cpp Classes.cpp ClassInfo.cpp CodeTemplate.cpp CodeTemplates.cpp ErrorReporter.cpp GlobalSymbolEnumeration.cpp NodeListBuilder.cpp PredefinedVariables.cpp UniversalMethodDesc.cpp ClassCodeGenerator.cpp CompileTarget.cpp ClassCodeAnalyzer.cpp FunctionsCodeAnalyzer.cpp ProjectCompiler.cpp CompilerPath.cpp CommentCodeGenerator.cpp Path.cpp FileFinder.cpp ImportManager.cpp ReservedKeywords.cpp CppInsertCodeGenerator.cpp jsoncpp.cpp GlobalFunctions.cpp ComplexStatementsManager.cpp CurrentMethodAnnotation.cpp UniqueIdGenerator.cpp

clean:
	rm zlangcompiler
