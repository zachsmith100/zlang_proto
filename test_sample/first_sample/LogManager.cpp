
#include "LogManager.h"

CLogManager Log;

CLogManager::CLogManager()
{

}

void CLogManager::SetCommandLine(int argc, char* argv[])
{
	std::string exePath = argv[0];

	Error.SetPath(exePath + ".error");
	Trace.SetPath(exePath + ".trace");
	Info.SetPath(exePath + ".info");
	Warnings.SetPath(exePath + ".warnings");	
}

CLogManager::~CLogManager()
{
}