
#include "Log.h"

FILE* OpenFile(const std::string& fileName, const std::string& mode)
{
#ifdef WIN32
	FILE* fl;
	fopen_s(&fl, fileName.c_str(), mode.c_str());
	return fl;
#else
	return fopen(fileName.c_str(), mode.c_str());
#endif
}

CLog::CLog():
	m_file(NULL)
{

}

void CLog::SetPath(const std::string& filePath)
{
	m_file = OpenFile(filePath, "w");
}

void CLog::AddLine(const std::string& str)
{
	if (m_file)
	{
		fprintf(m_file, "%s\n", str.c_str());
	}
}

void CLog::AddLine1s(const std::string& str, CUniVar param1)
{
	if (m_file)
	{
		fprintf(m_file, str.c_str(), param1.ToString().c_str());
		fprintf(m_file, "\n");
	}
}

void CLog::AddLine2s(const std::string& str, CUniVar param1, CUniVar param2)
{
	if (m_file)
	{
		fprintf(m_file, str.c_str(), param1.ToString().c_str(), param2.ToString().c_str());
		fprintf(m_file, "\n");
	}
}

void CLog::AddLine3s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3)
{
	if (m_file)
	{
		fprintf(m_file, str.c_str(), param1.ToString().c_str(), param2.ToString().c_str(), param3.ToString().c_str());
		fprintf(m_file, "\n");
	}
}

void CLog::AddLine4s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3, CUniVar param4)
{
	if (m_file)
	{
		fprintf(m_file, str.c_str(), param1.ToString().c_str(), param2.ToString().c_str(), param3.ToString().c_str(), param4.ToString().c_str());
		fprintf(m_file, "\n");
	}
}

void CLog::AddLine5s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3, CUniVar param4, CUniVar param5)
{
	if (m_file)
	{
		fprintf(m_file, str.c_str(), param1.ToString().c_str(), param2.ToString().c_str(), param3.ToString().c_str(), param4.ToString().c_str(), param5.ToString().c_str());
		fprintf(m_file, "\n");
	}
}

CLog::~CLog()
{
	if (m_file)
	{
		fclose(m_file);
	}
}