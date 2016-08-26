
#include "StandardFuncs.h"
#include "StreamSupport.h"
#include "LogManager.h"
#include <iostream>
#include <string.h>
#include "SymbolEnums.h"

unsigned int currentLine = 0;
int currentClass = -1;
unsigned int currentMethod = 0;

#ifdef __unix__
	#define _stricmp strcasecmp
#endif

#ifdef __APPLE__
	#define _stricmp strcasecmp
#endif

void zassert(bool cond, const std::string& fileName, const unsigned int line)
{
	if (!cond)
	{
		printf("Assertion failed in %s at line %d\n", fileName.c_str(), line);		
		abort();
	}
}

void zassert(bool cond, const std::string& errorText, const std::string& fileName, const unsigned int line)
{
	if (!cond)
	{
		printf("Assertion failed in %s at line %d\n", fileName.c_str(), line);
		printf("%s\n", errorText.c_str());
		abort();
	}
}

CUniVar parseInt(const CUniVar& var)
{
	if (var.IsConvertableToType(UV_INT))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_INT);
		return tmp;
	}

	if (var.IsConvertableToType(UV_STRING))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_STRING);
		return atoi(tmp.ToString().c_str());
	}

	return 0;
}

CUniVar parseFloat(const CUniVar& var)
{
	if (var.IsConvertableToType(UV_DOUBLE))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_DOUBLE);
		return tmp;
	}

	if (var.IsConvertableToType(UV_STRING))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_STRING);
		return atof(tmp.ToString().c_str());
	}

	return 0.0f;
}

CUniVar parseBoolean(const CUniVar& var)
{
	if (var.IsConvertableToType(UV_BOOLEAN))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_BOOLEAN);
		return tmp;
	}

	if (var.IsConvertableToType(UV_STRING))
	{
		CUniVar tmp = var;
		tmp.ConvertToType(UV_STRING);
		return _stricmp(tmp.ToString().c_str(), "true") == 0;
	}

	return false;
}

CUniVar print_(const CUniVar& var)
{	
	std::cout << var.ToString();	

	return true;
}

CUniVar print_(CUniVar stream, CUniVar var, CUniVar& byteCount)
{
	if (stream.GetType() == UV_INT)
	{
		switch (stream)
		{
			case error:
				Log.Error.AddLine(var);
				break;

			case trace:
				Log.Trace.AddLine(var);
				break;

			case info:
				Log.Info.AddLine(var);
				break;

			case warnings:
				Log.Warnings.AddLine(var);
				break;

			default:
				return false;
		}	

		return true;
	}
	else if (stream.GetType() == UV_STREAM)
	{
		return stream.ToStream()->Print(var, byteCount);
	}

	return false;
}

CUniVar print_(CUniVar stream, CUniVar var)
{	
	CUniVar zero(0);
	return print_(stream, var, zero);
}

CUniVar println_(const CUniVar& var)
{
	print_(var);
	print_(CUniVar("\n"));

	return true;
}

CUniVar println_(CUniVar stream, CUniVar var)
{
	CUniVar res1 = print_(stream, var);
	CUniVar res2 = true;
	if (stream.GetType() == UV_STREAM)
	if (stream.ToStream()->GetType() == STREAM_TEXT)
	{
		res2 = print_(stream, "\n");
	}
	
	return (res1 == true) && (res2 == true);
}

CUniVar println_(CUniVar stream, CUniVar var, CUniVar& byteCount)
{
	return print_(stream, var, byteCount);	
}

void trimString(std::string& str)
{
	while ((str.size() > 0) && (str[0] == ' '))
	{
		str.erase(str.begin());
	}

	while ((str.size() > 0) && ((str[str.size() - 1] == ' ') || (str[str.size() - 1] == '\n') || (str[str.size() - 1] == '\r')))
	{
		str.erase(str.begin() + str.size() - 1);
	}
}

CUniVar append(CUniVar& var)
{
	std::string str;
	getline(std::cin, str);
	
	trimString(str);

	if ((CUniVar::IsJSONObject(str)) || (CUniVar::IsJSONArray(str)))
	{		
		var.UpdateStateFromJson(str);
	}
	else if ((var.GetType() != UV_CLASS) && (var.GetType() != UV_ARRAY))
	{
		CUniVar secondVar = str;
		secondVar.UpdateType();
		if (secondVar.IsConvertableToTypeOf(&var))
		{
			secondVar.ConvertToTypeOf(&var);
			if (secondVar.GetType() == UV_STRING)
			{
				var %= secondVar; 
			}
			else
			{
				var += secondVar;
			}
		}	
		else
		{
			print2(warnings, CUniVar("Cannot append '" + str + "' to value of type " + var.GetTypeAsString()));
		}
	}
	else
	{
		print2(warnings, CUniVar("Cannot append '" + str + "' to instance of class/array"));
	}

	return true;
}

CUniVar read(CUniVar& var)
{
	std::string str;
	getline(std::cin, str);
	
	trimString(str);

	if ((CUniVar::IsJSONObject(str)) || (CUniVar::IsJSONArray(str)))
	{				
		var.UpdateStateFromJson(str, true);
	}
	else
	{
		var.PermitChangeType();
		var = str;
		var.ProhibitChangeType();
		var.UpdateType();
	}

	return true;
}

CUniVar read(CUniVar stream, CUniVar& var)
{
	if (stream.GetType() != UV_STREAM)
	{
		return false;
	}

	CUniVar zero(0);
	return stream.ToStream()->Read(var, zero);
}

CUniVar read(CUniVar stream, CUniVar& var, CUniVar& byteCount)
{
	if (stream.GetType() != UV_STREAM)
	{
		return false;
	}

	return stream.ToStream()->Read(var, byteCount);
}

CUniVar openStream(CUniVar path, CUniVar type)
{
	CStreamDataContainer* sdt = new CStreamDataContainer(path, type, false);
	return CUniVar(sdt);
}

CUniVar createStream(CUniVar path, CUniVar type)
{
	CStreamDataContainer* sdt = new CStreamDataContainer(path, type, true);
	return CUniVar(sdt);
}

CUniVar clearStream(CUniVar stream)
{
	if (stream.GetType() != UV_STREAM)
	{
		return false;		
	}
	
	return stream.ToStream()->Clear();
}

CUniVar seekStream(CUniVar stream, CUniVar shift)
{
	if (stream.GetType() != UV_STREAM)
	{
		return false;		
	}
	
	return stream.ToStream()->Seek(shift);
}

CUniVar getResultObject(bool resultFlag)
{
	CClassDataContainer* cdt = new CClassDataContainer();
	cdt->GetProperty("success") = resultFlag;
	return CUniVar(cdt);
}

CUniVar getFullResultObject(bool resultFlag)
{
	CClassDataContainer* cdt = new CClassDataContainer();
	cdt->GetProperty("success") = resultFlag;
	cdt->GetProperty("result") = resultFlag;
	return CUniVar(cdt);
}

CUniVar getResultObject(bool resultFlag, CUniVar& result)
{
	CClassDataContainer* cdt = new CClassDataContainer();
	cdt->GetProperty("success") = resultFlag;
	cdt->GetProperty("result") = result;
	return CUniVar(cdt);
}

CUniVar getResultObjectV(bool resultFlag, CUniVar result)
{
	CClassDataContainer* cdt = new CClassDataContainer();
	cdt->GetProperty("success") = resultFlag;
	cdt->GetProperty("result") = result;
	return CUniVar(cdt);
}

std::string getCurrentMethodName()
{
	std::string name = "";
	if (currentClass != 0xFFFFFFFF)
	{
		name = classesNames[currentClass];
		name += "::";
	}
	name += methodNames[currentMethod];
	return name;
}

void unset(CUniVar& a)
{
	a.SwitchToUnknownState();
}

CUniVar typeof(CUniVar& a)
{
	return a.GetTypeAsString();
}