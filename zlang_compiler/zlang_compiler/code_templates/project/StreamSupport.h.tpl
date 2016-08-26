
#pragma once

#include "UniVar.h"
#include "ClassSupport.h"

enum StreamType
{
	STREAM_UNKNOWN,
	STREAM_TEXT,
	STREAM_BINARY,
	STREAM_XML,
	STREAM_INI,
	STREAM_JSON
};

class CStreamDataContainer : public CClassDataContainer
{
public:
	CStreamDataContainer(const std::string& path, const std::string& type, const bool createIfNotExists);
	virtual ~CStreamDataContainer();

	CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	CUniVar& GetProperty__(const std::string& name);

	CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus);

	std::string GetFileMode(const bool isFullAccess);
	void OpenStream();	

	bool IsOpened()const;
	bool IsWriteable()const;
	FILE* GetFileHandle();

	StreamType GetType()const;
	
	bool Clear();
	bool Read(CUniVar& dest, CUniVar& byteCount);
	bool Print(CUniVar dest, CUniVar& byteCount);
	bool PrintLn(CUniVar dest);
	bool Seek(CUniVar& shift);

	std::string ToString()const;

private:	
	CUniVar GetFileSizeProperty()const;
	CUniVar GetPositionProperty()const;

	StreamType DetectType(const std::string& type);
	std::string GetTypeAsString()const;

	bool ReadText(CUniVar& dest);
	bool ReadBinary(CUniVar& dest, CUniVar& byteCount);
	bool ReadXml(CUniVar& dest);
	bool ReadIni(CUniVar& dest);
	bool ReadJson(CUniVar& dest);

	bool PrintText(CUniVar& dest);
	bool PrintBinary(CUniVar& dest, CUniVar& byteCount);
	bool PrintJson(CUniVar& dest);
	bool PrintXml(CUniVar& dest);
	bool PrintIni(CUniVar& dest);

	CUniVar m_tempObj;
	StreamType m_type;
	bool m_writeable;
	std::string m_path;
	FILE* m_file;
	bool m_createIfNotExists;
};