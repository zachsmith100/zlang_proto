
#pragma once

#include "Common.h"

#include <stdio.h>

bool IsDirectoryExists(const char* path);
bool ForceDirectory(const CFolderPath& path);
bool ForceDirectories(const CFolderPath& startPath, const CFolderPath& endPath);
bool IsFileExists(const CPath& path);

bool FileCopy(const CPath& inputPath, const CPath& outputPath);
FILE* OpenFile(const std::string& fileName, const std::string& mode);

std::string IntToStr(int num);

void CharReplace(std::string& str, const char from, const char to);

void TrimTabsAndSpaces(std::string& str);

std::vector<std::string> SplitStringIntoVector(const std::string& str, const char delim);

bool ExecuteExternalCommand(const std::string& path, const std::string& params);

std::string GetCurrentFolder();
CFolderPath GetTempFolder();
std::string GetTempFilePath();
std::string GetCppCompilerPath();

std::string GetPathAsStringConst(const std::string& path);

FILE* GetTempFileFromString(std::string& fileContent, std::string& filePath);

void StringReplace(std::string& str, std::string from, std::string to);
void StringReplaceOnce(std::string& str, std::string from, std::string to);

std::string UnquoteString(const std::string& str);