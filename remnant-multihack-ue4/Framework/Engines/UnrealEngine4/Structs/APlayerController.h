#pragma once

#include "UObject.h"

#include "APawn.h"
#include "AHUD.h"
#include "APlayerCameraManager.h"

#include "../Utils/FVector.h"
#include "../Utils/FRotator.h"
#include "../Offsets_UE4.h"
#include "../../../Utils/Memory.h"

namespace UnrealEngine4
{
	class APlayerController : public UObject
	{
	public:
		char pad_0028[824]; //0x0028
		APawn* Pawn; //0x0360
		char pad_0368[88]; //0x0368
		AHUD* HUD; //0x03C0
		APlayerCameraManager* TPSPlayerCameraManager; //0x03C8

		void SetControlRotation(const FRotator& NewRotation) // offset 648h, index 201
		{
			typedef void(__thiscall* OriginalFunc)(void* self, const FRotator& newRot);
			(Memory::CallVirtual<OriginalFunc>(this, APLAYERCONTROLLER_SETCONTROLROTATION_VTABLEINDEX))(this, NewRotation);
		}

		void GetPlayerViewPoint(FVector& Location, FRotator& Rotation) // offset 6d8, index 219
		{
			typedef void(__thiscall* OriginalFunc)(void* self, FVector& loc, FRotator& newRot);
			(Memory::CallVirtual<OriginalFunc>(this, APLAYERCONTROLLER_GETPLAYERVIEWPOINT_VTABLEINDEX))(this, Location, Rotation);
		}

	}; //Size: 0x03D0
	static_assert(sizeof(APlayerController) == 0x3D0);
}