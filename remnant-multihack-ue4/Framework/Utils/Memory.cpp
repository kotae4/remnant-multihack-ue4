#include "pch.h"
#include "Memory.h"
#include "Logger.h"

namespace Memory
{
	namespace
	{
		bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
		{
			for (; *szMask; ++szMask, ++pData, ++bMask)
				if (*szMask == 'x' && *pData != *bMask)   return 0;
			return (*szMask) == NULL;
		}
	}

	bool FindPattern(DWORD64 dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask, DWORD64* outAddr)
	{
		LOG_TRACE("[FINDPATTERN] Finding pattern {} [{}]\n", dwAddress, dwLen);
		for (DWORD64 i = 0; i < dwLen; i++)
		{
			if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			{
				*outAddr = (DWORD64)(dwAddress + i);
				LOG_TRACE("[FINDPATTERN] Found pattern! Returning.\n");
				return true;
			}
		}
		*outAddr = 0;
		LOG_TRACE("[FINDPATTERN] Could not find pattern\n");
		return false;
	}

	bool Is64Bit()
	{
		// strategy#1: check PE header of main module
		IMAGE_DOS_HEADER* dosHdr = (IMAGE_DOS_HEADER*)GetModuleHandle(NULL);
		IMAGE_NT_HEADERS* ntHdr = (IMAGE_NT_HEADERS*)((BYTE*)dosHdr + dosHdr->e_lfanew);
		return ntHdr->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64;

		// strategy#2: probably call IsWoW64 or something, i dunno. implement it when it's needed.
	}
}