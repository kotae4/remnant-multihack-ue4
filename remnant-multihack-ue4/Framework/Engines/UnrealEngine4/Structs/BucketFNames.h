#pragma once

#include "Framework/Engines/UnrealEngine4/Offsets_UE4.h"

namespace UnrealEngine4
{
	typedef char ANSICHAR;
	typedef wchar_t WIDECHAR;

	class FName
	{
	public:
		__int32 NameIndex; //0x0000
		__int32 Number; //0x0004
	}; //Size: 0x0008
	static_assert(sizeof(FName) == 0x8);


	struct FNameEntry
	{
		FNameEntry* HashNext; //0x0000 (don't know what this is used for)
		__int32 Index; //0x0008
		union //0x000C
		{
			char AnsiName[NAME_SIZE]; //0x0000
			wchar_t WideName[NAME_SIZE]; //0x0000
		};


		__forceinline __int32 GetIndex() const
		{
			return Index >> NAME_INDEX_SHIFT;
		}

		__forceinline bool IsWide() const
		{
			return (Index & NAME_WIDE_MASK);
		}
	}; //Size: 0x080C
	static_assert(sizeof(FNameEntry) == 0x810);

	class BucketFNames
	{
	public:
		FNameEntry** Buckets[NAME_MAX_BUCKETS]; //0x0000
		__int32 TotalEntities; //0x0800
		__int32 NumBuckets; //0x0804

		FNameEntry* GetNameFromIndex(int index)
		{
			if ((index < 0) || (index >= TotalEntities)) return nullptr;
			int bucketIndex = index / NAME_BUCKET_SIZE;
			int indexWithinBucket = index % NAME_BUCKET_SIZE;
			if ((bucketIndex < 0) || (bucketIndex > NumBuckets) || (indexWithinBucket < 0) || (indexWithinBucket > NAME_BUCKET_SIZE))
				return nullptr;
			//printf("Trying to get name for index#%d, should be in bucket#%d at element#%d\n", index, bucketIndex, indexWithinBucket);
			FNameEntry* nameEntry = Buckets[bucketIndex][indexWithinBucket];
			//printf("Got FNameEntry @ [%llx]\nString should be @ [%llx]\n", nameEntry, nameEntry->WideName);
			//printf("Name is %s\n", (nameEntry->IsWide() ? "WIDE" : "ANSI"));
			return nameEntry;
		}
	}; //Size: 0x0808
	static_assert(sizeof(BucketFNames) == 0x808);

	extern BucketFNames* GNames;
}