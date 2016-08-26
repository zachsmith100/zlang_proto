
#pragma once

#include <string>

void ParseExePath(const std::string& exePath, std::string& exeDir, std::string& exeName);
bool IsFloat(const std::string& str);
bool IsStringConvertableToInt(const std::string& str);
bool IsStringConvertableToFloat(const std::string& str);
