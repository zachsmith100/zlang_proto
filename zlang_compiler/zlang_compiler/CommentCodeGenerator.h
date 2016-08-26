
#pragma once

#include "BaseCodeGenerator.h"

class CCommentCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CCommentCodeGenerator)

	DECLARE_HANDLER(ProcessMultilineComment);
	DECLARE_HANDLER(ProcessSinglelineComment);
};
