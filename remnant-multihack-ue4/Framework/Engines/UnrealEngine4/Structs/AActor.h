#pragma once

#include "UObject.h"
#include "USceneComponent.h"

namespace UnrealEngine4
{
	class AActor : public UObject
	{
	public:
		char pad_0028[304]; //0x0028
		class USceneComponent* RootComponent; //0x0158
		char pad_0160[320]; //0x0160
		// is actually a UActorComponent
		UObject** OwnedComponents; //0x02A0
		__int32 NumOwnedComponents; //0x02A8
		__int32 MaxOwnedComponents; //0x02AC
		// is actually a UActorComponent
		UObject** InstanceComponents; //0x02B0
		__int32 NumInstanceComponents; //0x02B8
		__int32 MaxInstanceComponents; //0x02BC
	}; //Size: 0x02C0
	static_assert(sizeof(AActor) == 0x2C0);
}