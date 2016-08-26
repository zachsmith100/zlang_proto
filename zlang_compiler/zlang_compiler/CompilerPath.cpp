
#include "Common.h"
#include "CompilerPath.h"
#include "Utils.h"
#include <assert.h>
#include <string.h>

CCompilerPath::CCompilerPath()
{

}

CCompilerPath::CCompilerPath(const CCompilerPath& other)
{
	assert(false);
}

std::string CCompilerPath::Get()
{
	return GetInstance()->m_path;
}

void CCompilerPath::InitInstance(int argc, char* argv[])
{	
	for (int i = 1; i < argc; i++)
	{
		char* str = argv[i];
		if (str[strlen(str) - 1] == ' ')
		{
			str[strlen(str) - 1] = 0;
		}
	}

	CPath path(argv[0]);
	GetInstance()->Init(path.GetDirectory());
}
	
CCompilerPath* CCompilerPath::GetInstance()
{
	static CCompilerPath* obj = NULL;

	if (!obj)
	{
		obj = new CCompilerPath();
	}

	return obj;
}

void CCompilerPath::Init(const std::string& path)
{
	m_path = path;
}

void CCompilerPath::operator =(const CCompilerPath& other)
{
	assert(false);
}

CCompilerPath::~CCompilerPath()
{
}
