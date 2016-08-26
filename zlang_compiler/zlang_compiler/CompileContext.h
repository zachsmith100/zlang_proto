
#pragma once

#include "Common.h"
#include "SymbolTable.h"
#include <assert.h>
#include <stack>

// use API

#define CHANGE_STATE(a, val)	context.PushState(&context. a); context. a = val;
#define RESTORE_STATE(a)		context.PopState(&context. a);

// -------

typedef std::map<bool *, std::stack<bool> > StatesMap;

#define DEF_STATE(a) a = false; statesMap.insert(StatesMap::value_type(& a, std::stack<bool>()));

struct CompileContext
{	
	bool handlingCommaOperations;
	bool varDeclarationExists;
	bool classInstanceCreationExists;
	bool functionDeclaration;
	bool functionParametersDeclaration;	
	bool newVariablesProhibited;
	bool functionCallParameters;
	bool functionCallOutParameters;
	bool methodCallParameters;	
	bool usingObject;
	bool standardFunctionCompiling;
	bool globalFunctionCompiling;
	bool functionPointerCompiling;
	int highLevelCounter;
	std::stack<int> currentFunctionId;
	CSymbolTable* currentSymbolTable;

	std::string newClassName;
	std::string newClassInstanceName;

	StatesMap statesMap;
	
	CompileContext()
	{
		DEF_STATE(handlingCommaOperations);
		DEF_STATE(varDeclarationExists);
		DEF_STATE(functionDeclaration);
		DEF_STATE(functionParametersDeclaration);
		DEF_STATE(newVariablesProhibited);
		DEF_STATE(functionCallParameters);
		DEF_STATE(methodCallParameters);
		DEF_STATE(functionCallOutParameters);
		DEF_STATE(usingObject);
		DEF_STATE(classInstanceCreationExists);
		DEF_STATE(standardFunctionCompiling);
		DEF_STATE(globalFunctionCompiling);
		DEF_STATE(functionPointerCompiling);
		highLevelCounter = 0;
		currentSymbolTable = NULL;		
	}

	void PushState(bool* const state)
	{
		StatesMap::iterator it = statesMap.find(state);
		if (it != statesMap.end())
		{
			(*it).second.push(*state);
		}
		else
		{
			assert(false);
		}
	}

	void PopState(bool* const state)
	{
		StatesMap::iterator it = statesMap.find(state);
		if (it != statesMap.end())
		{
			if ((*it).second.size() != 0)
			{
				*state = (*it).second.top();
				(*it).second.pop();
			}
			else
			{
				assert(false);
			}
		}
		else
		{
			assert(false);
		}
	}

	void NewFunction(const unsigned int functionId)
	{
		currentFunctionId.push(functionId);
	}

	unsigned int GetCurrentFunctionId()
	{
		return currentFunctionId.top();
	}

	void EndFunction()
	{
		currentFunctionId.pop();
	}
};
