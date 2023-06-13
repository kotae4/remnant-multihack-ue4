#include "pch.h"
#include "Profiler.h"
#include "Logger.h"

namespace Utils
{
	std::map<std::string, Profiler::StatObject> Profiler::StatMap;
	LARGE_INTEGER Profiler::QPCFrequency;
	bool Profiler::HasInitialized = false;

	void Profiler::InitProfiling()
	{
		QueryPerformanceFrequency(&QPCFrequency);
		StatMap = std::map<std::string, StatObject>();
		HasInitialized = true;
		LOG_INFO("Initialized profiler");
	}

	void Profiler::BeginSample(const char* Stat_Name)
	{
		std::map<std::string, StatObject>::iterator it = StatMap.find(Stat_Name);
		StatObject* stat = nullptr;
		if (it == StatMap.end())
		{
			LOG_INFO("Creating new stat object for '{}'", Stat_Name);
			std::pair<std::map<std::string, StatObject>::iterator, bool> result = StatMap.insert(std::map<std::string, StatObject>::value_type(Stat_Name, StatObject()));
			if (result.second)
				stat = &result.first->second;
		}
		else
		{
			stat = &it->second;
		}
		if (stat != nullptr)
			QueryPerformanceCounter(&stat->QPCStart);
		else
			LOG_ERROR("Could not create stat object for '{}'", Stat_Name);
	}

	void Profiler::EndSample(const char* Stat_Name)
	{
		std::map<std::string, StatObject>::iterator it = StatMap.find(Stat_Name);
		if (it != StatMap.end())
		{
			StatObject* stat = &it->second;

			QueryPerformanceCounter(&stat->QPCEnd);

			stat->TotalSamples++;
			stat->ElapsedMicroseconds = (((stat->QPCEnd.QuadPart - stat->QPCStart.QuadPart) * 1000000ll) / Profiler::QPCFrequency.QuadPart);
			stat->CumulativeTotal += stat->ElapsedMicroseconds;
			stat->AverageSample_Microseconds = stat->CumulativeTotal / stat->TotalSamples;
			if (stat->ElapsedMicroseconds > stat->LongestSample_Microseconds) stat->LongestSample_Microseconds = stat->ElapsedMicroseconds;
			//printf("Ended sample for stat '%s', took %llu ticks\n", Stat_Name, &stat->QPCEnd.QuadPart);
		}
		else
		{
			LOG_INFO("Could not find stat '{}'", Stat_Name);
		}
	}

	void Profiler::GetStat(const char* Stat_Name, __int64& AverageSample_Microseconds, __int64& LongestSample_Microseconds, __int64& TotalSamples)
	{
		// NOTE:
		// if it doesn't exist, the [] operator should create it and return the newly created instance
		// so this shouldn't give any errors
		StatObject* stat = &StatMap[Stat_Name];
		AverageSample_Microseconds = stat->AverageSample_Microseconds;
		LongestSample_Microseconds = stat->LongestSample_Microseconds;
		TotalSamples = stat->TotalSamples;
	}

	const char* Profiler::GetFormattedStatString(const char* Stat_Name)
	{
		StatObject* stat = &StatMap[Stat_Name];
		std::stringstream profilingSS;
		profilingSS << stat->AverageSample_Microseconds << " us (" << (stat->AverageSample_Microseconds / 1000ll) << " ms)"
			<< "[longest: " << stat->LongestSample_Microseconds << " us][" << stat->TotalSamples << "]";

		return profilingSS.str().c_str();
	}

	const wchar_t* Profiler::GetFormattedStatStringWide(const char* Stat_Name)
	{
		StatObject* stat = &StatMap[Stat_Name];
		std::wstringstream profilingWSS;
		profilingWSS << stat->AverageSample_Microseconds << L" us (" << (stat->AverageSample_Microseconds / 1000ll) << L" ms)"
			<< L"[longest: " << stat->LongestSample_Microseconds << L" us][" << stat->TotalSamples << L"]";

		return profilingWSS.str().c_str();
	}

	void Profiler::AppendStatStringStreamWide(const char* Stat_Name, std::wstringstream& wss)
	{
		StatObject* stat = &StatMap[Stat_Name];
		wss << Stat_Name << stat->AverageSample_Microseconds << L" us (" << (stat->AverageSample_Microseconds / 1000ll) << L" ms)"
			<< L"[longest: " << stat->LongestSample_Microseconds << L" us][" << stat->TotalSamples << L"]\n";
	}

	void Profiler::WriteStatsToStream(std::wstringstream& wss)
	{
		//printf("Writing stats to stream\n");
		std::map<std::string, StatObject>::iterator it;
		for (it = StatMap.begin(); it != StatMap.end(); ++it)
		{
			StatObject* stat = &it->second;
			wss << it->first.c_str() << L": " << stat->AverageSample_Microseconds << L" us (" << (stat->AverageSample_Microseconds / 1000ll) << L" ms)"
				<< L"[longest: " << stat->LongestSample_Microseconds << L" us][" << stat->TotalSamples << L"]\n";
			//printf("----Appended stat '%s' (%lld - %lld - %lld - %lld)\n", it->first, stat->AverageSample_Microseconds, (stat->AverageSample_Microseconds / 1000ll), stat->LongestSample_Microseconds, stat->TotalSamples);
		}
		//printf("Done writing stats to stream\n");
	}

	void Profiler::ClearStat(const char* Stat_Name)
	{
		auto it = StatMap.find(Stat_Name);
		if (it != StatMap.end())
		{
			StatObject* stat = &it->second;
			stat->TotalSamples = 0;
			stat->CumulativeTotal = 0;
			stat->AverageSample_Microseconds = 0;
			stat->LongestSample_Microseconds = 0;
			stat->QPCStart.QuadPart = 0;
			stat->QPCEnd.QuadPart = 0;
			stat->ElapsedMicroseconds = 0;
		}
	}
}