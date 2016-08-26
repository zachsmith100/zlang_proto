
#include "Common.h"
#include "CommentCodeGenerator.h"

DECLARE_CODE_GENERATOR_CONSTRUCTOR(CCommentCodeGenerator)

PROCESS_TOKENS_START(CCommentCodeGenerator)
	PROCESS_TOKEN(OP_SINGLELINE_COMMENT, ProcessSinglelineComment);	
	PROCESS_TOKEN(OP_MULTILINE_COMMENT, ProcessMultilineComment);	
PROCESS_TOKENS_END

DECLARE_HANDLER(CCommentCodeGenerator::ProcessSinglelineComment)
{
	return nd->left->token;
}

DECLARE_HANDLER(CCommentCodeGenerator::ProcessMultilineComment)
{
	return nd->left->token;
}

std::vector<int> CCommentCodeGenerator::GetListOfSupportedTokens()const
{	
	std::vector<int> res;
	res.push_back(OP_SINGLELINE_COMMENT);
	res.push_back(OP_MULTILINE_COMMENT);
	return res;
}

DECLARE_CODE_GENERATOR_DESTRUCTOR(CCommentCodeGenerator)