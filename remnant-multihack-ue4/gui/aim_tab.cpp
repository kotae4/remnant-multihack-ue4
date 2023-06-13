#include "pch.h"
#include "aim_tab.h"
#include "../Config.h"
#include "../imgui/imgui.h"
#include "../Framework/Utils/CustomImGuiControls.h"

namespace AimTab
{
	void Render()
	{
		if (ImGui::BeginTabItem("Aiming"))
		{
			ImGui::Text("Toggle Keybind");
			ImGui::SameLine();
			if (HotKey(Config.Keybinds.Toggle_Aimbot))
			{
				Config.Save();
			}

			if (ImGui::Checkbox("Use Aim Assist", &Config.AimbotEnabled))
			{
				Config.Save();
			}

			if (ImGui::Checkbox("Use Character Distance (Work In Progress)", &Config.AimbotUseCharacterDistance))
			{
				Config.Save();
			}

			ImGui::EndTabItem();
		}
	}
}