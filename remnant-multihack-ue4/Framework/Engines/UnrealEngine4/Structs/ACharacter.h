#pragma once

#include "UObject.h"
#include "APawn.h"

namespace UnrealEngine4
{
	class ACharacter : public APawn
	{
	public:
		char pad_02C0[208]; //0x02C0
		class USkinnedMeshComponent* Mesh; //0x0390
		class N00000C20* CharacterMovement; //0x0398
		class N00000C58* CapsuleComponent; //0x03A0
		class N00000C84* StaticMeshCpnt; //0x03A8
	}; //Size: 0x03B0
	static_assert(sizeof(ACharacter) == 0x3B0);
}