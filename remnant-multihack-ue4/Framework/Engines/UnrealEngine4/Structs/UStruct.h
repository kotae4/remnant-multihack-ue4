#pragma once

#include "UField.h"

namespace UnrealEngine4
{
	class UStruct : public UField
	{
	public:
		UStruct* Super; //0x0040
		UField* Children; //0x0048
		__int32 PropertiesSize; //0x0050
		__int32 MinAlignment; //0x0054
		char pad_0018[16]; //0x0058
		// these are all pointers to UProperty instances
		void* Possibly_PropertyLink; //0x0068
		void* RefLink; //0x0070
		void* DestructorLink; //0x0078
		void* PostConstructLink; //0x0080
		char pad_0048[16]; //0x0088
	}; //Size: 0x0098
	static_assert(sizeof(UStruct) == 0x98);
}