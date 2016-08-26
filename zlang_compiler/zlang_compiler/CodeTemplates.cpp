
#include "Common.h"

// Platform
///////////
#include <iostream>
#include <assert.h>

// Project
//////////
#include "CodeTemplates.h"
#include "Utils.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////
CCodeTemplates::CCodeTemplates()
{
}

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////
CCodeTemplates::CCodeTemplates(const CCodeTemplates& other)
{
	assert(false);
}

////////////////////////////////////////////////////////////////////////////////
// Get Instance
////////////////////////////////////////////////////////////////////////////////
CCodeTemplates* CCodeTemplates::GetInstance()
{
	static CCodeTemplates* obj = NULL;

	if (!obj)
	{
		obj = new CCodeTemplates();
	}

	return obj;
}

////////////////////////////////////////////////////////////////////////////////
// Load Template
////////////////////////////////////////////////////////////////////////////////
CCodeTemplate* CCodeTemplates::LoadTemplate(
  const std::string & folder,
  const std::string & domain,
  const std::string& fileName)
{
	CPath path(folder, domain, fileName + ".tpl");  
	return LoadTemplate(path);
}

CCodeTemplate* CCodeTemplates::LoadTemplate(const CPath& fileName)
{	  
	CodeTemplatesList::iterator it = m_templatesMap.find(fileName);
	if (it != m_templatesMap.end())
	{
		assert((*it).second < m_templates.size());
		return m_templates[(*it).second];
	}

	CCodeTemplate* tpl = new CCodeTemplate();
	if (!tpl->Load(fileName))
	{
		delete tpl;
		return NULL;
	}

	m_templates.push_back(tpl);
	m_templatesMap.insert(CodeTemplatesList::value_type(fileName.ToStdString(), m_templates.size() - 1));
	return tpl;
}

unsigned int CCodeTemplates::GetTemplateID(const std::string& fileName)const
{
	CodeTemplatesList::const_iterator it = m_templatesMap.find(fileName);
	if (it == m_templatesMap.end())
	{
		return 0xFFFFFFFF;
	}

	return (*it).second;
}

CCodeTemplate* CCodeTemplates::GetTemplate(const unsigned int id)const
{
	if (id >= m_templates.size())
	{
		return NULL;
	}

	return m_templates[id];
}

void CCodeTemplates::operator =(const CCodeTemplates& other)
{
	assert(false);
}

CCodeTemplates::~CCodeTemplates()
{
	std::vector<CCodeTemplate*>::iterator it = m_templates.begin();
	std::vector<CCodeTemplate*>::iterator itEnd = m_templates.end();

	for ( ; it != itEnd; it++)
	{
		delete (*it);
	}
}
