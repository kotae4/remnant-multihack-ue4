#include "pch.h"
#include "debug_tab.h"
#include "../imgui/imgui.h"
#include "../Framework/Utils/Logger.h"
#include "../Framework/Engines/UnrealEngine4/Core.h"

using namespace UnrealEngine4;

namespace DebugTab
{
	void DbgPrintProperties()
	{
		ULocalPlayer* LocalPlayer = GWorld->OwningGameInstance->LocalPlayers[0];
		APawn* LocalPlayerPawn = LocalPlayer->PlayerController->Pawn;

		UClass* PawnClass = LocalPlayerPawn->Class;
		FString PawnClassName = PawnClass->GetFullName(NULL);
		LOG_INFOW(L"{}", PawnClassName.c_str());
		for (UProperty* curProperty = PawnClass->PropertyLink; curProperty != NULL; curProperty = curProperty->PropertyLinkNext)
		{
			UnrealEngine4::FNameEntry* propertyFName = UnrealEngine4::GNames->GetNameFromIndex(curProperty->Name.NameIndex);
			std::string propertyName;
			if (propertyFName->IsWide())
			{
				if (Utils::Strings::TryConvertUtf16ToUtf8(std::wstring(propertyFName->WideName), propertyName) == false)
				{
					LOG_ERRORW(L"Could not convert '{}' to narrow string", propertyFName->WideName);
					return;
				}
			}
			else
			{
				propertyName = std::string(propertyFName->AnsiName);
			}

			LOG_INFO("--{}", propertyName.c_str());
		}
	}

	void Render()
	{
		if (ImGui::BeginTabItem("Debug"))
		{
			ImGui::TextColored(ImColor(255, 0, 0), "Hello World");
			if (ImGui::Button("Print Properties"))
				DbgPrintProperties();

			ImGui::EndTabItem();
		}
	}
}