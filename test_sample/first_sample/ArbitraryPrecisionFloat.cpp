
#include "ArbitraryPrecisionFloat.h"
#include "UniVar.h"

#ifdef _WIN32
#define strcpy strcpy_s
#define strcat strcat_s
#define sprintf sprintf_s
#define itoa _itoa_s
#endif

const mpfr_rnd_t roundingStrategy = MPFR_RNDU;

#define MAX(a, b)						((a > b) ? a : b)
#define DECLARE_SIMPLE_RESULT			CArbitraryPrecisionFloat result(mpfr_get_prec(m_value));
#define DECLARE_MAX_PRECISION_RESULT	CArbitraryPrecisionFloat result(MAX(mpfr_get_prec(m_value), mpfr_get_prec(other.m_value)));

#define UPDATE_PRECISION_TO_MAX			int precision = mpfr_get_prec(m_value);	\
	int targetPrecision = MAX(precision, mpfr_get_prec(other.m_value)); \
	if (targetPrecision != precision)	ChangePrecision(targetPrecision);

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(unsigned int precision)
{	
	mpfr_init2(m_value, precision);
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const int other, unsigned int precision)
{	
	mpfr_init2(m_value, precision);
	mpfr_set_flt(m_value, (float)other, roundingStrategy);
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const CUniVar& other, unsigned int precision)
{
	if (other.GetType() == UV_INT)
	{
		mpfr_init2(m_value, precision);
		mpfr_set_flt(m_value, (float)other, roundingStrategy);
	}
	else if (other.GetType() == UV_DOUBLE)
	{
		mpfr_init2(m_value, precision);
		mpfr_set_d(m_value, (double)other, roundingStrategy);
	}
	else
	{
		mpfr_init2(m_value, precision);
		if (mpfr_set_str(m_value, other.ToString().c_str(), 10, roundingStrategy) != 0)
		{
			throw std::runtime_error("Attempt to create arbitrary precision float using non-numeric value as default value");
		}
	}
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const std::string& other, unsigned int precision)
{
	mpfr_init2(m_value, precision);
	if (mpfr_set_str(m_value, other.c_str(), 10, roundingStrategy) != 0)
	{
		throw std::runtime_error("Attempt to create arbitrary precision float using non-numeric value as default value");
	}
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const char* other, unsigned int precision)
{
	mpfr_init2(m_value, precision);
	if (mpfr_set_str(m_value, other, 10, roundingStrategy) != 0)
	{
		throw std::runtime_error("Attempt to create arbitrary precision float using non-numeric value as default value");
	}
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const CArbitraryPrecisionFloat& other)
{
	mpfr_init2(m_value, mpfr_get_prec(other.m_value));
	mpfr_set(m_value, other.m_value, roundingStrategy);
}

CArbitraryPrecisionFloat::CArbitraryPrecisionFloat(const double& other, unsigned int precision)
{
	mpfr_init2(m_value, precision);
	mpfr_set_d(m_value, other, roundingStrategy);
}

void CArbitraryPrecisionFloat::operator =(const int other)
{
	mpfr_set_flt(m_value, (float)other, roundingStrategy);
}

void CArbitraryPrecisionFloat::operator =(const double& other)
{
	mpfr_set_d(m_value, other, roundingStrategy);
}

void CArbitraryPrecisionFloat::operator =(const CArbitraryPrecisionFloat& other)
{
	mpfr_set(m_value, other.m_value, roundingStrategy);
}

void CArbitraryPrecisionFloat::ChangePrecision(const unsigned int targetPrecision)
{
	mpfr_t val;
	mpfr_init2(val, targetPrecision);
	mpfr_set(val, m_value, roundingStrategy);
	mpfr_set_prec(m_value, targetPrecision);
	mpfr_set(m_value, val, roundingStrategy);
	mpfr_clear(val);
}

void CArbitraryPrecisionFloat::operator +=(const CArbitraryPrecisionFloat& other)
{
	UPDATE_PRECISION_TO_MAX	
	mpfr_add(m_value, m_value, other.m_value, roundingStrategy);	
}

void CArbitraryPrecisionFloat::operator -=(const CArbitraryPrecisionFloat& other)
{
	UPDATE_PRECISION_TO_MAX
	mpfr_sub(m_value, m_value, other.m_value, roundingStrategy);	
}

void CArbitraryPrecisionFloat::operator /=(const CArbitraryPrecisionFloat& other)
{
	UPDATE_PRECISION_TO_MAX	
	mpfr_div(m_value, m_value, other.m_value, roundingStrategy);
}

void CArbitraryPrecisionFloat::operator *=(const CArbitraryPrecisionFloat& other)
{
	UPDATE_PRECISION_TO_MAX	
	mpfr_mul(m_value, m_value, other.m_value, roundingStrategy);
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::operator +(const CArbitraryPrecisionFloat& other)
{
	DECLARE_MAX_PRECISION_RESULT
	mpfr_add(result.m_value, m_value, other.m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::operator -(const CArbitraryPrecisionFloat& other)
{
	DECLARE_MAX_PRECISION_RESULT
	mpfr_sub(result.m_value, m_value, other.m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::operator /(const CArbitraryPrecisionFloat& other)
{
	DECLARE_MAX_PRECISION_RESULT
	mpfr_div(result.m_value, m_value, other.m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::operator *(const CArbitraryPrecisionFloat& other)
{
	DECLARE_MAX_PRECISION_RESULT
	mpfr_mul(result.m_value, m_value, other.m_value, roundingStrategy);
	return result;
}

bool CArbitraryPrecisionFloat::operator >(const int other)
{
	return mpfr_cmp_si(m_value, other) > 0;
}

bool CArbitraryPrecisionFloat::operator >(const double& other)
{
	return mpfr_cmp_d(m_value, other) > 0;
}

bool CArbitraryPrecisionFloat::operator >(const CArbitraryPrecisionFloat& other)
{
	return mpfr_cmp(m_value, other.m_value) > 0;
}

bool CArbitraryPrecisionFloat::operator >=(const int other)
{
	return mpfr_cmp_si(m_value, other) >= 0;
}

bool CArbitraryPrecisionFloat::operator >=(const double& other)
{
	return mpfr_cmp_d(m_value, other) >= 0;
}

bool CArbitraryPrecisionFloat::operator >=(const CArbitraryPrecisionFloat& other)
{
	return mpfr_cmp(m_value, other.m_value) >= 0;
}

bool CArbitraryPrecisionFloat::operator ==(const int other)
{
	return mpfr_cmp_si(m_value, other) == 0;
}

bool CArbitraryPrecisionFloat::operator ==(const double& other)
{
	return mpfr_cmp_d(m_value, other) == 0;
}

bool CArbitraryPrecisionFloat::operator ==(const CArbitraryPrecisionFloat& other)
{
	return mpfr_cmp(m_value, other.m_value) == 0;
}

bool CArbitraryPrecisionFloat::operator !=(const int other)
{
	return !operator ==(other);
}

bool CArbitraryPrecisionFloat::operator !=(const double& other)
{
	return !operator ==(other);
}

bool CArbitraryPrecisionFloat::operator !=(const CArbitraryPrecisionFloat& other)
{
	return !operator ==(other);
}

bool CArbitraryPrecisionFloat::operator <(const int other)
{
	return !operator >(other);
}

bool CArbitraryPrecisionFloat::operator <(const double& other)
{
	return !operator >(other);
}

bool CArbitraryPrecisionFloat::operator <(const CArbitraryPrecisionFloat& other)
{
	return !operator >(other);
}

bool CArbitraryPrecisionFloat::operator <=(const int other)
{
	return mpfr_cmp_si(m_value, other) <= 0;
}

bool CArbitraryPrecisionFloat::operator <=(const double& other)
{
	return mpfr_cmp_d(m_value, other) <= 0;
}

bool CArbitraryPrecisionFloat::operator <=(const CArbitraryPrecisionFloat& other)
{
	return mpfr_cmp(m_value, other.m_value) <= 0;
}

void CArbitraryPrecisionFloat::operator++(int)
{
	mpfr_add_si(m_value, m_value, 1, roundingStrategy);
}
	
void CArbitraryPrecisionFloat::operator--(int)
{
	mpfr_sub_si(m_value, m_value, 1, roundingStrategy);
}

std::string CArbitraryPrecisionFloat::ToString()
{
	return ToString(mpfr_get_prec(m_value));
}

std::string CArbitraryPrecisionFloat::ToString(const unsigned int afterPointPrecision)
{
	return ToString(afterPointPrecision, 0xFFFFFFFF);
}

std::string CArbitraryPrecisionFloat::ToString(const unsigned int afterPointPrecision, const unsigned int beforePointPrecision)
{
	char tmp[1000];
	char format[20];
	char prec[5];

	if (beforePointPrecision != 0xFFFFFFFF)
	{
		strcpy(format, "%");		
		sprintf(prec, "%d", beforePointPrecision + afterPointPrecision + 1);		
		strcat(format, prec);
		strcat(format, ".");
	}
	else
	{
		strcpy(format, "%.");
	}
	
	sprintf(prec, "%d", afterPointPrecision);			
	strcat(format, prec);
	strcat(format, "Rf");

	mpfr_sprintf(&tmp[0], format, m_value);

	return tmp;
}

double CArbitraryPrecisionFloat::ToDouble()
{
	return mpfr_get_d(m_value, roundingStrategy);
}

int CArbitraryPrecisionFloat::ToInt()
{
	return (int)mpfr_get_d(m_value, roundingStrategy);
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Abs()
{
	DECLARE_SIMPLE_RESULT
	mpfr_abs(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Sin()
{
	DECLARE_SIMPLE_RESULT
	mpfr_sin(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Cos()
{
	DECLARE_SIMPLE_RESULT
	mpfr_cos(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Tan()
{
	DECLARE_SIMPLE_RESULT
	mpfr_tan(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Ctg()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_tan(result.m_value, m_value, roundingStrategy);
	if (result != 0.0)
	{
		result = CArbitraryPrecisionFloat(1.0, mpfr_get_prec(m_value)) / result;
	}
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Pow(const CArbitraryPrecisionFloat& degree)
{
	CArbitraryPrecisionFloat result(MAX(mpfr_get_prec(m_value), mpfr_get_prec(degree.m_value)));
	mpfr_pow(result.m_value, m_value, degree.m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Log()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_log(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Log2()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_log2(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Log10()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_log10(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Exp()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_exp(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Exp2()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_exp2(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Exp10()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_exp10(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Sqrt()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_sqrt(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Asin()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_asin(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Acos()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_acos(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Atan()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_atan(result.m_value, m_value, roundingStrategy);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Actg()
{
	DECLARE_SIMPLE_RESULT	
	if (mpfr_cmp_si(m_value, 0))
	{
		result = 0;
	}
	else
	{
		result = CArbitraryPrecisionFloat(1.0, mpfr_get_prec(m_value)) / *this;
		mpfr_atan(result.m_value, result.m_value, roundingStrategy);		
	}
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Round()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_round(result.m_value, m_value);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Truncate()
{
	DECLARE_SIMPLE_RESULT
	mpfr_trunc(result.m_value, m_value);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Ceil()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_ceil(result.m_value, m_value);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Floor()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_floor(result.m_value, m_value);
	return result;
}

CArbitraryPrecisionFloat CArbitraryPrecisionFloat::Fraction()
{
	DECLARE_SIMPLE_RESULT	
	mpfr_frac(result.m_value, m_value, roundingStrategy);
	return result;
}

unsigned int CArbitraryPrecisionFloat::GetPrecision()
{
	return mpfr_get_prec(m_value);
}

CArbitraryPrecisionFloat::~CArbitraryPrecisionFloat()
{
	mpfr_clear(m_value);
}