#include "pch.h"
#include "USkinnedMeshComponent.h"
#include "../../../Utils/Memory.h"
#include "../Offsets_UE4.h"

namespace UnrealEngine4
{
	__int64 USkinnedMeshComponent::GetBoneName(__int32 BoneIndex)
	{
		__int64 retValue = 0;
		__int64 tossValue = -1;
		((int(__thiscall*)(void* object, __int64* retVal, __int32 boneIndex))(Memory::ProcessBaseAddress + USKINNEDMESHCPNT_GETBONENAME_OFFSET))(this, &retValue, BoneIndex);
		return retValue > 0 ? retValue : tossValue;
	}

	FMatrix USkinnedMeshComponent::GetBoneMatrix(__int32 BoneIndex)
	{
		FMatrix retValue = FMatrix();
		((void(__thiscall*)(void* object, FMatrix * retValue, __int32 boneIndex))(Memory::ProcessBaseAddress + USKINNEDMESHCPNT_GETBONEMATRIX_OFFSET))(this, &retValue, BoneIndex);
		return retValue;
	}

	FVector USkinnedMeshComponent::GetBoneLocation_MyImpl(__int32 BoneIndex)
	{
		FMatrix boneMatrix = GetBoneMatrix(BoneIndex);
		return boneMatrix.GetOrigin();
	}
}