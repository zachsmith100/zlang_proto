
#pragma once

#include <string>
#include <vector>
#include <map>

struct SymbolDesc
{
	bool isOutParameter;
	bool isUsed;

	SymbolDesc()
	{
		isUsed = false;
		isOutParameter = false;
	}

	SymbolDesc(const SymbolDesc& other)
	{
		isUsed = other.isUsed;
		isOutParameter = other.isOutParameter;
	}
};

typedef std::map<std::string, SymbolDesc> SymbolsMap;

class CSymbolTable
{
public:
	CSymbolTable();
	~CSymbolTable();

	void SetParent(CSymbolTable* const table);
	CSymbolTable* GetParent();

	void AddChild(CSymbolTable* const table);
	std::vector<CSymbolTable *> GetChildren();

	void AddSymbol(const std::string& id);
	void MarkSymbolAsUsed(const std::string& id);
	void MarkSymbolAsOutParameter(const std::string& id);
	
	bool IsSymbolUsed(const std::string& id)const;
	bool IsSymbolUsedFirstly(const std::string& id)const;
	bool IsSymbolCanBeUsedForVariable(const std::string& id)const;

	bool IsSymbolOutParameter(const std::string& id)const;

	void Print();

	void SaveState();
	void ResetState();

	void MoveSymbolToHigherLevel(const std::string& id);

	static CSymbolTable* GetRoot();

	SymbolDesc GetSymbolDesc(const std::string& id)const;

private:
	void SetSymbolStatus(const std::string& id, const bool isUsed);
	void SetSymbolType(const std::string& id, const bool isOutParameter);

	SymbolsMap m_symbols;

	SymbolsMap m_savedSymbols;

	CSymbolTable* m_parent;
	std::vector<CSymbolTable *> m_children;
};