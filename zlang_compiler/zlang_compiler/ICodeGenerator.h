
#pragma once

#include "node.h"
#include <string>
#include <vector>
#include "assert.h"
#include "CompileContext.h"

#define PROCESS_TOKENS_START(className) std::string className::GenerateCodeForNode(node* const nd, CompileContext& context) { switch(nd->cmd) {
#define PROCESS_TOKEN(tkn, func)	case tkn: return func(nd, context); break;
#define PROCESS_TOKENS_END	default: assert(false); return ""; } return ""; }
#define DECLARE_CODE_GENERATOR_CONSTRUCTOR(name) DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(name) { } 
#define DECLARE_CODE_GENERATOR_CONSTRUCTOR_HEADER(name) name::name(CCodeGenerator* codeGenerator): CBaseCodeGenerator(codeGenerator)
#define DECLARE_CODE_GENERATOR_DESTRUCTOR(name) name::~name() { }
#define DECLARE_HANDLER(handler) std::string handler(node* const nd, CompileContext& context)

#define DECLARE_CLASS_DEFINITION(className) public: \
	className(CCodeGenerator* codeGenerator); \
	~className(); \
	std::string GenerateCodeForNode(node* const nd, CompileContext& context); \
	std::vector<int> GetListOfSupportedTokens()const;	\
protected:

class ICodeGenerator
{
public:
	virtual std::string GenerateCodeForNode(node* const nd, CompileContext& context) = 0;
	virtual std::vector<int> GetListOfSupportedTokens()const = 0;
	virtual ~ICodeGenerator() { };
};