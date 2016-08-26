
#pragma once

#include <cstddef>
#include "gmp.h"
#include "mpfr.h"
#include <string>

class CUniVar;

class CArbitraryPrecisionFloat
{
public:
	CArbitraryPrecisionFloat(const char* other, unsigned int precision);
	CArbitraryPrecisionFloat(const std::string& other, unsigned int precision);
	CArbitraryPrecisionFloat(const int other, unsigned int precision);	
	CArbitraryPrecisionFloat(const double& other, unsigned int precision);
	CArbitraryPrecisionFloat(const CArbitraryPrecisionFloat& other);
	CArbitraryPrecisionFloat(const CUniVar& other, unsigned int precision);
	~CArbitraryPrecisionFloat();

	bool operator >(const int other);
	bool operator >(const double& other);
	bool operator >(const CArbitraryPrecisionFloat& other);

	bool operator >=(const int other);
	bool operator >=(const double& other);
	bool operator >=(const CArbitraryPrecisionFloat& other);

	bool operator ==(const int other);
	bool operator ==(const double& other);
	bool operator ==(const CArbitraryPrecisionFloat& other);

	bool operator !=(const int other);
	bool operator !=(const double& other);
	bool operator !=(const CArbitraryPrecisionFloat& other);

	bool operator <(const int other);
	bool operator <(const double& other);
	bool operator <(const CArbitraryPrecisionFloat& other);

	bool operator <=(const int other);
	bool operator <=(const double& other);
	bool operator <=(const CArbitraryPrecisionFloat& other);

	void operator =(const int other);	
	void operator =(const double& other);
	void operator =(const std::string& other);
	void operator =(const CArbitraryPrecisionFloat& other);
	
	void operator +=(const CArbitraryPrecisionFloat& other);	
	void operator -=(const CArbitraryPrecisionFloat& other);	
	void operator /=(const CArbitraryPrecisionFloat& other);
	void operator *=(const CArbitraryPrecisionFloat& other);

	void operator++(int);
	void operator--(int);

	CArbitraryPrecisionFloat operator +(const CArbitraryPrecisionFloat& other);
	CArbitraryPrecisionFloat operator -(const CArbitraryPrecisionFloat& other);
	CArbitraryPrecisionFloat operator /(const CArbitraryPrecisionFloat& other);
	CArbitraryPrecisionFloat operator *(const CArbitraryPrecisionFloat& other);

	std::string ToString();
	std::string ToString(const unsigned int afterPointPrecision);
	std::string ToString(const unsigned int afterPointPrecision, const unsigned int beforePointPrecision);

	double ToDouble();
	int ToInt();	

	void ChangePrecision(const unsigned int targetPrecision);
	unsigned int GetPrecision();

	CArbitraryPrecisionFloat Abs();
	CArbitraryPrecisionFloat Sin();
	CArbitraryPrecisionFloat Cos();
	CArbitraryPrecisionFloat Tan();
	CArbitraryPrecisionFloat Ctg();
	CArbitraryPrecisionFloat Pow(const CArbitraryPrecisionFloat& degree);
	CArbitraryPrecisionFloat Log();
	CArbitraryPrecisionFloat Log2();
	CArbitraryPrecisionFloat Log10();
	CArbitraryPrecisionFloat Exp();
	CArbitraryPrecisionFloat Exp2();
	CArbitraryPrecisionFloat Exp10();
	CArbitraryPrecisionFloat Sqrt();
	CArbitraryPrecisionFloat Asin();
	CArbitraryPrecisionFloat Acos();
	CArbitraryPrecisionFloat Atan();
	CArbitraryPrecisionFloat Actg();
	CArbitraryPrecisionFloat Round();
	CArbitraryPrecisionFloat Truncate();
	CArbitraryPrecisionFloat Ceil();
	CArbitraryPrecisionFloat Floor();
	CArbitraryPrecisionFloat Fraction();

private:
	CArbitraryPrecisionFloat(unsigned int precision);

	mpfr_t m_value;
};