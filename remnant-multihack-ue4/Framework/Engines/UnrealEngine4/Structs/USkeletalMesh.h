#pragma once

namespace UnrealEngine4
{
	struct FMeshBoneInfo
	{
		// Bone's name.
		__int32 Name_IndexIntoGNames;

		// 0/NULL if this is the root bone. 
		__int32 ParentIndex;
	};

	struct FReferenceSkeleton
	{
		__int64 FinalRefBoneInfo; // DO NOT USE
		__int32 NumBones;

		//__int64 GetBoneName(const __int32& boneIndex);
	};

	class USkeletalMesh
	{
	public:
		char pad_0x0000[0x190]; //0x0000
		FMeshBoneInfo** FinalRefBoneInfo; //0x0190 
		__int32 NumBones; //0x0198 
		__int32 MaxBones; //0x019C

		// NOTE:
		// this actually returns an FName instance, but an FName is basically just 2 int32's.
		//__int64* GetBoneName(const __int32& boneIndex);

	private:
		USkeletalMesh();
		~USkeletalMesh();
	};
}