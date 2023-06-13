#include "pch.h"
#include "radar_tab.h"
#include "../imgui/imgui.h"

namespace RadarTab
{
	void Render()
	{
		if (ImGui::BeginTabItem("Radar"))
		{
			ImGui::TextColored(ImColor(255, 0, 0), "Work In Progress");

			ImGui::EndTabItem();
		}
	}
}