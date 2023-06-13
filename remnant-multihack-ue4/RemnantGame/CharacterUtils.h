#pragma once

#include "RemnantCharacter.h"
#include "RemnantPlayerInventoryComponent.h"

namespace RemnantGame
{
	bool FindInventoryItemOnCharacter(CharacterGunfire* character, const char* itemName, InventoryItem** outItem);
}