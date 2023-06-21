#pragma once

#include "UField.h"
#include "BucketFNames.h"

namespace UnrealEngine4
{
#pragma pack(push, 8)
	class UProperty : public UField
	{
	public:
		__int32 ArrayDim; //0x0030
		__int32 ElementSize; //0x0034
		__int64 PropertyFlags; //0x0038
		unsigned short RepIndex; //0x0040
		unsigned char BlueprintReplicationCondition; //0x0042
		__int32 Offset_Internal; //0x0044
		FName RepNotifyFuncName; //0x0048
		UProperty* PropertyLinkNext; //0x0050
		UProperty* NextRef; //0x0058
		UProperty* DestructorLinkNext; //0x0060
		UProperty* PostConstructLinkNext; //0x0068

	}; //Size: 0x0070
	static_assert(sizeof(UProperty) == 0x70);
#pragma pack(pop)
}