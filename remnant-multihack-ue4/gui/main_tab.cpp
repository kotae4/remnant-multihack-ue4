#include "pch.h"
#include "main_tab.h"
#include "../imgui/imgui.h"
#include "../Config.h"
#include "../Framework/Utils/Keybinds.h"

namespace MainTab
{
	void Render()
	{
		if (ImGui::BeginTabItem("Settings"))
		{
			ImGui::TextColored(ImColor(255, 255, 255), "Hello World");

			if (ImGui::Button("Reload Config"))
			{
				Config.Load();
			}

			ImGui::EndTabItem();
		}
	}
}