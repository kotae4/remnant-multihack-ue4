#include "pch.h"
#include "FileUtils.h"
#include "Logger.h"
#include "StringUtils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

bool Utils::Files::ParseStringReplacementsFile(std::string filePath, _Out_ std::map<std::wstring, std::wstring>& replacementMap)
{
	replacementMap.clear();

	std::ifstream file(filePath);
	if (file.is_open() == false)
	{
		LOG_ERROR("Could not open file '{}'", filePath);
		return false;
	}

	bool success = true;
	std::string line;
	int lineNumber = 0;
	while (std::getline(file, line))
	{
		std::wstring badNameW, friendlyNameW;
		std::vector<std::string> stringPair = Utils::Strings::SplitA(line, ":");
		if (stringPair.size() != 2)
		{
			LOG_ERROR("Malformed line#{} in string replacements file '{}'", lineNumber, filePath);
			success = false;
			break;
		}

		if (Utils::Strings::TryConvertUtf8ToUtf16(stringPair[0], badNameW) == FALSE)
		{
			LOG_ERROR("Could not convert {} to widestring", stringPair[0]);
			success = false;
			break;
		}
		if (Utils::Strings::TryConvertUtf8ToUtf16(stringPair[1], friendlyNameW) == FALSE)
		{
			LOG_ERROR("Could not convert {} to widestring", stringPair[1]);
			success = false;
			break;
		}

		replacementMap.insert(std::pair<std::wstring, std::wstring>(badNameW, friendlyNameW));
		lineNumber++;
	}

	file.close();
	return success;
}

bool Utils::Files::ReadLinesFromFileA(std::string filePath, _Out_ std::vector<std::string>& lines)
{
	lines.clear();

	std::ifstream file(filePath);
	if (file.is_open() == false)
	{
		LOG_ERROR("Could not open file '{}'", filePath);
		return false;
	}

	bool success = true;
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	file.close();
	return success;
}

bool Utils::Files::ReadLinesFromFileW(std::string filePath, _Out_ std::vector<std::wstring>& lines)
{
	lines.clear();

	std::ifstream file(filePath);
	if (file.is_open() == false)
	{
		LOG_ERROR("Could not open file '{}'", filePath);
		return false;
	}

	bool success = true;
	std::string line;
	std::wstring lineW;
	while (std::getline(file, line))
	{
		if (Utils::Strings::TryConvertUtf8ToUtf16(line, lineW) == FALSE)
		{
			LOG_ERROR("Could not convert {} to widestring", line);
			success = false;
			break;
		}
		lines.push_back(lineW);
	}

	file.close();
	return success;
}