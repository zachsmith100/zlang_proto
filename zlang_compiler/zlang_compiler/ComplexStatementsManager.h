
#pragma once

#include <vector>
#include <string>

struct SCycle
{
	unsigned int tabLevel;
	unsigned int line;
	bool isActive;

	SCycle(const unsigned int _tabLevel, const unsigned int _line)
	{
		tabLevel = _tabLevel;
		line = _line;		
		isActive = true;
	}
};

struct SCondition
{
	unsigned int tabLevel;
	unsigned int line;
	bool isElseUsed;
	bool isActive;
	bool ifCodeExists;
	bool elseCodeExists;
	bool bracketsClosed;

	SCondition(const unsigned int _tabLevel, const unsigned int _line)
	{
		tabLevel = _tabLevel;
		line = _line;
		isActive = true;
		isElseUsed = false;
		ifCodeExists = false;
		elseCodeExists = false;
		bracketsClosed = false;
	}
};

struct SForEach
{
	unsigned int tabLevel;
	unsigned int line;
	std::string iterationCode;
	std::string footerCode;
	std::string cycleVar;
	bool isIterationCodePlaced;
	bool isActive;

	SForEach(const unsigned int _tabLevel, const unsigned int _line, const std::string _iterationCode, const std::string _footerCode, const std::string _cycleVar)
	{
		tabLevel = _tabLevel;
		line = _line;
		isIterationCodePlaced = false;
		isActive = true;
		iterationCode = _iterationCode;
		footerCode = _footerCode;
		cycleVar = _cycleVar;
	}
};

class CCodeGenerator;

class CComplexStatementsManager
{
public:
	static CComplexStatementsManager* GetInstance();

	void RegisterIF(const unsigned int line, const unsigned int level);
	void RegisterForEach(const unsigned int line, const unsigned int level, const std::string& iterationCode, const std::string& footerCode, const std::string& cycleVar);
	void RegisterCycle(const unsigned int line, const unsigned int level);
	void OnEndProgram();
	void OnNewTabLevel(const unsigned int level);
	void OnBeforeNewLine();
	void OnAfterNewLine();

	bool CanElseBeUsed()const;
	void MarkElseAsUsed();

	void SetCodeGenerator(CCodeGenerator* codeGenerator);

	bool CanCycleVarBeUsed(const std::string& var);
	bool CanCycleOperatorBeUsed(const unsigned int tabLevel)const;

private:
	CComplexStatementsManager();
	CComplexStatementsManager(const CComplexStatementsManager& other);
	void operator =(const CComplexStatementsManager& other);
	~CComplexStatementsManager();

	unsigned int m_tabLevel;
	unsigned int m_skipLines;
	
	std::vector<SCondition *> m_conditions;
	std::vector<SForEach *> m_forEaches;
	std::vector<SCycle *> m_cycles;
	CCodeGenerator* m_codeGenerator;
};
