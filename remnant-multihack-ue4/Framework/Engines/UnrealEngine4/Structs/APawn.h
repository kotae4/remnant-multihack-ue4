#pragma once

#include "UObject.h"
#include "AActor.h"
#include "../Utils/FRotator.h"
#include "../../../Utils/Memory.h"

namespace UnrealEngine4
{
	class APawn : public AActor
	{
	public:
		// TODO verify vtable index (and move to '../Offsets_UE4.h')
		void FaceRotation(const FRotator& NewRotation, float DeltaTime) // offset 6d0h, index 218
		{
			typedef void(__thiscall* OriginalFunc)(void* self, const FRotator& newRot, float dTime);
			(Memory::CallVirtual<OriginalFunc>(this, 218))(this, NewRotation, DeltaTime);
		}
	}; //Size: 0x030
	static_assert(sizeof(APawn) == 0x2C0);
}