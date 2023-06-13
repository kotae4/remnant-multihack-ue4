#pragma once

#include "USkeletalMesh.h"
#include "../Utils/FMatrix.h"
#include "../Utils/FVector.h"

namespace UnrealEngine4
{
	class USkinnedMeshComponent
	{
	public:
	public:
		char pad_0x0000[0x5A0]; //0x0000
		class USkeletalMesh* SkeletalMesh; //0x05A0 

		//FVector GetBoneLocation(__int64 boneNameIndex);
		__int64 GetBoneName(__int32 boneIndex);
		FMatrix GetBoneMatrix(__int32 boneIndex);

		FVector GetBoneLocation_MyImpl(__int32 boneIndex);

	private:
		USkinnedMeshComponent();
		~USkinnedMeshComponent();
	};
}