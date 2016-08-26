
#pragma once

#include <string>
#include <vector>

class CFileFinder
{
public:
	void AddExcludeFolder(const std::string& excludeFolder);
	
	bool FindFiles(const std::string& folderPath);

	virtual bool HandleFile(const std::string& path, const std::string& name) = 0;

private:
	std::vector<std::string> m_excludeFolders;
};