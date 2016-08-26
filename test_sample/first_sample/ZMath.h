
#pragma once

#include "UniVar.h"
#include "ClassSupport.h"

class Number : public CClassDataContainer
{
public:
	static CUniVar Abs(CUniVar* num);
	static CUniVar Sin(CUniVar* num);
	static CUniVar Cos(CUniVar* num);
	static CUniVar Tan(CUniVar* num);
	static CUniVar Ctg(CUniVar* num);
	static CUniVar Pow(CUniVar* num, CUniVar* degree);
	static CUniVar Log(CUniVar* num);	
	static CUniVar Log10(CUniVar* num);
	static CUniVar Exp(CUniVar* num);	
	static CUniVar Sqrt(CUniVar* num);
	static CUniVar Asin(CUniVar* num);
	static CUniVar Acos(CUniVar* num);
	static CUniVar Atan(CUniVar* num);
	static CUniVar Actg(CUniVar* num);
	static CUniVar Round(CUniVar* num);
	static CUniVar Truncate(CUniVar* num);
	static CUniVar Ceil(CUniVar* num);
	static CUniVar Floor(CUniVar* num);
	static CUniVar Fraction(CUniVar* num);
	static CUniVar GetPrecision(CUniVar* num);
	static CUniVar PI(CUniVar* num);
	static Number* GetInstance();	

private:
	Number();
	Number(const Number& other);
	void operator =(const Number& other);
	~Number();
};