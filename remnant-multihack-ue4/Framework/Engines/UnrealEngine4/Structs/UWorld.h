#pragma once

#include "UObject.h"
#include "UGameInstance.h"
#include "ULineBatchComponent.h"

namespace UnrealEngine4
{
	enum ECollisionChannel
	{
		ECC_WorldStatic,
		ECC_WorldDynamic,
		ECC_Pawn,
		ECC_Visibility,
		ECC_Camera
	};

	struct FCollisionQueryParams_DerpModeActive
	{
	public:
		__int64 StatNameIndex; //0x0000 
		__int64 OwnerNameIndex; //0x0008 
		__int8 bTraceComplex; //0x0010 
		__int32 AlwaysOne_i; //0x0011 
		__int8 AlwaysZero_5; //0x0015 
		char pad_0x0016[0x2]; //0x0016
		__int32 AlwaysZero_3; //0x0018 
		__int8 AlwaysZero_4; //0x001C 
		char pad_0x001D[0x1]; //0x001D
		__int8 AlwaysOne_b; //0x001E 
		char pad_0x001F[0x21]; //0x001F
		__int64 AlwaysZero; //0x0040 
		__int64 AlwaysZero_1; //0x0048 
		unsigned __int32 IgnoredActors[4]; //0x0050 
		__int64 AlwaysZero_2; //0x0060 
		__int32 NumIgnoredActors; //0x0068 
		__int32 MaxIgnoredActors; //0x006C 

		FCollisionQueryParams_DerpModeActive();
		FCollisionQueryParams_DerpModeActive(class AActor* player, class AActor* target);

	}; //Size=0x0070

	struct FCollisionResponseParams
	{
		__int64 _padding;
	};

	class UWorld : public UObject
	{
	public:
		char pad_0x0028[0x8]; //0x0028
		UObject* PersistentLevel; //0x0030
		char pad_0x0038[0x8]; //0x0038
		class ULineBatchComponent* LineBatcher; //0x0040
		char pad_0048[0xf0]; //0x0048
		class ULevel** LevelList; //0x0138
		int32_t NumLevels; //0x0140
		int32_t MaxLevels; //0x0144
		char pad_0148[24]; //0x0148
		UGameInstance* OwningGameInstance; //0x0160

		// returns TRUE if a blocking hit is found
		bool LineTraceTestByChannel(const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, const FCollisionQueryParams_DerpModeActive& Params, const FCollisionResponseParams& ResponseParam);

	}; //Size: 0x0168
	static_assert(sizeof(UWorld) == 0x168);


	extern UWorld* GWorld;
	// NOTE: 0x358C300
	extern struct FCollisionResponseParams* GDefaultCollisionResponseParams;
	// NOTE: 0x35C4670
	extern class FName* GLineOfSight_FName;
}