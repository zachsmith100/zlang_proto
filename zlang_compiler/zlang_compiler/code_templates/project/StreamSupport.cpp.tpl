
#include "StreamSupport.h"
#include "SymbolEnums.h"
#include "LogManager.h"
#include <assert.h>
#include <stdio.h>
#include "ArraySupport.h"
#include "HashtableSupport.h"
#ifdef WIN32
#include <io.h>
#else
#define _fileno fileno
#include <unistd.h>
#define _chsize ftruncate
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include "tinyxml2.h"
#include "minIni.h"
#include "StandardFuncs.h"

static char globalBuffer[65535];

using namespace tinyxml2;

CStreamDataContainer::CStreamDataContainer(const std::string& path, const std::string& type, const bool createIfNotExists):
	m_file(NULL),
	m_path(path),
	m_writeable(false)
{	
	m_type = DetectType(type);	
	m_createIfNotExists = createIfNotExists;

	if (m_type != STREAM_UNKNOWN)
	{
		OpenStream();
	}
	else
	{
		Log.Warnings.AddLine4s("Cannot open stream %s because of unknown type %s after line %s in %s", path, type, currentLine, getCurrentMethodName());
	}
}

std::string CStreamDataContainer::GetFileMode(const bool isFullAccess)
{
	if (m_type == STREAM_BINARY)
	{
		return isFullAccess ? (m_createIfNotExists ? "w+b" : "a+b") : "rb";
	}
	else
	{
		return isFullAccess ? "a+" : "r";
	}
}

void CStreamDataContainer::OpenStream()
{
	m_file = OpenFile(m_path, "r");

	if ((!m_createIfNotExists) && (!m_file))
	{
		Log.Warnings.AddLine3s("Cannot open stream %s after line %s in %s", m_path, currentLine, getCurrentMethodName());
		return;
	}

	if (m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	if ((m_type == STREAM_XML) || (m_type == STREAM_INI))
	{
		return;
	}

	m_file = OpenFile(m_path.c_str(), GetFileMode(true).c_str());
	m_writeable = true;
	if (!m_file)
	{
		m_file = OpenFile(m_path.c_str(), GetFileMode(false).c_str());
		m_writeable = false;
		if (!m_file)
		{
			Log.Warnings.AddLine3s("Cannot open stream %s after line %s in %s", m_path, currentLine, getCurrentMethodName());
		}
	}

	if (m_file)
	{
		rewind(m_file);
	}
}
 

StreamType CStreamDataContainer::DetectType(const std::string& type)
{
	if (type == "text")
	{
		return STREAM_TEXT;
	}
	else if (type == "binary")
	{
		return STREAM_BINARY;
	}
	else if (type == "xml")
	{
		return STREAM_XML;
	}
	else if (type == "ini")
	{
		return STREAM_INI;
	}
	else if (type == "json")
	{
		return STREAM_JSON;
	}

	return STREAM_UNKNOWN;
}

std::string CStreamDataContainer::GetTypeAsString()const
{
	if (m_type == STREAM_TEXT)
	{
		return "text";
	}
	else if (m_type == STREAM_BINARY)
	{
		return "binary";
	}
	else if (m_type == STREAM_XML)
	{
		return "xml";
	}
	else if (m_type == STREAM_INI)
	{
		return "ini";
	}
	else if (m_type == STREAM_JSON)
	{
		return "json";
	}

	return "";
}

CUniVar CStreamDataContainer::GetFileSizeProperty()const
{
	struct stat fileInfo;
	bool isFileValid = (stat(m_path.c_str(), &fileInfo) == 0);
	CUniVar res = isFileValid ? fileInfo.st_size : 0;	
	if ((!m_file) && (!isFileValid))
	{
		Log.Warnings.AddLine3s("Attempt to get size property of unopened file %s after line %s in %s", m_path, currentLine, getCurrentMethodName());
	}	
	return res;
}

CUniVar CStreamDataContainer::GetPositionProperty()const
{
	if (m_file)
	{
		return ftell(m_file);
	}
	else
	{		
		Log.Warnings.AddLine3s("Attempt to get position property of unopened file %s after line %s in %s", m_path, currentLine, getCurrentMethodName());
		return 0;
	}
}

CUniVar& CStreamDataContainer::GetProperty__(const int callStatus, const ClassPropertyEnum propId)
{
	if (propId == __PROP_ENUM__writeable)
	{
		m_tempObj = m_writeable;		
	}
	else if (propId == __PROP_ENUM__opened)
	{
		m_tempObj = (m_file != NULL);
	}
	else if (propId == __PROP_ENUM__path)
	{
		m_tempObj = m_path;
	}
	else if (propId == __PROP_ENUM__type)
	{
		m_tempObj = GetTypeAsString();
	}
	else if (propId == __PROP_ENUM__size)
	{
		m_tempObj = GetFileSizeProperty();
	}
	else if (propId == __PROP_ENUM__position)
	{
		m_tempObj = GetPositionProperty();		
	}

	return m_tempObj;
}

CUniVar& CStreamDataContainer::GetProperty__(const std::string& name)
{
	if (name == "writeable")
	{
		m_tempObj = m_writeable;		
	}
	else if (name == "opened")
	{
		m_tempObj = (m_file != NULL);
	}
	else if (name == "path")
	{
		m_tempObj = m_path;
	}
	else if (name == "type")
	{
		m_tempObj = GetTypeAsString();
	}
	else if (name == "size")
	{
		m_tempObj = GetFileSizeProperty();		
	}
	else if (name == "position")
	{
		m_tempObj = GetPositionProperty();		
	}

	return m_tempObj;
}

CUniVar CStreamDataContainer::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus)
{
	Log.Warnings.AddLine2s("Attempt to call method of stream after line %s in %s. Stream has no methods.", currentLine, getCurrentMethodName());
	return false;
}

bool CStreamDataContainer::IsOpened()const
{
	return (m_file != NULL);
}
	
bool CStreamDataContainer::IsWriteable()const
{
	return m_writeable;
}

FILE* CStreamDataContainer::GetFileHandle()
{
	return m_file;
}

bool CStreamDataContainer::Clear()
{
	if (!m_file)
	{
		return false;
	}

	int fileId = _fileno(m_file);
    _chsize(fileId, 0);	
	return true;
}

bool CStreamDataContainer::ReadText(CUniVar& dest)
{
	if (feof(m_file))
	{
		return false;
	}

	if (!fgets(globalBuffer, sizeof(globalBuffer), m_file))
	{
		return false;
	}

	while ((strlen(globalBuffer) != 0) && ((globalBuffer[strlen(globalBuffer) - 1] == 10) || (globalBuffer[strlen(globalBuffer) - 1] == 13)))
	{
		globalBuffer[strlen(globalBuffer) - 1] = 0;
	}

	dest = &globalBuffer[0];
	return true;
}

bool CStreamDataContainer::ReadBinary(CUniVar& dest, CUniVar& _byteCount)
{
	int byteCount = _byteCount.ToInt();
	if (byteCount <= 0)
	{
		return false;
	}

	unsigned char* buf = new unsigned char[byteCount];
	byteCount = fread(buf, 1, byteCount, m_file);

	CArrayDataContainer* adt = new CArrayDataContainer();

	for (int i = 0; i < byteCount; i++)
	{
		adt->Push(buf[i]);
	}

	dest = CUniVar(adt);
	delete[] buf;

	if (_byteCount.ToInt() != byteCount)
	{
		Log.Warnings.AddLine5s("Attempt to read %s bytes from file %s, but read with success only %s bytes after line %s in %s", _byteCount, m_path, byteCount, currentLine, getCurrentMethodName());
	}

	_byteCount = byteCount;

	return (byteCount != 0);
}

bool CStreamDataContainer::ReadXml(CUniVar& dest)
{
	XMLDocument doc;
	XMLError err = doc.LoadFile(m_path.c_str());
	if (err != XML_SUCCESS)
	{
		std::string errorText = doc.ErrorName();
		Log.Warnings.AddLine4s("XML parse error (%s) for file %s after line %s in %s", errorText, m_path, currentLine, getCurrentMethodName());
		return false;
	}

	dest.UpdateStateFromXml(doc.RootElement(), true);	

	return true;
}

int IniCallback(const char* section, const char* key, const char* value, const void* userdata)
{
	CHashtableDataContainer* hdt = (CHashtableDataContainer*)userdata;
	assert(hdt);
	if (!hdt->KeyExists(section))
	{
		hdt->GetElement(section) = CUniVar(new CHashtableDataContainer());
	}

	CUniVar& sectionVar = hdt->GetElement(section);
	assert(sectionVar.GetType() == UV_HASHTABLE);
	CHashtableDataContainer* sectionHDT = sectionVar.ToHashtable();
	assert(sectionHDT);
	sectionHDT->GetElement(key) = value;

	return 1;
}

bool CStreamDataContainer::ReadIni(CUniVar& dest)
{
	CHashtableDataContainer* hdt = new CHashtableDataContainer();
	if (ini_browse(IniCallback, hdt, m_path.c_str()) == 0)
	{
		return false;
	}
	dest = CUniVar(hdt);
	return true;
}

bool CStreamDataContainer::ReadJson(CUniVar& dest)
{
	std::string str;
	fseek(m_file, 0, 0);
	while (!feof(m_file))
	{
		static char globalBuffer[65535];
		if (!fgets(globalBuffer, sizeof(globalBuffer), m_file))
		{
			break;
		}	
		str += globalBuffer;
	}
	
	return dest.UpdateStateFromJson(str, true);	
}

bool CStreamDataContainer::Read(CUniVar& dest, CUniVar& byteCount)
{
	if ((m_file) || (m_type == STREAM_XML) || (m_type == STREAM_INI))
	{
		switch (m_type)
		{
		case STREAM_TEXT:
			return ReadText(dest);
		case STREAM_BINARY:
			return ReadBinary(dest, byteCount);			
		case STREAM_XML:
			return ReadXml(dest);			
		case STREAM_INI:
			return ReadIni(dest);			
		case STREAM_JSON:
			return ReadJson(dest);			
		default:
			assert(false);
			break;
		}
		
		return false;
	}
	else
	{
		Log.Warnings.AddLine3s("Attempt to read from file %s after line %s in %s. File is not opened", m_path, currentLine, getCurrentMethodName());
		return false;
	}
}

bool CStreamDataContainer::PrintText(CUniVar& dest)
{
	fputs(dest.ToString().c_str(), m_file);

	return true;
}

bool CStreamDataContainer::PrintBinary(CUniVar& dest, CUniVar& byteCount)
{
	if (dest.GetType() != UV_ARRAY)
	{
		throw std::runtime_error(("Attempt to write non-array to binary file " + m_path).c_str());
		return false;
	}

	CArrayDataContainer* adt = dest.ToArray();

	for (unsigned int i = 0; i < adt->GetLength(); i++)
	{
		unsigned char ch = adt->GetElement(i).ToSymbol();
		if (fwrite(&ch, 1, 1, m_file) != 1)
		{
			Log.Warnings.AddLine4s("Failed to write byte %s to binary file %s after line %s in %s", i, m_path, currentLine, getCurrentMethodName());
			return false;
		}
	}

	return true;
}

bool CStreamDataContainer::PrintJson(CUniVar& dest)
{
	Clear();
	fputs(dest.ToString().c_str(), m_file);
	return true;
}

bool CStreamDataContainer::PrintXml(CUniVar& dest)
{	
	FILE* m_file = OpenFile(m_path.c_str(), "w");
	if (!m_file)
	{
		return false;
	}
	fputs(dest.ToXml().c_str(), m_file);
	fclose(m_file);
	return true;
}

bool CStreamDataContainer::PrintIni(CUniVar& dest)
{	
	if ((dest.GetType() != UV_HASHTABLE) && (dest.GetType() != UV_CLASS))
	{		
		throw std::runtime_error("Hashtable or class instance must be used for print to ini-file.");
		return false;
	}

	std::vector<std::string> sectionNames;
	std::vector<CUniVar *> sectionVars;

	switch (dest.GetType())
	{
	case UV_HASHTABLE:
		dest.ToHashtable()->GetAssociativeContainers(sectionNames, sectionVars);
		break;

	case UV_CLASS:
		dest.ToClass()->GetAssociativeContainers(sectionNames, sectionVars);
		break;

	default:
		assert(false);
		break;
	}

	for (unsigned int i = 0; i < sectionNames.size(); i++)
	{
		std::string section = sectionNames[i];

		CUniVar* sectionContents = sectionVars[i];
		
		std::vector<std::string> propertyNames;
		std::vector<CUniVar *> propertyVars;

		switch (sectionContents->GetType())
		{
		case UV_HASHTABLE:
			sectionContents->ToHashtable()->GetSimpleProperties(propertyNames, propertyVars);
			break;

		case UV_CLASS:
			sectionContents->ToClass()->GetSimpleProperties(propertyNames, propertyVars);
			break;

		default:
			assert(false);
			break;
		}

		for (unsigned int k = 0; k < propertyNames.size(); k++)
		{
			if (ini_puts(section.c_str(), propertyNames[k].c_str(), propertyVars[k]->ToString().c_str(), m_path.c_str()) == 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool CStreamDataContainer::Print(CUniVar dest, CUniVar& byteCount)
{
	if (((m_file) && (m_writeable)) || (m_type == STREAM_XML) || (m_type == STREAM_INI))
	{
		switch (m_type)
		{
		case STREAM_TEXT:
			return PrintText(dest);
		case STREAM_BINARY:
			return PrintBinary(dest, byteCount);			
		case STREAM_XML:
			return PrintXml(dest);			
		case STREAM_INI:
			return PrintIni(dest);			
		case STREAM_JSON:
			return PrintJson(dest);			
		default:
			assert(false);
			break;
		}

		return false;
	}
	else
	{
		Log.Warnings.AddLine4s("Attempt to write %s to file %s after line %s in %s", dest.ToString(), m_path, currentLine, getCurrentMethodName());
		if (m_file)
		{
			Log.Warnings.AddLine2s("File is not writeable after line %s in %s", currentLine, getCurrentMethodName());
		}
		else
		{
			Log.Warnings.AddLine2s("File is not opened after line %s in %s", currentLine, getCurrentMethodName());
		}
		return false;
	}
}

bool CStreamDataContainer::PrintLn(CUniVar dest)
{
	CUniVar zero(0);
	Print(dest, zero);
			
	if ((m_file) && (m_writeable))
	{
		fprintf(m_file, "\n");
		return true;
	}	

	return false;
}

bool CStreamDataContainer::Seek(CUniVar& _shift)
{
	if (!m_file)
	{
		return false;
	}

	int shift = _shift.ToInt();
	bool res = true;
	if (0 == shift)
	{
		rewind(m_file);		
	}
	else if (m_type != STREAM_BINARY)
	{
		Log.Warnings.AddLine3s("Attempt to seek in non-binary file %s after line %s in %s!", m_path, currentLine, getCurrentMethodName());
		return false;
	}
	else if (shift > 0)
	{
		res = (fseek(m_file, shift, 0) == 0);
	}
	else if (shift < 0)
	{
		res = (fseek(m_file, -shift, 2) == 0);
	}

	if (!res)
	{
		Log.Warnings.AddLine4s("Failed to seek file %s to position %s after line %s in %s!", m_path, _shift, currentLine, getCurrentMethodName());
	}

	return res;
}

StreamType CStreamDataContainer::GetType()const
{
	return m_type;
}

std::string CStreamDataContainer::ToString()const
{
	Json::Value root;		
	root["writeable"] = m_writeable;
	root["opened"] = (m_file != NULL); 
	root["path"] = m_path; 
	root["type"] = GetTypeAsString(); 
	root["size"] = GetFileSizeProperty().ToInt();
	root["position"] = GetPositionProperty().ToInt();
	return root.toStyledString();
}

CStreamDataContainer::~CStreamDataContainer()
{
	if (m_file != NULL)
	{
		fclose(m_file);
	}
}
