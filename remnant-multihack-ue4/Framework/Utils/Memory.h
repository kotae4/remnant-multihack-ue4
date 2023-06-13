#pragma once

namespace Memory
{
	namespace
	{
		// credit: honestly no idea. i can't find the original author. kokole maybe?
		bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	}
	// credit: honestly no idea. i can't find the original author. kokole maybe?
	bool FindPattern(DWORD64 dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask, DWORD64* outAddr);

	bool Is64Bit();

	template<typename Function>
	Function CallVirtual(PVOID Base, DWORD Index)
	{
		PDWORD64* VTablePointer = (PDWORD64*)Base;
		PDWORD64 VTableFunctionBase = *VTablePointer;
		DWORD64 dwAddress = VTableFunctionBase[Index];

		return (Function)(dwAddress);
	}

	extern HMODULE Module;
	extern DWORD64 ProcessBaseAddress;
}