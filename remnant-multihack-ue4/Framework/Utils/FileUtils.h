#pragma once

#include <string>
#include <map>
#include <vector>
#include <sal.h>

namespace Utils
{
	namespace Files
	{
		bool ParseStringReplacementsFile(std::string filePath, _Out_ std::map<std::wstring, std::wstring>& replacementMap);
		bool ReadLinesFromFileA(std::string filePath, _Out_ std::vector<std::string>& lines);
		bool ReadLinesFromFileW(std::string filePath, _Out_ std::vector<std::wstring>& lines);
	}
};