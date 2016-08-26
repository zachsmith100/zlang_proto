
#include "ZString.h"
#include "StandardFuncs.h"
#include "LogManager.h"
#include "ClassSupport.h"
#include "ArraySupport.h"

#define CONVERT_TO_STRING_IF_NEEDED if (str_->GetType() == UV_UNKNOWN) *str_ = "";

CUniVar String::Trim(CUniVar* str_)
{		
	CONVERT_TO_STRING_IF_NEEDED
	CUniVar tmp = LeftTrim(str_);
	return RightTrim(&tmp);
}

CUniVar String::LeftTrim(CUniVar* str_)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	if (str.size() == 0)
	{
		return "";
	}
	int i = 0;
	while ((str[i] == ' ') && (str.size() != i - 1))
	{
		i++;
	}
	if (i != 0)
	{
		str.erase(str.begin(), str.begin() + i);
	}
	return str;
}

CUniVar String::RightTrim(CUniVar* str_)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	if (str.size() == 0)
	{
		return "";
	}
	int i = str.size() - 1;
	while ((str[i] == ' ') && (i >= 0))
	{
		i--;
	}
	if (i != str.size() - 1)
	{
		str.erase(str.begin() + i + 1, str.begin() + str.size());
	}	
	return str;
}

CUniVar String::Length(CUniVar* str_)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.size();
}

CUniVar String::Replace(CUniVar* str_, const CUniVar& from_, const CUniVar& to_)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	std::string from = from_.ToString();
	std::string to = to_.ToString();

	if (from.size() == 0)
	{
		Log.Warnings.AddLine2s("Attempt to replace empty string by another string after line %s in %s", currentLine, getCurrentMethodName());
		return *str_;
	}

	int p = str.find(from);
	
	while (p != -1) 
	{ 
		str.replace(p, from.length(), to); 
		p = str.find(from, p + to.length());
	}
	
	return str;
}

CUniVar String::Insert(CUniVar* str_, const CUniVar& _index, const CUniVar& _ch)
{
	CONVERT_TO_STRING_IF_NEEDED
	unsigned int index = _index.ToUInt();
	std::string ch = _ch.ToString();
	std::string str = str_->ToString();

	if (index > str.size())
	{
		Log.Warnings.AddLine3s("Attempt to insert using wrong index to string %s after line %s in %s", str, currentLine, getCurrentMethodName());
		return *str_;
	}
	
	str.insert(index, ch);
	
	return str;
}

CUniVar String::Erase(CUniVar* str_, const CUniVar& index_)
{
	CONVERT_TO_STRING_IF_NEEDED
	unsigned int index = index_.ToUInt();
	std::string str = str_->ToString();

	if (index >= str.size())
	{
		Log.Warnings.AddLine3s("Attempt to erase using wrong index from string %s after line %s in %s", str, currentLine, getCurrentMethodName());
		return *str_;
	}
	
	str.erase(str.begin() + index);
	
	return str;
}

CUniVar String::Erase(CUniVar* str_, const CUniVar& index_, const CUniVar& count_)
{
	CONVERT_TO_STRING_IF_NEEDED
	unsigned int index = index_.ToUInt();
	unsigned int count = count_.ToUInt();
	std::string str = str_->ToString();

	if ((index >= str.size()) || (index + count > str.size()))
	{
		Log.Warnings.AddLine3s("Attempt to erase using wrong index or count from string %s after line %s in %s", str, currentLine, getCurrentMethodName());
		return *str_;
	}
	
	str.erase(str.begin() + index, str.begin() + index + count);

	return str;
}

CUniVar String::Find(CUniVar* str_, const CUniVar& ch)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find(ch.ToString());
}

CUniVar String::Find(CUniVar* str_, const CUniVar& ch, const CUniVar& pos)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find(ch.ToString(), pos.ToInt());
}

CUniVar String::FindFirstOf(CUniVar* str_, const CUniVar& ch)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find_first_of(ch.ToString());
}

CUniVar String::FindFirstOf(CUniVar* str_, const CUniVar& ch, const CUniVar& pos)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find_first_of(ch.ToString(), pos.ToInt());
}

CUniVar String::FindLastOf(CUniVar* str_, const CUniVar& ch)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find_last_of(ch.ToString());
}

CUniVar String::FindLastOf(CUniVar* str_, const CUniVar& ch,  const CUniVar& pos)
{
	CONVERT_TO_STRING_IF_NEEDED
	std::string str = str_->ToString();
	return str.find_last_of(ch.ToString(), pos.ToInt());
}

CUniVar String::SubString(CUniVar* str_, const CUniVar& start_, const CUniVar& size_)
{
	CONVERT_TO_STRING_IF_NEEDED
	unsigned int start = start_.ToUInt();
	unsigned int size = size_.ToUInt();	
	std::string str = str_->ToString();

	if (start >= str.size())
	{
		Log.Warnings.AddLine3s("Attempt to get substring using wrong index or count from string %s after line %s in %s", str, currentLine, getCurrentMethodName());
		return "";
	}
	
	return str.substr(start, size);
}

std::string String::GetRegexErrorAsString(const std::regex_error& e)
{
	switch (e.code())
	{
	case std::regex_constants::error_collate:
		return "The expression contained an invalid collating element name.";
		break;

	case std::regex_constants::error_ctype:
		return "The expression contained an invalid character class name.";
		break;

	case std::regex_constants::error_escape:
		return "The expression contained an invalid escaped character, or a trailing escape.";
		break;

	case std::regex_constants::error_backref:
		return "The expression contained an invalid back reference.";
		break;

	case std::regex_constants::error_brack:
		return "The expression contained mismatched brackets ([ and ]).";
		break;

	case std::regex_constants::error_paren:
		return "The expression contained mismatched parentheses (( and )).";
		break;

	case std::regex_constants::error_brace:
		return "The expression contained mismatched braces ({ and }).";
		break;

	case std::regex_constants::error_badbrace:
		return "The expression contained an invalid range between braces ({ and }).";
		break;

	case std::regex_constants::error_range:
		return "The expression contained an invalid character range.";
		break;

	case std::regex_constants::error_space:
		return "There was insufficient memory to convert the expression into a finite state machine.";
		break;

	case std::regex_constants::error_badrepeat:
		return "The expression contained a repeat specifier (one of *?+{) that was not preceded by a valid regular expression.";
		break;

	case std::regex_constants::error_complexity:
		return "The complexity of an attempted match against a regular expression exceeded a pre-set level.";
		break;

	case std::regex_constants::error_stack:
		return "There was insufficient memory to determine whether the regular expression could match the specified character sequence.";
		break;

	default:
		return "unknown error";
	}
}

CUniVar String::RegexMatch(CUniVar* str_, const CUniVar& reg, const CUniVar& options)
{
	CONVERT_TO_STRING_IF_NEEDED
	CArrayDataContainer* results = new CArrayDataContainer();

	zrn::regex::flag_type flags;
	if (!SetupRegexOptions(options, flags))
	{
		return CUniVar(results);
	}

	std::string str = str_->ToString();

	try 
	{
		zrn::regex regex(reg.ToString(), flags);	
		
		auto matches_begin = std::sregex_iterator(str.begin(), str.end(), regex);
		auto matches_end = std::sregex_iterator();

	    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) 		
		{
			std::smatch match = *i;

			auto it = match.begin();
			auto itEnd = match.end();

			CArrayDataContainer* adt = new CArrayDataContainer();

			for ( ; it != itEnd; it++)
			{
				CUniVar match1(*it);
				adt->Push(match1);
			}

			CUniVar result1(adt);
			results->Push(result1);    
		}
	} 
	catch (std::regex_error& ) 
	{
		Log.Warnings.AddLine3s("Invalid regular expression %s after line %s in %s", reg.ToString(), currentLine, getCurrentMethodName());
	}

	return CUniVar(results);
}

CUniVar String::RegexMatch(CUniVar* str, const CUniVar& reg)
{
	return RegexMatch(str, reg, CUniVar(new CClassDataContainer()));
}

#define HANDLE_FLAG(flg) if ((cdt->IsPropertyExist(#flg)) && (cdt->GetProperty__(#flg).ToBoolean())) flags = flags | zrn::regex::flg;

#define HANDLE_TYPE_FLAG(flg) if ((cdt->IsPropertyExist(#flg)) && (cdt->GetProperty__(#flg).ToBoolean())) { flags = flags | zrn::regex::flg; cnt++; }

bool String::SetupRegexOptions(const CUniVar& options, zrn::regex::flag_type& flags)
{	
	if (options.GetType() != UV_CLASS)
	{
		Log.Warnings.AddLine2s("You must use object with named properties to set regular expression properties after line %s in %s", currentLine, getCurrentMethodName());
		return false;
	}

	CClassDataContainer* cdt = options.ToConstClass();

	flags = zrn::regex::flag_type();
	HANDLE_FLAG(icase);
	HANDLE_FLAG(nosubs);
	HANDLE_FLAG(optimize);
	HANDLE_FLAG(collate);
	int cnt = 0;
	HANDLE_TYPE_FLAG(ECMAScript);
	HANDLE_TYPE_FLAG(basic);
	HANDLE_TYPE_FLAG(extended);
	HANDLE_TYPE_FLAG(awk);
	HANDLE_TYPE_FLAG(grep);
	HANDLE_TYPE_FLAG(egrep);

	if (cnt == 0)
	{
		flags |= zrn::regex::ECMAScript;
	}
	else if (cnt > 1)
	{
		Log.Warnings.AddLine2s("You must define only one type of regular expressions after line %s in %s", currentLine, getCurrentMethodName());
		return false;
	}
	
	return true;
}

CUniVar String::RegexSearch(CUniVar* str_, const CUniVar& reg, const CUniVar& options)
{
	CONVERT_TO_STRING_IF_NEEDED
	zrn::regex::flag_type flags;
	if (!SetupRegexOptions(options, flags))
	{
		return getFullResultObject(false);
	}

	std::string str = str_->ToString();

	try 
	{
		zrn::regex regex(reg.ToString(), flags);

		if (zrn::regex_search(str, regex)) 
		{
			return true;
		}
	}
	catch (std::regex_error& ) 
	{
		Log.Warnings.AddLine3s("Invalid regular expression %s after line %s in %s", reg.ToString(), currentLine, getCurrentMethodName());
	}

	return false;
}

CUniVar String::RegexSearch(CUniVar* str, const CUniVar& reg)
{
	return RegexSearch(str, reg, CUniVar(new CClassDataContainer()));
}

CUniVar String::RegexReplace(CUniVar* str_, const CUniVar& reg, const CUniVar& to, const CUniVar& options)
{
	CONVERT_TO_STRING_IF_NEEDED
	zrn::regex::flag_type flags;
	if (!SetupRegexOptions(options, flags))
	{
		return *str_;
	}

	std::string str = str_->ToString();
	try 
	{		
		zrn::regex regex(reg.ToString(), flags);

		std::string newStr = zrn::regex_replace(str, regex, to.ToString());
		return newStr;
	}
	catch (std::regex_error& ) 
	{
		Log.Warnings.AddLine3s("Invalid regular expression %s after line %s in %s", reg.ToString(), currentLine, getCurrentMethodName());
		return str;
	}

	return str;
}

CUniVar String::RegexReplace(CUniVar* str, const CUniVar& reg, const CUniVar& to)
{
	return RegexReplace(str, reg, to, CUniVar(new CClassDataContainer()));
}

CUniVar String::Explode(CUniVar* str_, const CUniVar& _delim)
{
	CONVERT_TO_STRING_IF_NEEDED
	CArrayDataContainer* adt = new CArrayDataContainer();	
	std::string str = str_->ToString();
	if (str.size() == 0)
	{
		Log.Warnings.AddLine2s("Attempt to explode empty string after line %s in %s", currentLine, getCurrentMethodName());
		return CUniVar(adt);
	}

	std::string delim = _delim.ToString();
	if (delim.size() == 0)
	{
		Log.Warnings.AddLine2s("Attempt to explode string using empty delimeter after line %s in %s", currentLine, getCurrentMethodName());
		return CUniVar(adt);
	}

	int pos = str.find(delim);
	while (pos != std::string::npos)
	{
		std::string elem = str.substr(0, pos);
		adt->Push(CUniVar(elem));
		int offset = pos + delim.size();
		str = str.substr(offset, str.size() - offset);
		pos = str.find(delim);
	}
	adt->Push(str);

	return CUniVar(adt);
}