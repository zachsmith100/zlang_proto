
#pragma once

#include "UniVar.h"

class String
{
public:
	static CUniVar Trim(CUniVar* str);
	static CUniVar LeftTrim(CUniVar* str);
	static CUniVar RightTrim(CUniVar* str);
	static CUniVar Length(CUniVar* str);
	static CUniVar Replace(CUniVar* str, const CUniVar& _from, const CUniVar& _to);
	static CUniVar Insert(CUniVar* str, const CUniVar& _index, const CUniVar& _ch);
	static CUniVar Erase(CUniVar* str, const CUniVar& _index);
	static CUniVar Erase(CUniVar* str, const CUniVar& _index, const CUniVar& _count);
	static CUniVar Find(CUniVar* str, const CUniVar& ch);
	static CUniVar Find(CUniVar* str, const CUniVar& ch, const CUniVar& pos);
	static CUniVar FindFirstOf(CUniVar* str, const CUniVar& ch);
	static CUniVar FindFirstOf(CUniVar* str, const CUniVar& ch, const CUniVar& pos);
	static CUniVar FindLastOf(CUniVar* str, const CUniVar& ch);
	static CUniVar FindLastOf(CUniVar* str, const CUniVar& ch,  const CUniVar& pos);
	static CUniVar SubString(CUniVar* str, const CUniVar& _start, const CUniVar& _size);

	static CUniVar RegexMatch(CUniVar* str, const CUniVar& reg, const CUniVar& options);
	static CUniVar RegexMatch(CUniVar* str, const CUniVar& reg);	
	static CUniVar RegexSearch(CUniVar* str, const CUniVar& reg, const CUniVar& options);
	static CUniVar RegexSearch(CUniVar* str, const CUniVar& reg);
	static CUniVar RegexReplace(CUniVar* str, const CUniVar& reg, const CUniVar& to, const CUniVar& options);
	static CUniVar RegexReplace(CUniVar* str, const CUniVar& reg, const CUniVar& to);

	static CUniVar Explode(CUniVar* str, const CUniVar& _delim);

private:
	static bool SetupRegexOptions(const CUniVar& options, zrn::regex::flag_type& flags);
	static std::string GetRegexErrorAsString(const std::regex_error& e);
};