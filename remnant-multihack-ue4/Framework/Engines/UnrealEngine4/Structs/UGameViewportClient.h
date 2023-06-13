#pragma once

#include "UObject.h"
#include "../Utils/FVector2D.h"

namespace UnrealEngine4
{
	class FViewport
	{
	public:
		char pad_0x0000[0x48]; // 0x0000
		FVector2D Size;
	};

	class UGameViewportClient : public UObject
	{
	public:
		char pad_0x0028[0x80]; //0x0028
		FViewport* Viewport; //0x00A8
	}; //Size: 0x00B0
	static_assert(sizeof(UGameViewportClient) == 0xB0);
}