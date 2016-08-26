
#include "Utils.h"
#include <cstdlib>

void ParseExePath(const std::string& exePath, std::string& exeDir, std::string& exeName)
{
	int pos1 = exePath.find_last_of("\\");
	int pos2 = exePath.find_last_of("/");
	int pos = (pos1 > pos2) ? pos1 : pos2;

	exeName = exePath.substr(pos + 1, exePath.size() - pos - 1);	
	exeDir = exePath.substr(0, pos + 1);	
	if (exeDir == "./")
	{
		exeDir = "";
	}
}

bool IsFloat(const std::string& str)
{
	unsigned int len = str.size();
	unsigned int dotsCount = 0;

	unsigned int startOffset = ((str[0] == '-') || (str[0] == '+')) ? 1 : 0;

	for (unsigned int i = startOffset; i < len; i++)
	if ((str[i] < '0') || (str[i] > '9'))
	{		
		if ((str[i] == '.') && (0 == dotsCount))
		{
			dotsCount++;			
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool IsStringConvertableToInt(const std::string& str)
{
	if (str.size() == 0)
	{
		return false;
	}

	if (str.find(".") != -1)
	{
		return false;
	}

	int n = atoi(str.c_str());
	if (n != 0)
	{
		return true;
	}

	int i = 0;
	if ((str.size() > 1) && ((str[0] == '+') || (str[0] == '-')))
	{
		i++;		
	}

	for (unsigned int i = 0; i < str.size(); i++)
	if ((str[i] < '0') || (str[i] > '9'))
	{
		return false;
	}

	return true;
}

bool IsStringConvertableToFloat(const std::string& str)
{
	if (str.size() == 0)
	{
		return false;
	}

	double n = atof(str.c_str());
	if (n != 0.0)
	{
		return true;
	}

	return IsFloat(str);	
}
