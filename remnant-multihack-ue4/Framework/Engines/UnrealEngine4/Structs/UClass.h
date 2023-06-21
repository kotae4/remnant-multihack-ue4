#pragma once

#include "UStruct.h"
#include "UObject.h"
#include "BucketFNames.h"
#include "UProperty.h"
#include "ArrayTypes.h"

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
		void* ClassConstructorFunc;
		void* ClassVTableHelperCtorCallerFunc;
		void* ClassAddReferencedObjectsFunc;
		unsigned int ClassUniqueCounter;
		unsigned int bCooked;
		unsigned int ClassFlags;
		unsigned int ClassCastFlags;
		UClass* ClassWithin; //0x00C0
		UObject* ClassGeneratedBy; //0x00C8
		FName ClassConfigName; //0x00D0
		TArray<UProperty*> ClassReps; //0x00D8
		TArray<UField*> NetFields; //0x00E8
		UObject* ClassDefaultObject; //0x00F8
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