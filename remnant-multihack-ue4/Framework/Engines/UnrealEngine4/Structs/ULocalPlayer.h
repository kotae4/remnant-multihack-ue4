#pragma once

#include "UObject.h"
#include "APlayerController.h"
#include "UGameViewportClient.h"

namespace UnrealEngine4
{
	class ULocalPlayer : public UObject
	{
	public:
		char pad_0x0028[0x8]; //0x0028
		APlayerController* PlayerController; //0x0030
		char pad_0038[56]; //0x0038
		UGameViewportClient* ViewportClient; //0x0070
	}; //Size: 0x0078
	static_assert(sizeof(ULocalPlayer) == 0x78);
}