#pragma once

#include "UObject.h"

namespace UnrealEngine4
{
	class BucketUObjects
	{
	public:
		char pad_0000[16]; //0x0000
		class FUObjectItem** Buckets; //0x0010
		char pad_0018[8]; //0x0018
		__int32 MaxObjects; //0x0020
		__int32 NumObjects; //0x0024
		__int32 MaxBuckets; //0x0028
		__int32 NumBuckets; //0x002C
	}; //Size: 0x0030
	static_assert(sizeof(BucketUObjects) == 0x30);

	class FUObjectItem
	{
	public:
		class UObject* Object; //0x0000
		__int32 Flags; //0x0008
		__int32 ClusterRootIndex; //0x000C
		__int32 SerialNumber; //0x0010
		__int32 Padding; //0x0014
	}; //Size: 0x0018
	static_assert(sizeof(FUObjectItem) == 0x18);

	extern BucketUObjects* GObjects;
}