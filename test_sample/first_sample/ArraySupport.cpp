
#include "ArraySupport.h"
#include "StandardFuncs.h"
#include "json.h"
#include <assert.h>
#include "LogManager.h"
#include <string.h>
#include "SymbolEnums.h"

CArrayDataContainer::CArrayDataContainer():
	m_length(0)
{
}

void CArrayDataContainer::Clear()
{
	for (unsigned int i = 0; i < m_length; i++)
	{
		CClassDataContainer::Remove(i);
	}
	m_length = 0;
}

CUniVar& CArrayDataContainer::GetElement(const unsigned int index)
{
	if (index < GetLength())
	{
		CUniVar indexName(index);
		return GetProperty__(indexName.ToString());
	}

	CUniVar indexName(index);
	throw std::runtime_error("Attempt to use out of range index for array element access");
	m_tempObj.SwitchToUnknownState();
	return m_tempObj;
}

unsigned int CArrayDataContainer::GetLength()const
{
	return m_length;
}

CUniVar& CArrayDataContainer::GetProperty__(const int callStatus, const ClassPropertyEnum propId)
{
	if (propId == __PROP_ENUM__length) 
	{
		m_tempObj = m_length;
		return m_tempObj;
	}

	return CClassDataContainer::GetProperty__(callStatus, propId);
}

CUniVar& CArrayDataContainer::GetProperty__(const std::string& name)
{
	if (name == "length")
	{
		m_tempObj = m_length;
		return m_tempObj;
	}

	return CClassDataContainer::GetProperty__(name);
}

CUniVar CArrayDataContainer::CallMethod__(int count, ...) 
{ 
	CUniVar result = getResultObject(false); 
	va_list vl; 
	va_start(vl, count); 
	result = CallMethod__(count, vl); 
	va_end(vl);	 
	return result; 
} 
 
CUniVar CArrayDataContainer::CallMethod__(int count, va_list& vl) 
{ 
	ClassMethodEnum method = (ClassMethodEnum)va_arg(vl, int); 
	bool defaultResultNeeded = (va_arg(vl, int) > 0);
	int callStatus = va_arg(vl, int);  
	return CallMethod__(count, vl, method, callStatus, defaultResultNeeded); 
}			 

CUniVar CArrayDataContainer::GetValue(const unsigned int index)
{
	CUniVar indexUV(index);
	return GetProperty__(indexUV.ToString());
}

void CArrayDataContainer::SetValue(const unsigned int index, const CUniVar& value)
{
	CUniVar indexUV(index);
	GetProperty__(indexUV.ToString()) = value;
}

CUniVar CArrayDataContainer::Insert(const CUniVar& _index, const CUniVar& value)
{
	unsigned int index = _index.ToUInt();	
	if (index > GetLength())
	{
		Log.Warnings.AddLine2s("Failed to insert array element because of position to insert is out of range after line %s in %s", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}

	CUniVar::PermitChangeType();
	
	for (unsigned int k = m_length; k > index; k--)
	{
		SetValue(k, GetValue(k - 1));
	}	

	SetValue(index, value);
	m_length++;

	CUniVar::ProhibitChangeType();

	return getFullResultObject(true);
}

CUniVar CArrayDataContainer::Clone()
{
	CArrayDataContainer* adt = new CArrayDataContainer();

	for (unsigned int i = 0; i < m_length; i++)
	{
		adt->Push(GetValue(i));
	}

	CUniVar res(adt);
	return getResultObject(true, res);
}

void CArrayDataContainer::Remove(const CUniVar index)
{
	Erase(index);
}

CUniVar CArrayDataContainer::Erase(const CUniVar& _index)
{
	unsigned int index = _index.ToUInt();	
	if (index >= GetLength())
	{
		Log.Warnings.AddLine2s("Failed to erase array elements because of position to remove is out of range after line %s in %s", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}

	CUniVar::PermitChangeType();
	m_length--;
	for (unsigned int k = index; k < m_length; k++)
	{
		SetValue(k, GetValue(k + 1));
	}
	CClassDataContainer::Remove(m_length);
	CUniVar::ProhibitChangeType();
	
	return getFullResultObject(true);
}

CUniVar CArrayDataContainer::Erase(const CUniVar& _index, const CUniVar& _size)
{
	unsigned int index = _index.ToUInt();	
	int size = _size.ToInt();
	if (index >= GetLength())
	{
		Log.Warnings.AddLine2s("Failed to erase array elements because of start position out of range after line %s in %s", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}
	if (size < 0)
	{
		Log.Warnings.AddLine2s("Failed to erase array elemets because of negative size of range to remove after line %s in %s", currentLine, getCurrentMethodName());
		return getFullResultObject(false);		
	}

	CUniVar::PermitChangeType();
	for (unsigned int k = index; k < m_length - size; k++)
	{
		SetValue(k, GetValue(k + size));
	}
	for (int i = 0; i < size; i++ )
	{
		m_length--;
		CClassDataContainer::Remove(m_length);
	}
	CUniVar::ProhibitChangeType();

	return getFullResultObject(true);
}

CUniVar CArrayDataContainer::Push(const CUniVar& value)
{
	CUniVar index(m_length);
	GetProperty__(index.ToString()) = value;
	m_length++;

	return getFullResultObject(true);
}

CUniVar CArrayDataContainer::Resize(const CUniVar& _size)
{	
	if (_size.ToInt() < 0)
	{
		Log.Warnings.AddLine2s("Failed to resize array because of negative new array size after line %s in %s", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}

	unsigned int size = _size.ToUInt();

	if (size > m_length)
	{
		for (unsigned int i = m_length; i < size; i++)
		{
			SetValue(i, 0);
		}		
	}

	m_length = size;
	return getFullResultObject(true);
}
 
CUniVar CArrayDataContainer::CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded) 
{ 
	CUniVar result = getResultObject(false); 
	if ((method == __FUNC_ENUM__resize) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = Resize(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__remove) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar::PermitChangeType();
		result = Erase(*arg0); 
		CUniVar::ProhibitChangeType();
	} 
	else if ((method == __FUNC_ENUM__remove) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		CUniVar::PermitChangeType();
		result = Erase(*arg0, *arg1); 
		CUniVar::ProhibitChangeType();
	} 
	else if ((method == __FUNC_ENUM__cut) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		CUniVar::PermitChangeType();
		result = Cut(*arg0, *arg1); 
		CUniVar::ProhibitChangeType();
	} 
	else if ((method == __FUNC_ENUM__implode) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 		
		result = Implode(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__paste) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		result = Insert(*arg0, *arg1); 
	} 
	else if ((method == __FUNC_ENUM__push) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = Push(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__indexOf) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = Find(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__indexFirstOf) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = FindFirstOf(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__indexLastOf) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = FindLastOf(*arg0); 
	} 
	else if ((method == __FUNC_ENUM__clone) && (count == 3) && (callStatus >= 0)) 
	{ 		
		result = Clone(); 
	} 
	else if ((method == __FUNC_ENUM__convertable) && (count == 4) && (callStatus >= 0)) 
	{ 		
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		result = CUniVar::IsConvertableTypeToType("array", arg0->ToString());		
	} 
	else if ((method == __FUNC_ENUM__removeAll) && (count == 3) && (callStatus >= 0)) 
	{ 		
		Clear(); 
		result = getFullResultObject(true);
	} 
	else if ((method == __FUNC_ENUM__extractNumeric) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		result = ExtractNumeric(*arg0, *arg1); 
	} 
	else if ((method == __FUNC_ENUM__extractString) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		result = ExtractString(*arg0, *arg1); 
	} 
	else if ((method == __FUNC_ENUM__extractBoolean) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		result = ExtractBoolean(*arg0, *arg1); 
	} 
	else if ((method == __FUNC_ENUM__extractFloat) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		result = ExtractFloat(*arg0, *arg1); 
	} 
	else if ((method == __FUNC_ENUM__binarizeNumeric) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		BinarizeNumeric(*arg0, *arg1); 
		result = getFullResultObject(true);
	} 
	else if ((method == __FUNC_ENUM__binarizeString) && (count == 4) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 		
		BinarizeString(*arg0); 
		result = getFullResultObject(true);
	} 
	else if ((method == __FUNC_ENUM__binarizeBoolean) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		BinarizeBoolean(*arg0, *arg1); 
		result = getFullResultObject(true);
	} 
	else if ((method == __FUNC_ENUM__binarizeFloat) && (count == 5) && (callStatus >= 0)) 
	{ 
		CUniVar* arg0 = va_arg(vl, CUniVar*); 
		CUniVar* arg1 = va_arg(vl, CUniVar*); 
		BinarizeFloat(*arg0, *arg1); 
		result = getFullResultObject(true);
	} 
	else if (count == 1) 
	{ 
		return GetProperty__(callStatus, (ClassPropertyEnum)method); 
	}			 
	else 
	{ 
		Log.Warnings.AddLine3s("Attempt to call unknown method %s after line %s in %s", methodNames[(int)method], currentLine, getCurrentMethodName()); 
	} 
	if (defaultResultNeeded) 
	{ 
		result.PermitChangeType();
		result = result.GetProperty__(1, __PROP_ENUM__result); 
		result.ProhibitChangeType();
	}
	return result; 
} 

bool CArrayDataContainer::IsMethodExist(ClassMethodEnum method)
{
	if ((method == __FUNC_ENUM__resize) || (method == __FUNC_ENUM__remove) ||
		(method == __FUNC_ENUM__cut) || (method == __FUNC_ENUM__implode) ||
		(method == __FUNC_ENUM__paste) || (method == __FUNC_ENUM__push) ||
		(method == __FUNC_ENUM__indexOf) || (method == __FUNC_ENUM__indexFirstOf) ||
		(method == __FUNC_ENUM__indexLastOf) || (method == __FUNC_ENUM__removeAll) ||
		(method == __FUNC_ENUM__extractNumeric) || (method == __FUNC_ENUM__extractString) ||
		(method == __FUNC_ENUM__extractBoolean) || (method == __FUNC_ENUM__extractFloat) ||
		(method == __FUNC_ENUM__binarizeNumeric) || (method == __FUNC_ENUM__binarizeString) ||
		(method == __FUNC_ENUM__binarizeBoolean) || (method == __FUNC_ENUM__binarizeFloat))
	{
		return true;
	}

	return false;
}

bool CArrayDataContainer::UpdateStateFromJson(const std::string& json)
{
	Json::Value root;
	Json::Reader reader;
	bool parseSuccess = reader.parse(json, root, false);

	if (!parseSuccess)
	{
		std::string errorText = reader.getFormattedErrorMessages();
		if (errorText.size() > 0)
		{
			Log.Warnings.AddLine2s("JSON parse error '" + errorText + "' for json string '" + json + "' after line %s in %s", currentLine, getCurrentMethodName());	
		}
		else
		{
			Log.Warnings.AddLine2s("Error on attempt to parse JSON string '" + json + "' after line %s in %s", currentLine, getCurrentMethodName());
		}
		return false;
	}	

	if (!root.isArray())
	{
		return false;
	}

	for (unsigned int i = 0; i < root.size(); i++)
	{
		Json::Value val = root[i];
		CUniVar index(i);
		
		HandleJSONElement(val, index.ToString());				
	}	

	m_length = root.size();

	return true;
}

Json::Value CArrayDataContainer::ToJsonValue()
{
	Json::Value root;	
	root.resize(m_length);

	for (unsigned int i = 0; i < m_length; i++)
	{
		root[i] = GetValue(i).ToJsonValue();
	}	

	return root;
}

bool CArrayDataContainer::IsEqual(CArrayDataContainer& adt)
{
	int length = GetLength();
	if (length != adt.GetLength())
	{
		return false;
	}

	for (int i = 0; i < length; i++)
	if (GetElement(i) != adt.GetElement(i))
	{
		return false;
	}

	return true;
}

CUniVar CArrayDataContainer::Find(CUniVar val)
{
	int pos = -1;

	for (unsigned int i = 0; i < GetLength(); i++)
	if ((GetElement(i).IsConvertableToTypeOf(&val)) && (GetElement(i) == val))
	{
		pos = i;
		break;
	}
		
	return getResultObjectV(pos != -1, CUniVar(pos));
}

CUniVar CArrayDataContainer::FindFirstOf(CUniVar val)
{
	return Find(val);
}

CUniVar CArrayDataContainer::FindLastOf(CUniVar val)
{
	int pos = -1;

	for (int i =  GetLength() - 1; i >= 0; i--)
	if (GetElement(i) == val)
	{
		pos = i;
		break;
	}
	
	return getResultObjectV(pos != -1, CUniVar(pos));
}

CUniVar CArrayDataContainer::Cut(CUniVar startPos, CUniVar size)
{
	int startPos_ = startPos.ToInt();
	int size_ = size.ToInt();

	if (startPos_ < 0)
	{
		Log.Warnings.AddLine2s("Cut range is incorrect after line %s in %s, start position must be positive", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}

	if (startPos_ + size_ > (int)GetLength())
	{
		Log.Warnings.AddLine2s("Cut range is incorrect after line %s in %s, too big size", currentLine, getCurrentMethodName());
		return getFullResultObject(false);
	}

	Erase(startPos + size, GetLength() - startPos - size);
	Erase(0, startPos);
	
	return getFullResultObject(true);
}

CUniVar CArrayDataContainer::Implode(CUniVar delim)
{
	if (GetLength() == 0)
	{
		return getResultObjectV(true, CUniVar(""));
	}

	std::string str = GetElement(0).ToString();
	for (unsigned int i = 1; i < GetLength(); i++)
	{
		str += delim.ToString();
		str	+= GetElement(i).ToString();
	}
	return getResultObjectV(true, CUniVar(str));
}

unsigned char* CArrayDataContainer::GetCharArray(CUniVar _startOffset, CUniVar _size)
{
	int startOffset = _startOffset.ToInt();
	int size = _size.ToInt();

	unsigned char* result = new unsigned char[size + 1];

	for (int i = startOffset, k = 0; i < startOffset + size; i++, k++)
	{
		result[k] = GetElement(i).ToSymbol();
	}

	return result;
}

bool CArrayDataContainer::IsArrayBlockValid(CUniVar _startOffset, CUniVar _size)
{
	int startOffset = _startOffset.ToInt();
	int size = _size.ToInt();

	if (startOffset < 0)
	{
		Log.Warnings.AddLine2s("Attempt to use negative offset of variable to construct from binary file array after line %s in %s", currentLine, getCurrentMethodName());
		return false;
	}

	if (size < 0)
	{
		Log.Warnings.AddLine2s("Attempt to use negative size of string variable to construct from binary file array after line %s in %s", currentLine, getCurrentMethodName());
		return false;
	}

	if ((unsigned int)(startOffset + size) > m_length)
	{
		Log.Warnings.AddLine2s("Attempt to use out of range block of binary file array to construct after line %s in %s", currentLine, getCurrentMethodName());
		return false;
	}

	return true;
}

CUniVar CArrayDataContainer::ExtractString(CUniVar startOffset, CUniVar size)
{
	if (!IsArrayBlockValid(startOffset, size))
	{
		return getResultObjectV(false, CUniVar(0));
	}

	unsigned char* data = GetCharArray(startOffset, size);
	data[size] = 0;
	CUniVar result((char*)data);		
	delete[] data;
	return  getResultObjectV(true, result);
}

CUniVar CArrayDataContainer::ExtractBoolean(CUniVar startOffset, CUniVar size)
{
	if (!IsArrayBlockValid(startOffset, size))
	{
		return getResultObjectV(false, CUniVar(0));
	}

	if ((sizeof(char) == size) || (sizeof(short) == size) || (sizeof(int) == size))
	{
		unsigned char* data = GetCharArray(startOffset, size);
		int value;
		switch (size)
		{
		case sizeof(char):
			{
				unsigned char* val = data;
				value = *val;
			}
			break;

		case sizeof(short):
			{
				unsigned short* val = (unsigned short*)data;
				value = *val;
			}
			break;

		case sizeof(int):
			{
				unsigned int* val = (unsigned int*)data;
				value = *val;
			}
			break;
		}
		delete[] data;
		return getResultObjectV(true, CUniVar(value != 0));
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to use unsupported size of boolean variable to construct from binary file array after line %s in %s", currentLine, getCurrentMethodName());
		return getResultObjectV(false, CUniVar(0));
	}
}

CUniVar CArrayDataContainer::ExtractFloat(CUniVar startOffset, CUniVar size)
{
	if (!IsArrayBlockValid(startOffset, size))
	{
		return getResultObjectV(false, CUniVar(0));
	}

	if ((sizeof(float) == size) || (sizeof(double) == size))
	{
		unsigned char* data = GetCharArray(startOffset, size);
		float value;
		switch (size)
		{
		case sizeof(float):
			{
				float* val = (float*)data;
				value = *val;
			}
			break;

		case sizeof(double):
			{
				double* val = (double*)data;
				value = (float)*val;
			}
			break;
		}
		delete[] data;
		return getResultObjectV(true, CUniVar(value));
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to use unsupported size of float variable to construct from binary file array after line %s in %s", currentLine, getCurrentMethodName());
		return getResultObjectV(false, CUniVar(0));
	}
}

void CArrayDataContainer::Binarize(const char* const str, const int size)
{
	for (int i = 0; i < size; i++)
	{
		int ch = str[i];
		CUniVar symbol(ch);
		Push(symbol);
	}
}

void CArrayDataContainer::BinarizeNumeric(CUniVar var, CUniVar _size)
{
	int intVar = var.ToInt();
	int size = _size.ToInt();	
	if ((size == sizeof(char)) || (size == sizeof(short)) || (size == sizeof(int)))
	{
		Binarize((char*)&intVar, size);
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to add numeric value of unsupported size to binary array after line %s in %s", currentLine, getCurrentMethodName());
	}
}

void CArrayDataContainer::BinarizeFloat(CUniVar var, CUniVar _size)
{
	int size = _size.ToInt();	
	if (size == sizeof(double))
	{
		double dblVar = var.ToDouble();	
		Binarize((char*)&dblVar, size);
	}
	else if (size == sizeof(float))
	{
		float dblVar = (float)var.ToDouble();	
		Binarize((char*)&dblVar, size);
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to add float value of unsupported size to binary array after line %s in %s", currentLine, getCurrentMethodName());
	}
}

void CArrayDataContainer::BinarizeString(CUniVar _str)
{
	std::string str = _str.ToString();
	Binarize(str.c_str(), str.size());
}

void CArrayDataContainer::BinarizeBoolean(CUniVar var, CUniVar _size)
{
	int size = _size.ToInt();	
	if ((size == sizeof(char)) || (size == sizeof(short)) || (size == sizeof(int)))
	{
		int intVar = var.ToBoolean() ? 1 : 0;
		BinarizeNumeric(intVar, _size);
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to add boolean value of unsupported size to binary array after line %s in %s", currentLine, getCurrentMethodName());
	}
}

CUniVar CArrayDataContainer::ExtractNumeric(CUniVar startOffset, CUniVar size)
{
	if (!IsArrayBlockValid(startOffset, size))
	{
		return getResultObjectV(false, CUniVar(0));
	}

	if ((sizeof(char) == size) || (sizeof(short) == size) || (sizeof(int) == size))
	{
		unsigned char* data = GetCharArray(startOffset, size);
		int value;
		switch (size)
		{
		case sizeof(char):
			{
				unsigned char* val = data;
				value = *val;
			}
			break;

		case sizeof(short):
			{
				unsigned short* val = (unsigned short*)data;
				value = *val;
			}
			break;

		case sizeof(int):
			{
				unsigned int* val = (unsigned int*)data;
				value = *val;
			}
			break;
		}
		delete[] data;
		return getResultObjectV(true, CUniVar(value));
	}
	else
	{
		Log.Warnings.AddLine2s("Attempt to use unsupported size of numeric variable to construct from binary file array after line %s in %s", currentLine, getCurrentMethodName());
		return getResultObjectV(false, CUniVar(0));
	}
}

CArrayDataContainer::~CArrayDataContainer()
{
}

#define OF_METHOD_IMPL(varType, convFunc) std::vector<varType> result; if (m_adt)	for (unsigned int i = 0; i < m_adt->GetLength(); i++)	result.push_back(m_adt->GetElement(i).convFunc()); return result;

CVectorConvertor::CVectorConvertor(CArrayDataContainer* adt):
	m_adt(adt)
{
}

std::vector<std::string> CVectorConvertor::OfString()const
{
	OF_METHOD_IMPL(std::string, ToString)	
}

std::vector<int> CVectorConvertor::OfInt()const
{
	OF_METHOD_IMPL(int, ToInt)
}

std::vector<unsigned int> CVectorConvertor::OfUInt()const
{
	OF_METHOD_IMPL(unsigned int, ToUInt)
}

std::vector<double> CVectorConvertor::OfDouble()const
{
	OF_METHOD_IMPL(double, ToDouble)
}

std::vector<bool> CVectorConvertor::OfBoolean()const
{
	OF_METHOD_IMPL(bool, ToBoolean)
}

std::vector<char> CVectorConvertor::OfSymbol()const
{
	OF_METHOD_IMPL(char, ToSymbol)
}