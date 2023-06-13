#include "pch.h"
#include "visuals_tab.h"
#include "../Config.h"
#include "../imgui/imgui.h"

namespace VisualsTab
{
	void Render()
	{
		if (ImGui::BeginTabItem("Visuals"))
		{
			ImGui::TextColored(ImColor(255, 0, 0), "Hello World");
			if (ImGui::Checkbox("Draw Names", &Config.ESPNamesEnabled))
			{
				Config.Save();
			}
			if (ImGui::Checkbox("Draw Bones", &Config.ESPBonesEnabled))
			{
				Config.Save();
			}

			if (ImGui::SliderInt("Bone Draw Distance", &Config.ESPBoneDrawDistance, 1000, 10000))
			{
				Config.Save();
			}

			ImGui::EndTabItem();
		}
	}
}