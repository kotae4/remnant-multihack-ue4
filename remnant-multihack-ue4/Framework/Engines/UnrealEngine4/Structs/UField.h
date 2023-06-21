#pragma once

#include "UObject.h"

namespace UnrealEngine4
{
	class UObject;
	#pragma pack(1)
	class UField : public UObject
	{
		UField* Possibly_NextUField; //0x0028
	}; //Size: 0x0030
	static_assert(sizeof(UField) == 0x30);
}