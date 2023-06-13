#pragma once

#include "UObject.h"
#include "ULocalPlayer.h"

namespace UnrealEngine4
{
	class UGameInstance : public UObject
	{
	public:
		char pad_0x0028[0x10]; //0x0028
		ULocalPlayer** LocalPlayers; //0x0038
		__int32 NumLocalPlayers; //0x0040
		__int32 MaxLocalPlayers; //0x0044
	}; //Size: 0x0048
	static_assert(sizeof(UGameInstance) == 0x48);
}