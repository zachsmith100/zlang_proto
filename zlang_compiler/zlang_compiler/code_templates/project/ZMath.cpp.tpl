
#include "ZMath.h"
#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h> 

CUniVar GetPI(UniVarType tp, unsigned int width = DEFAULT_PRECISION)
{
	if ((tp == UV_DOUBLE) || (tp == UV_INT))
	{
		return M_PI;
	}
	else if (tp == UV_ARB_DOUBLE)
	{
		static std::string piStr = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";
		return CUniVar(CArbitraryPrecisionFloat(piStr.substr(0, width + 2), width));
	}

	assert(false);
	return CUniVar(0);
}

int zround(const double& x) 
{ 
	return (int)floor(x + 0.5); 
}

int ztrunc(const double& x)
{
	return (int)x;
}

double zfrac(const double& x)
{
     return x - floor(x);
}

#define INIT_BY_DEFAULT(num) if (num->GetType() == UV_UNKNOWN)	\
    { \
		if (DEFAULT_DOUBLE_TYPE == UV_DOUBLE) \
		{ \
			*num = CUniVar(0.0); \
		} \
		else \
		{ \
			*num = CUniVar(CArbitraryPrecisionFloat(0.0, DEFAULT_PRECISION)); \
		} \
    } 

#define IMPLEMENT_RAW_MATH_OP_EXT(intOp, intType, dblOp, arbDblOp) switch (num->GetType()) \
	{ \
	case UV_INT: \
		return CUniVar(intOp((intType)num->ToInt())); \
		break; \
	case UV_DOUBLE: \
		return CUniVar(dblOp(num->ToDouble())); \
		break; \
	case UV_ARB_DOUBLE: \
		return CUniVar(num->ToArbDouble().arbDblOp()); \
		break; \
	default: \
		if (num->IsConvertableToType(UV_ARB_DOUBLE)) \
		{ \
			return num->ToArbDouble().arbDblOp(); \
		} \
		else \
		{ \
			throw std::runtime_error("Failed to convert to numeric"); \
		} \
		break; \
	}

#define IMPLEMENT_MATH_OP_EXT(intOp, intType, dblOp, arbDblOp) INIT_BY_DEFAULT(num); \
	IMPLEMENT_RAW_MATH_OP_EXT(intOp, intType, dblOp, arbDblOp)

#define IMPLEMENT_MATH_OP(dblOp, arbDblOp) IMPLEMENT_MATH_OP_EXT(dblOp, double, dblOp, arbDblOp)
#define IMPLEMENT_RAW_MATH_OP(dblOp, arbDblOp) IMPLEMENT_RAW_MATH_OP_EXT(dblOp, double, dblOp, arbDblOp)

Number::Number()
{
	GetProperty__(0, __PROP_ENUM__PI) = (double)M_PI;
}

Number::Number(const Number& other)
{
	assert(false);
}	

CUniVar Number::Abs(CUniVar* num)
{
	IMPLEMENT_MATH_OP_EXT(abs, int, fabs, Abs)
}

CUniVar Number::Sin(CUniVar* num)
{
	IMPLEMENT_MATH_OP(sin, Sin)
}

CUniVar Number::Cos(CUniVar* num)
{
	IMPLEMENT_MATH_OP(cos, Cos)	
}

CUniVar Number::Tan(CUniVar* num)
{
	IMPLEMENT_MATH_OP(tan, Tan)	
}

CUniVar Number::Ctg(CUniVar* num)
{
	CUniVar sinVal  = Sin(num);

	if (sinVal == 0.0)
	{
		throw std::runtime_error("Number.Ctg cannot be calculated for input parameter");
	}

	return Cos(num) / sinVal;
}

CUniVar Number::Pow(CUniVar* num, CUniVar* degree)
{
	INIT_BY_DEFAULT(num);
	INIT_BY_DEFAULT(degree);

	if ((*num == 0) && (*degree < 0))
	{
		throw std::runtime_error("Invalid parameters for Number.Pow");
	}

	switch (num->GetType())
	{
	case UV_INT:
		return CUniVar(pow(num->ToInt(), degree->ToDouble()));
		break;

	case UV_DOUBLE:
		return CUniVar(pow(num->ToDouble(), degree->ToDouble()));
		break;

	case UV_ARB_DOUBLE:
		return CUniVar(num->ToArbDouble().Pow(degree->ToArbDouble()));
		break;

	default:
		if (num->IsConvertableToType(UV_ARB_DOUBLE))
		{
			return num->ToArbDouble().Abs();
		}
		else
		{
			throw std::runtime_error("Failed to convert to numeric");
		}
		break;
	}

	return CUniVar(0);
}

CUniVar Number::Log(CUniVar* num)
{
	INIT_BY_DEFAULT(num)
	if (*num > 0)
	{
		IMPLEMENT_RAW_MATH_OP(log, Log)		
	}
	else
	{
		throw std::runtime_error("Attempt to call Number.Log using incorrect parameter");
	}
}

CUniVar Number::Log10(CUniVar* num)
{
	INIT_BY_DEFAULT(num)
	if (*num > 0)
	{
		IMPLEMENT_RAW_MATH_OP(log10, Log10)
	}
	else
	{
		throw std::runtime_error("Attempt to call Number.Log10 using incorrect parameter");
	}
}

CUniVar Number::Exp(CUniVar* num)
{
	IMPLEMENT_MATH_OP(exp, Exp)	
}

CUniVar Number::Sqrt(CUniVar* num)
{
	INIT_BY_DEFAULT(num)
	if (*num >= 0)
	{
		IMPLEMENT_RAW_MATH_OP(sqrt, Sqrt)		
	}
	else
	{
		throw std::runtime_error("Attempt to call Number.Sqrt using incorrect parameter");
	}
}

CUniVar Number::Asin(CUniVar* num)
{
	INIT_BY_DEFAULT(num)
	if ((*num >= -1) && (*num <= 1))
	{
		IMPLEMENT_RAW_MATH_OP(asin, Asin)			
	}
	else
	{
		throw std::runtime_error("Attempt to call Number.Asin using incorrect parameter");
	}
}

CUniVar Number::Acos(CUniVar* num)
{
	INIT_BY_DEFAULT(num)
	if ((*num >= -1) && (*num <= 1))
	{
		IMPLEMENT_MATH_OP(acos, Acos)
	}
	else
	{
		throw std::runtime_error("Attempt to call Number.Acos using incorrect parameter");
	}
}

CUniVar Number::Atan(CUniVar* num)
{
	IMPLEMENT_MATH_OP(atan, Atan)		
}

CUniVar Number::Actg(CUniVar* num)
{
	INIT_BY_DEFAULT(num);

	if (*num == 0.0)
	{
		return GetPI(num->GetType()) / CUniVar(2.0);
	}
	
	CUniVar tmp = CUniVar(1.0) / *num;
	return Atan(&tmp);
}

CUniVar Number::Round(CUniVar* num)
{
	IMPLEMENT_MATH_OP(zround, Round)	
}

CUniVar Number::Truncate(CUniVar* num)
{
	IMPLEMENT_MATH_OP(ztrunc, Truncate)	
}

CUniVar Number::Ceil(CUniVar* num)
{
	IMPLEMENT_MATH_OP(ceil, Ceil)
}

CUniVar Number::Floor(CUniVar* num)
{
	IMPLEMENT_MATH_OP(floor, Floor)
}

CUniVar Number::Fraction(CUniVar* num)
{
	IMPLEMENT_MATH_OP(zfrac, Fraction)
}

CUniVar Number::GetPrecision(CUniVar* num)
{
	if (num->GetType() == UV_DOUBLE)
	{
		return 64;
	}
	else if (num->GetType() == UV_ARB_DOUBLE)
	{
		return num->ToArbDouble().GetPrecision();
	}
	else if (num->GetType() == UV_INT)
	{
		return 0;
	}

	throw std::runtime_error("Attempt to get precision of non-numeric variable");
	return 0;
}

CUniVar Number::PI(CUniVar* num)
{
	static std::string strPI = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989";
	int digitCount = num->ToInt();
	if ((digitCount < 16) || (digitCount > (int)strPI.size()))
	{
		throw std::runtime_error("Precision must be defined between 16 and 1000 for query of PI value");
	}
	
	return GetPI(UV_ARB_DOUBLE, digitCount);
}

Number* Number::GetInstance()
{
	static Number* obj = NULL;

	if (!obj)
	{
		obj = new Number();
	}

	return obj;
}

void Number::operator =(const Number& other)
{
	assert(false);
}

Number::~Number()
{
}