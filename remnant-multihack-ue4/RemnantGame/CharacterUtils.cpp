#include "pch.h"
#include "CharacterUtils.h"
#include "../Framework/Utils/Logger.h"
#include "../Framework/Engines/UnrealEngine4/Core.h"
#include "../Framework/Engines/UnrealEngine4/Structs/UObject.h"
#include "../Framework/Engines/UnrealEngine4/Structs/UWorld.h"
#include "../Framework/Engines/UnrealEngine4/Structs/BucketFNames.h"
#include "../RemnantGame/RemnantCharacter.h"
#include "../RemnantGame/RemnantPlayerInventoryComponent.h"

namespace RemnantGame
{
	bool FindInventoryItemOnCharacter(CharacterGunfire* character, const char* itemName, InventoryItem** outItem)
	{
		if (character == NULL) return false;
		InventoryItem* foundItem = NULL;
		RemnantPlayerInventoryComponent* inv = character->InventoryCpnt;
		if (inv == NULL)
		{
			//LOG_ERROR("Character does not have an inventory");
			return false;
		}
		//LOG_INFO("Saw inventory cpnt at [{:x}] with {} items", (uintptr_t)inv, inv->NumItems);
		for (int itemIndex = 0; itemIndex < inv->NumItems; itemIndex++)
		{
			UnrealEngine4::FNameEntry* invItemName = UnrealEngine4::GNames->GetNameFromIndex(((UnrealEngine4::UObject*)inv->Items[itemIndex].pItem)->Name.NameIndex);
			std::string outputInvItemName;
			if (invItemName->IsWide())
			{
				if (Utils::Strings::TryConvertUtf16ToUtf8(std::wstring(invItemName->WideName), outputInvItemName) == false)
				{
					//LOG_ERRORW(L"Could not convert '{}' to narrow string", invItemName->WideName);
					return false;
				}
			}
			else
			{
				outputInvItemName = std::string(invItemName->AnsiName);
			}
			//LOG_INFO("[{}][{:x}] Saw '{}' (pItem: [{:x}])", itemIndex, (uintptr_t)(&inv->Items[itemIndex]), outputInvItemName.c_str(), (uintptr_t)(inv->Items[itemIndex].pItem));
			if (outputInvItemName.find(itemName) != std::string::npos)
			{
				//LOG_INFO("Adding 50000 to scrap item quantity (was at {})", inv->Items[itemIndex].pItemData->Quantity);
				foundItem = &inv->Items[itemIndex];
				break;
			}
		}
		if (foundItem == NULL) return false;
		*outItem = foundItem;
		return true;
	}
}