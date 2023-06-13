#pragma once

#include <string>
#include <sstream>
#include <map>
#include <Windows.h>

namespace Utils
{
	class Profiler
	{
	public:
		static void InitProfiling();
		static void BeginSample(const char* Stat_Name);
		static void EndSample(const char* Stat_Name);
		static void GetStat(const char* Stat_Name, __int64& AverageSample_Microseconds, __int64& LongestSample_Microseconds, __int64& TotalSamples);
		static const char* GetFormattedStatString(const char* Stat_Name);
		static const wchar_t* GetFormattedStatStringWide(const char* Stat_Name);
		static void AppendStatStringStreamWide(const char* Stat_Name, std::wstringstream& wss);
		static void WriteStatsToStream(std::wstringstream& wss);
		static void ClearStat(const char* Stat_Name);

		static bool HasInitialized;

	private:
		struct StatObject
		{
			__int64 AverageSample_Microseconds = 0;
			__int64 LongestSample_Microseconds = 0;
			__int64 TotalSamples = 0;
			// 
			LARGE_INTEGER QPCStart, QPCEnd;
			__int64 CumulativeTotal = 0;
			__int64 ElapsedMicroseconds = 0;
		};
		Profiler() {};
		static LARGE_INTEGER QPCFrequency;
		static std::map<std::string, StatObject> StatMap;
	};
}