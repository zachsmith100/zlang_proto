
#pragma once

#include "BaseCodeGenerator.h"

class CConditionsCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CConditionsCodeGenerator)

	DECLARE_HANDLER(ProcessOR);
	DECLARE_HANDLER(ProcessAND);
	DECLARE_HANDLER(ProcessXOR);
	DECLARE_HANDLER(ProcessIF);	
	DECLARE_HANDLER(ProcessShortIF);	
	DECLARE_HANDLER(ProcessElse);

public:
	static bool IsCondition(const int cmd);
};