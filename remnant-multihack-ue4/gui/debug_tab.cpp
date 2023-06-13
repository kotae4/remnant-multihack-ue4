#include "pch.h"
#include "debug_tab.h"
#include "../imgui/imgui.h"

namespace DebugTab
{
	void Render()
	{
		if (ImGui::BeginTabItem("Debug"))
		{
			ImGui::TextColored(ImColor(255, 0, 0), "Hello World");

			ImGui::EndTabItem();
		}
	}
}