#pragma once

#include "UObject.h"

namespace UnrealEngine4
{
	class UObject;
	#pragma pack(1)
	class UField : public UObject
	{
		UField* Possibly_NextUField; //0x0028
		char pad_0030[0x10]; //0x0030
	}; //Size: 0x0040
	static_assert(sizeof(UObject) == 0x28);
	//static_assert(sizeof(UField) == 0x40);
}