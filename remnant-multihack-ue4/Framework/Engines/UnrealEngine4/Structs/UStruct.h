#pragma once

#include "UField.h"
#include "UObject.h"
#include "ArrayTypes.h"
#include "UProperty.h"

namespace UnrealEngine4
{

#pragma pack(push, 8)
	class FStructBaseChain
	{
	public:
		FStructBaseChain** StructBaseChainArray;
		__int32 NumStructBasesInChainMinusOne;

		friend class UStruct;
	};
	static_assert(sizeof(FStructBaseChain) == 0x10);
#pragma pack(pop)

	class UStruct : public UField, public FStructBaseChain
	{
	public:
		UStruct* Super; //0x0040
		UField* Children; //0x0048
		__int32 PropertiesSize; //0x0050
		__int32 MinAlignment; //0x0054
		TArray<uint8_t> ScriptBytecode;
		// these are all pointers to UProperty instances
		UProperty* PropertyLink; //0x0068
		UProperty* RefLink; //0x0070
		UProperty* DestructorLink; //0x0078
		UProperty* PostConstructLink; //0x0080
		TArray<UObject*> ScriptObjectReferences; //0x0088
	}; //Size: 0x0098
	static_assert(sizeof(UStruct) == 0x98);
}