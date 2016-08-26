
#pragma once

#include "Log.h"

class CLogManager
{
public:	
	CLogManager();
	~CLogManager();

	void SetCommandLine(int argc, char* argv[]);

public:
	CLog Error;
	CLog Trace;
	CLog Info;
	CLog Warnings;
};

extern CLogManager Log;