
#pragma once

#include <string>
#include <vector>

struct ParameterAnnotation
{
	std::string paramName;
	std::string editor;
	std::string group;
	std::string description;
	int max;
	int min;
	std::vector<std::string> validators;
	bool used;
	unsigned int startLine;

	ParameterAnnotation(const std::string& name, const unsigned int line)
	{
		startLine = line;
		paramName = name;
		used = false;
		min = -1;
		max = -1;
	}

	ParameterAnnotation(const ParameterAnnotation& other)
	{
		paramName = other.paramName;
		editor = other.editor;
		group = other.group;
		validators = other.validators;
		used = other.used;
		startLine = other.startLine;
		description = other.description;
		min = other.min;
		max = other.max;
	}
};

class CCurrentMethodAnnotation
{
public:
	static CCurrentMethodAnnotation* GetInstance();

	void SetCurrentParameter(const std::string& name, const unsigned int line);
	std::string GetCurrentParameter()const;

	bool SetProperty(const std::string& name, const std::string& value);
	bool SetProperty(const std::string& name, const std::vector<std::string>& values);

	void Reset();

	ParameterAnnotation* FindParameterAnnotation(const std::string& name);

	bool IsArrayAsValueNeeded(const std::string& name)const;

	std::string GetDefaultReturnField()const;

private:
	CCurrentMethodAnnotation();
	CCurrentMethodAnnotation(const CCurrentMethodAnnotation& other);
	void operator =(const CCurrentMethodAnnotation& other);
	~CCurrentMethodAnnotation();

	void FindNonUsedParameters();

	std::string m_defaultReturnField;
	std::vector<ParameterAnnotation> m_parameters;
};