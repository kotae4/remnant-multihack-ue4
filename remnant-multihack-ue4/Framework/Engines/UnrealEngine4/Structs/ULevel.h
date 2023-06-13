#pragma once

#include "UObject.h"

namespace UnrealEngine4
{
	class ULevel : public UObject
	{
	public:
		char pad_0x0028[0x70]; //0x0028
		AActor** ActorList; //0x0098
		__int32 NumActors; //0x00A0
		__int32 MaxActors; //0X00A4
	}; //Size: 0x0A8
	static_assert(sizeof(ULevel) == 0xa8);
}