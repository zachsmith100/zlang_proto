
#pragma once

#include "BaseCodeGenerator.h"

class CCyclesCodeGenerator : public CBaseCodeGenerator
{
	DECLARE_CLASS_DEFINITION(CCyclesCodeGenerator)

	DECLARE_HANDLER(ProcessWhile);
	DECLARE_HANDLER(ProcessFor);
	DECLARE_HANDLER(ProcessForEach);
	DECLARE_HANDLER(ProcessCommaSeparatedCommands);
	DECLARE_HANDLER(ProcessBreak);
	DECLARE_HANDLER(ProcessContinue);
};