
#pragma once

#include <vector>
#include <string>

enum ZTokenType
{
	TT_INTEGER = 0,
	TT_BOOLEAN,
	TT_SYMBOL,
	TT_STRING,
	TT_FLOAT,
	TT_ARB_FLOAT,
	TT_HEX_INTEGER,
	TT_VARIABLE
};

enum VariableType
{
	VT_UNRECOGNIZED = 0,
	VT_INTEGER,
	VT_ARBITRARY_INTEGER,
	VT_DOUBLE,
	VT_ARBITRARY_FLOAT_POINT,
	VT_ARBITRARY_FIXED_POINT
};

enum CompileTarget
{
	TARGET_GCC = 0,
	TARGET_VISUAL_STUDIO,
	TARGET_CLANG,
	TARGET_ALL
};

class CCompileTarget
{
public:	
	static CCompileTarget* GetInstance();
	
	void SetCommandLine(int argc, char* argv[]);

	CompileTarget GetTarget()const;

	ZTokenType RefineTokenType(const ZTokenType& tokenType);

	bool GenerateFilesForCurrentTarget(const std::string& templatesPath, const std::string& outputDir);

	void AddTargetTemplatesFilenames(std::vector<std::string>& files, const CompileTarget target = TARGET_ALL);	

	bool Compile(const std::string& outputDir);

	bool IsMethodsListNeeded()const;

	void SetDefaultIntegerType(const std::string& defIntType);
	VariableType GetDefaultIntegerType()const;

	void SetDefaultDoubleType(const std::string& defDblType);
	VariableType GetDefaultDoubleType()const;

	void SetDefaultPrecision(const unsigned int precision);
	unsigned int GetDefaultPrecision()const;
	
private:
	CCompileTarget();
	CCompileTarget(const CCompileTarget& other);
	void operator =(const CCompileTarget& other);
	~CCompileTarget();

	void ParseTargetString(const std::string& targetStr);
	void ParseOptionalParameter(const std::string& optionalParam);

	VariableType GetVarTypeByString(const std::string& typeName);
	ZTokenType NumericTypeToTokenType(const VariableType& varType);

	std::string GetMSBuildPath()const;	

	CompileTarget m_target;

	bool m_isMethodsListNeeded;
		
	VariableType m_defaultIntegerType;
	VariableType m_defaultDoubleType;
	unsigned int m_defaultPrecision;
};