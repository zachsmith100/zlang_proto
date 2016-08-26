
#pragma once

#include "Common.h"
#include "CodeTemplate.h"
#include <assert.h>
#include <vector>
#include <string>
#include <map>

#define USE_TPL(domain, name) static CCodeTemplate* name##Tpl = CCodeTemplates::GetInstance()->LoadTemplate("code_templates", #domain, #name); assert(name##Tpl)
#define INIT_TPL(tplName, domain, name) tplName##Tpl = CCodeTemplates::GetInstance()->LoadTemplate("code_templates", #domain, #name); assert(tplName##Tpl)
#define ADD_FROM_TPL(domain, name, var) USE_TPL(domain, name); var += name##Tpl->GetText()
#define RETURN_FROM_TPL(domain, name) USE_TPL(domain, name); return name##Tpl->GetText()
#define USE_INDEX(tpl, name) static unsigned int name##Index = tpl##Tpl->GetVariableIndex(#name)
#define GET_END_SIGN(line) (context.highLevelCounter == 1 ? CCodeGenerator::GetEndLineCode(line) : "")
#define DEFINE_END_SIGN(line)	std::string end = GET_END_SIGN(line)
#define IS_FIRST_COMMAND_IN_LINE (context.highLevelCounter == 1)

typedef std::map<std::string, unsigned int> CodeTemplatesList;

class CCodeTemplates
{
public:
	static CCodeTemplates* GetInstance();

	unsigned int GetTemplateID(const std::string& fileName)const;	
	CCodeTemplate* GetTemplate(const unsigned int id)const;

	CCodeTemplate* LoadTemplate(const CPath& fileName);
	CCodeTemplate* LoadTemplate(const std::string& folder, const std::string& domain, const std::string& fileName);

private:
	CCodeTemplates();
	CCodeTemplates(const CCodeTemplates& other);
	void operator =(const CCodeTemplates& other);
	~CCodeTemplates();

	CodeTemplatesList m_templatesMap;
	std::vector<CCodeTemplate*> m_templates;
};
