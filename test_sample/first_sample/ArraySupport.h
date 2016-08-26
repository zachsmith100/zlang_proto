
#pragma once

#include "UniVar.h"
#include "ClassSupport.h"

class CArrayDataContainer : public CClassDataContainer
{
public:
	CArrayDataContainer();
	virtual ~CArrayDataContainer();

	CUniVar& GetElement(const unsigned int index);
	unsigned int GetLength()const;

	CUniVar& GetProperty__(const int callStatus, const ClassPropertyEnum propId);		
	CUniVar& GetProperty__(const std::string& name);

	CUniVar CallMethod__(int count, ...);
	CUniVar CallMethod__(int count, va_list& vl);
	CUniVar CallMethod__(int count, va_list& vl, ClassMethodEnum method, int callStatus, bool defaultResultNeeded);
		
	CUniVar GetValue(const unsigned int index);
	void SetValue(const unsigned int index, const CUniVar& value);
	
	CUniVar Insert(const CUniVar& index, const CUniVar& value);
	CUniVar Erase(const CUniVar& index);
	CUniVar Erase(const CUniVar& _index, const CUniVar& _size);
	CUniVar Push(const CUniVar& value);
	CUniVar Resize(const CUniVar& size);

	CUniVar Find(CUniVar val);
	CUniVar FindFirstOf(CUniVar val);
	CUniVar FindLastOf(CUniVar val);
	CUniVar Cut(CUniVar startPos, CUniVar size);
	CUniVar Implode(CUniVar delim);

	CUniVar Clone();

	bool UpdateStateFromJson(const std::string& json);

	virtual Json::Value ToJsonValue();

	bool IsEqual(CArrayDataContainer& adt);

	void Clear();

	CUniVar ExtractNumeric(CUniVar startOffset, CUniVar size);
	CUniVar ExtractString(CUniVar startOffset, CUniVar size);
	CUniVar ExtractBoolean(CUniVar startOffset, CUniVar size);
	CUniVar ExtractFloat(CUniVar startOffset, CUniVar size);

	void Binarize(const char* const str, const int size);
	void BinarizeNumeric(CUniVar var, CUniVar _size);
	void BinarizeFloat(CUniVar var, CUniVar _size);
	void BinarizeString(CUniVar _str);
	void BinarizeBoolean(CUniVar var, CUniVar _size);

	void Remove(const CUniVar index);

	static bool IsMethodExist(ClassMethodEnum method);

private:
	unsigned char* GetCharArray(CUniVar _startOffset, CUniVar _size);

	bool IsArrayBlockValid(CUniVar _startOffset, CUniVar _size);

	unsigned int m_length;
	CUniVar m_tempObj;
};

class ArrayProto : public CArrayDataContainer
{
};

class Array : public CUniClass<ArrayProto>  
{ 
public:
	Array():
		CUniClass<ArrayProto>(false)
	{
	}
};

class CVectorConvertor
{
public:
	CVectorConvertor(CArrayDataContainer* adt);

	std::vector<std::string> OfString()const;
	std::vector<int> OfInt()const;
	std::vector<unsigned int> OfUInt()const;
	std::vector<double> OfDouble()const;
	std::vector<bool> OfBoolean()const;
	std::vector<char> OfSymbol()const;	

private:
	CArrayDataContainer* m_adt;
};

template <class T>
CUniVar ConvertTo(const std::vector<T>& vec)
{
	CArrayDataContainer* adt = new CArrayDataContainer();

	typename std::vector<T>::const_iterator it = vec.begin();
	typename std::vector<T>::const_iterator itEnd = vec.end();

	for ( ; it != itEnd; it++)
	{
		adt->Push(CUniVar(*it));
	}

	return CUniVar(adt);
}

template <class T>
CUniVar ConvertTo(const T* const arr, const unsigned int size)
{
	CArrayDataContainer* adt = new CArrayDataContainer();

	for (unsigned int i = 0; i < size; i++)
	{
		adt->Push(CUniVar(arr[i]));
	}

	return CUniVar(adt);
}