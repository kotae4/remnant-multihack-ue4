#include "pch.h"
#include "UWorld.h"

#include "../Offsets_UE4.h"

namespace UnrealEngine4
{
	FCollisionQueryParams_DerpModeActive::FCollisionQueryParams_DerpModeActive()
	{
		StatNameIndex = GLineOfSight_FName->NameIndex;
		OwnerNameIndex = 0;
		bTraceComplex = 1;
		AlwaysOne_i = 1;
		AlwaysOne_b = 1;
		AlwaysZero = 0;
		AlwaysZero_1 = 0;
		AlwaysZero_2 = 0;
		AlwaysZero_3 = 0;
		AlwaysZero_4 = 0;
		AlwaysZero_5 = 0;
		NumIgnoredActors = 0;
		MaxIgnoredActors = 4;
	}

	FCollisionQueryParams_DerpModeActive::FCollisionQueryParams_DerpModeActive(AActor* player, AActor* target)
	{
		StatNameIndex = GLineOfSight_FName->NameIndex;
		OwnerNameIndex = target->Index;
		bTraceComplex = 1;
		AlwaysOne_i = 1;
		AlwaysOne_b = 1;
		AlwaysZero = 0;
		AlwaysZero_1 = 0;
		AlwaysZero_2 = 0;
		AlwaysZero_3 = 0;
		AlwaysZero_4 = 0;
		AlwaysZero_5 = 0;
		NumIgnoredActors = 2;
		MaxIgnoredActors = 4;
		IgnoredActors[0] = target->Index;
		IgnoredActors[1] = player->Index;
	}


	bool UWorld::LineTraceTestByChannel(const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const FCollisionQueryParams_DerpModeActive& Params, const FCollisionResponseParams& ResponseParam)
	{
		// TODO move hardcoded offset to config or pattern scan for it
		return ((bool(__thiscall*)(void* world, const FVector & _Start, const FVector & _End, ECollisionChannel _TraceChannel, const FCollisionQueryParams_DerpModeActive & _Params, const FCollisionResponseParams & _ResponseParam))(Memory::ProcessBaseAddress + UWORLD_LINETRACETESTBYCHANNEL_OFFSET))(this, Start, End, TraceChannel, Params, ResponseParam);
	}
}