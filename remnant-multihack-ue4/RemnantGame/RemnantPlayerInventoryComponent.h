#pragma once

#include "RemnantItem.h"
#include "RemnantItemInstanceData.h"

namespace RemnantGame
{
	struct InventoryItem
	{
	public:
		char pad_0x0000[0x8]; //0x0000
		class RemnantItem* pItem; //0x0008 
		char pad_0x0010[0x8]; //0x0010
		class RemnantItemInstanceData* pItemData; //0x0018 
		char pad_0x0020[0x8]; //0x0020

	}; //Size=0x0028

	class RemnantPlayerInventoryComponent
	{
	public:
		char pad_0x0000[0xE8]; //0x0000
		class UObject* PlayerCharacter; //0x00E8 
		char pad_0x00F0[0xF8]; //0x00F0
		InventoryItem* Items; //0x01E8 
		__int32 NumItems; //0x01F0 
		__int32 MaxItems; //0x01F4 
		char pad_0x01F8[0x288]; //0x01F8

	}; //Size=0x0480
}