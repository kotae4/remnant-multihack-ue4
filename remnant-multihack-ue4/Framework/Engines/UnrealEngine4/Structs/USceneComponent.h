#pragma once

#include "UObject.h"
#include "../Utils/FVector.h"

namespace UnrealEngine4
{
	class USceneComponent : public UObject
	{
	public:
		char pad_0028[376]; //0x0028
		FVector Position; //0x01A0
	}; //Size: 0x01AC
	static_assert(sizeof(USceneComponent) == 0x1AC);
}