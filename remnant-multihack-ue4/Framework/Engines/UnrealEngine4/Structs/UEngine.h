#pragma once

#include "UObject.h"

namespace UnrealEngine4
{
	class UEngine : public UObject
	{
	public:
		char pad_0x0028[0x28]; //0x0028
		UObject* SmallFont; //0x0050
		char pad_0x0058[0x18]; //0x0058
		class UObject* MediumFont; //0x0070 
		char pad_0x0078[0x6D8]; //0x0078
		class UObject* GameViewportClient; //0x0750
	}; //Size: 0x0758
	static_assert(sizeof(UEngine) == 0x758);

	extern UEngine* GEngine;
}