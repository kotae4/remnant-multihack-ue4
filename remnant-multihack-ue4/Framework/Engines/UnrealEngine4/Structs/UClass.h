#pragma once

#include "UStruct.h"
#include "UObject.h"
#include "BucketFNames.h"

namespace UnrealEngine4
{
	class FNativeFunctionLookup
	{
	public:
		FName Name; // 0x0000
		void* Pointer; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(FNativeFunctionLookup) == 0x10);


	class UClass : public UStruct //Size: 0x0098
	{
	public:
		char pad_00C8[24]; //0x0098
		unsigned int Possibly_ClassUnique_and_bCooked; //0x00B0
		unsigned int ClassFlags; //0x00B4
		unsigned long long ClassCastFlags; //0x00B8
		class UClass* ClassWithin; //0x00C0
		class UObject* ClassGeneratedBy; //0x00C8
		class FName ClassConfigName; //0x00D0
		char pad_0108[32]; //0x00D8
		class UObject* ClassDefaultObject; //0x00F8
		void* FuncMap; //0x0100
		__int32 FuncMap_Count; //0x0108
		__int32 FuncMap_Max; //0x010C
		char pad_0140[64]; //0x0110
		void* SuperFuncMap; //0x0150
		__int32 SuperFuncMap_Count; //0x0158
		__int32 SuperFuncMap_Max; //0x015C
		char pad_0190[64]; //0x0160
		void* SuperFuncMapLock; //0x01A0
		void* Interfaces; //0x01A8
		__int32 Interfaces_Count; //0x01B0
		__int32 Interfaces_Max; //0x01B4
		char pad_01E8[56]; //0x01B8
		class FNativeFunctionLookup* NativeFunctionLookupTable; //0x01F0
		__int32 NativeFunctionLookupTable_Count; //0x01F8
		__int32 NativeFunctionLookupTable_Max; //0x01FC
	}; //Size: 0x200
	static_assert(sizeof(UClass) == 0x200);
}