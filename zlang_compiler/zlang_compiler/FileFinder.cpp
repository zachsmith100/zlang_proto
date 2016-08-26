
#include "Common.h"
#include "FileFinder.h"
#include <string.h>

bool CFileFinder::FindFiles(const std::string& folderPath)
{
#ifdef PLATFORM_UNIX
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(folderPath.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL) 
		{
			if (ent->d_name != NULL)
			{
				if ((ent->d_type == DT_REG) || (ent->d_type == DT_UNKNOWN))
				{
					if (!HandleFile(folderPath, ent->d_name))
					{
						return false;
					}
				}
				else if ((ent->d_type == DT_DIR) && (strcmp(ent->d_name, "..") != 0) && (strcmp(ent->d_name, ".") != 0))
				{
					bool skip = false;

					std::vector<std::string>::iterator it = m_excludeFolders.begin();
					std::vector<std::string>::iterator itEnd = m_excludeFolders.end();

					for ( ; it != itEnd; it++)
					if (strcmp(ent->d_name, (*it).c_str()) == 0)
					{
						skip = true;
						break;
					}

					if (!skip)
					{
						CFolderPath newFolderPath = folderPath;
						newFolderPath.Add(ent->d_name);					
						FindFiles(newFolderPath);
					}
				}

			}
		}
		closedir(dir);
	} 
#else
	HANDLE find;
	BOOL needContinue = TRUE;
	WIN32_FIND_DATAA data;
	memset(&data, 0, sizeof(WIN32_FIND_DATAA));

	find = FindFirstFileA((folderPath + "*.*").c_str(), &data); 

	if (!find)
	{
		return false;
	}

	while (find && needContinue) 
	{		
		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (strlen(data.cFileName) > 0)
			if (!HandleFile(folderPath, data.cFileName))
			{
				return false;
			}
		}
		else if ((strcmp(data.cFileName, ".") != 0) && (strcmp(data.cFileName, "..") != 0))
		{
			bool skip = false;

			std::vector<std::string>::iterator it = m_excludeFolders.begin();
			std::vector<std::string>::iterator itEnd = m_excludeFolders.end();

			for ( ; it != itEnd; it++)
			if (strcmp(data.cFileName, (*it).c_str()) == 0)
			{
				skip = true;
				break;
			}

			if (!skip)
			{
				CFolderPath newFolderPath(folderPath);			
				newFolderPath.Add(data.cFileName);
				FindFiles(newFolderPath);
			}
		}
		needContinue = FindNextFileA(find, &data);
	}

	FindClose(find); 
#endif

	return true;
}

void CFileFinder::AddExcludeFolder(const std::string& excludeFolder)
{
	m_excludeFolders.push_back(excludeFolder);
}