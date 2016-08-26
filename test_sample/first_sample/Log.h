
#pragma once

#include <string>
#include "UniVar.h"

class CLog
{
public:
	CLog();
	~CLog();

	void SetPath(const std::string& filePath);

	void AddLine(const std::string& str);
	void AddLine1s(const std::string& str, CUniVar param1);
	void AddLine2s(const std::string& str, CUniVar param1, CUniVar param2);
	void AddLine3s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3);
	void AddLine4s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3, CUniVar param4);
	void AddLine5s(const std::string& str, CUniVar param1, CUniVar param2, CUniVar param3, CUniVar param4, CUniVar param5);

private:
	FILE* m_file;
};

FILE* OpenFile(const std::string& fileName, const std::string& mode);