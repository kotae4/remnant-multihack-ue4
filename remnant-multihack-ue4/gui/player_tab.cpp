#include "pch.h"
#include "player_tab.h"
#include "../imgui/imgui.h"
#include "../Framework/Utils/Logger.h"
#include "../Framework/Engines/UnrealEngine4/Core.h"
#include "../RemnantGame/RemnantCharacter.h"
#include "../RemnantGame/RemnantPlayerInventoryComponent.h"
#include "../RemnantGame/CharacterUtils.h"

namespace PlayerTab
{
	void Render()
	{
		static bool error = false;
		static int desiredScrap = 0.f;
		if (ImGui::BeginTabItem("Player"))
		{
			ImGui::SliderInt("Scrap", &desiredScrap, -5000, 5000, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::SameLine();
			if (ImGui::Button("Add"))
			{
				RemnantGame::CharacterGunfire* localPlayerCharacter = (RemnantGame::CharacterGunfire*)UnrealEngine4::GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn;
				if (localPlayerCharacter != NULL)
				{
					RemnantGame::InventoryItem* scrap = NULL;
					if (RemnantGame::FindInventoryItemOnCharacter(localPlayerCharacter, "Resource_Scraps_C", &scrap))
					{
						scrap->pItemData->Quantity += desiredScrap;
						error = false;
					}
					else
					{
						error = true;
					}
				}
			}
			if (error == true)
				ImGui::TextColored(ImColor(255, 255, 255), "Could not find Scrap in player inventory...");

			ImGui::EndTabItem();
		}
	}
}