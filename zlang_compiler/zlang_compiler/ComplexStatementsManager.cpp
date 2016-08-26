
#include "Common.h"
#include "ComplexStatementsManager.h"
#include "ErrorReporter.h"
#include "CodeGenerator.h"
#include <assert.h>

CComplexStatementsManager::CComplexStatementsManager():
	m_tabLevel(0),
	m_skipLines(0),
	m_codeGenerator(NULL)
{
}

void CComplexStatementsManager::SetCodeGenerator(CCodeGenerator* codeGenerator)
{
	m_codeGenerator = codeGenerator;
}

CComplexStatementsManager::CComplexStatementsManager(const CComplexStatementsManager& other)
{
	assert(false);
}

CComplexStatementsManager* CComplexStatementsManager::GetInstance()
{
	static CComplexStatementsManager* obj = NULL;

	if (!obj)
	{
		obj = new CComplexStatementsManager();
	}

	return obj;
}

void CComplexStatementsManager::RegisterCycle(const unsigned int line, const unsigned int level)
{
	SCycle* cycle = new SCycle(level, line);
	m_cycles.push_back(cycle);
	m_skipLines = 4;
}

void CComplexStatementsManager::RegisterIF(const unsigned int line, const unsigned int level)
{
	SCondition* cond = new SCondition(level, line);
	m_conditions.push_back(cond);
}

void CComplexStatementsManager::RegisterForEach(const unsigned int line, const unsigned int level, const std::string& iterationCode, const std::string& footerCode, const std::string& cycleVar)
{
	SForEach* forEach = new SForEach(level, line, iterationCode, footerCode, cycleVar);
	m_forEaches.push_back(forEach);
	m_skipLines = 4;
}

void CComplexStatementsManager::OnNewTabLevel(const unsigned int level)
{
	m_tabLevel = level;

	std::vector<SCondition*>::const_reverse_iterator it = m_conditions.rbegin();
	std::vector<SCondition*>::const_reverse_iterator itEnd = m_conditions.rend();

	for ( ; it != itEnd; it++)
	if (((*it)->isActive) && (m_tabLevel >= (*it)->tabLevel + 1))
	{
		if (!(*it)->isElseUsed)
		{
			(*it)->ifCodeExists = true;
		}
		else
		{
			(*it)->elseCodeExists = true;
		}

		break;
	}	

	std::vector<SForEach*>::const_reverse_iterator itf = m_forEaches.rbegin();
	std::vector<SForEach*>::const_reverse_iterator itfEnd = m_forEaches.rend();

	for ( ; itf != itfEnd; itf++)
	if (((*itf)->isActive) && (m_tabLevel >= (*itf)->tabLevel + 1) && (!(*itf)->isIterationCodePlaced))
	{
		(*itf)->isIterationCodePlaced = true;		
		m_codeGenerator->PurePrintTabs(m_tabLevel);		
		m_codeGenerator->PurePrint((char*)(*itf)->iterationCode.c_str());						
	}
}

void CComplexStatementsManager::OnBeforeNewLine()
{
	if (m_skipLines > 0)
	{
		m_skipLines--;
		return;
	}

	std::vector<SCondition*>::const_iterator it = m_conditions.begin();
	std::vector<SCondition*>::const_iterator itEnd = m_conditions.end();

	for ( ; it != itEnd; it++)
	if (((*it)->isActive) && (m_tabLevel <= (*it)->tabLevel) && ((*it)->ifCodeExists)) 
	{
		if ((*it)->bracketsClosed)
		{
			(*it)->isActive = false;
		}
		else
		{
			(*it)->bracketsClosed = true;
		}		
	}

	std::vector<SForEach*>::const_iterator itf = m_forEaches.begin();
	std::vector<SForEach*>::const_iterator itfEnd = m_forEaches.end();

	for ( ; itf != itfEnd; itf++)
	if (((*itf)->isActive) && (m_tabLevel <= (*itf)->tabLevel)) 
	{			
		m_codeGenerator->PurePrint((char*)(*itf)->footerCode.c_str());
		m_codeGenerator->PurePrintTabs(m_tabLevel);
		(*itf)->isActive = false;
	}

	std::vector<SCycle*>::const_iterator itc = m_cycles.begin();
	std::vector<SCycle*>::const_iterator itcEnd = m_cycles.end();

	for ( ; itc != itcEnd; itc++)
	if (((*itc)->isActive) && (m_tabLevel <= (*itc)->tabLevel)) 
	{
		(*itc)->isActive = false;
	}
}

void CComplexStatementsManager::OnAfterNewLine()
{
	if (m_skipLines > 0)
	{
		m_skipLines--;
		return;
	}
}

bool CComplexStatementsManager::CanCycleOperatorBeUsed(const unsigned int tabLevel)const
{
	std::vector<SCycle*>::const_reverse_iterator it = m_cycles.rbegin();
	std::vector<SCycle*>::const_reverse_iterator itEnd = m_cycles.rend();

	for ( ; it != itEnd; it++)
	if (((*it)->isActive) && ((*it)->tabLevel < tabLevel))
	{
		return true;		
	}

	return false;	
}

bool CComplexStatementsManager::CanElseBeUsed()const
{
	std::vector<SCondition*>::const_reverse_iterator it = m_conditions.rbegin();
	std::vector<SCondition*>::const_reverse_iterator itEnd = m_conditions.rend();

	for ( ; it != itEnd; it++)
	if (((*it)->isActive) && ((*it)->ifCodeExists))
	{
		if (((*it)->tabLevel == m_tabLevel) && (!(*it)->isElseUsed))
		{
			return true;
		}
	}

	return false;
}

void CComplexStatementsManager::MarkElseAsUsed()
{
	std::vector<SCondition*>::reverse_iterator it = m_conditions.rbegin();
	std::vector<SCondition*>::reverse_iterator itEnd = m_conditions.rend();

	for ( ; it != itEnd; it++)
	if ((*it)->isActive)
	{
		if ((*it)->tabLevel == m_tabLevel)
		{
			(*it)->isElseUsed = true;
			m_skipLines = 4;
			break;
		}
	}
}

void CComplexStatementsManager::OnEndProgram()
{
	std::vector<SCondition*>::iterator it = m_conditions.begin();
	std::vector<SCondition*>::iterator itEnd = m_conditions.end();

	for ( ; it != itEnd; it++)
	{
		bool wrongIf  = (!(*it)->ifCodeExists);
		bool wrongElse = ((!(*it)->elseCodeExists) && ((*it)->isElseUsed));
		if (((*it)->isActive) || (wrongIf) || (wrongElse))
		{
			CErrorReporter::GetInstance()->OnFatalError(wrongIf ? INCORRECT_IF : INCORRECT_ELSE, (*it)->line);			
		}
	}
}

bool CComplexStatementsManager::CanCycleVarBeUsed(const std::string& var)
{
	std::vector<SForEach*>::iterator it = m_forEaches.begin();
	std::vector<SForEach*>::iterator itEnd = m_forEaches.end();

	for ( ; it != itEnd; it++)
	if (((*it)->isActive) && ((*it)->cycleVar == var))
	{
		return false;
	}

	return true;
}

void CComplexStatementsManager::operator =(const CComplexStatementsManager& other)
{
	assert(false);
}

CComplexStatementsManager::~CComplexStatementsManager()
{
	{
		std::vector<SCondition*>::iterator it = m_conditions.begin();
		std::vector<SCondition*>::iterator itEnd = m_conditions.end();

		for ( ; it != itEnd; it++)
		{
			delete *it;
		}
	}

	{
		std::vector<SCycle*>::iterator it = m_cycles.begin();
		std::vector<SCycle*>::iterator itEnd = m_cycles.end();

		for ( ; it != itEnd; it++)
		{
			delete *it;
		}
	}

	{
		std::vector<SForEach*>::iterator it = m_forEaches.begin();
		std::vector<SForEach*>::iterator itEnd = m_forEaches.end();

		for ( ; it != itEnd; it++)
		{
			delete *it;
		}
	}
}