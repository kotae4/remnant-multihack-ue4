#include "pch.h"
#include "CustomImGuiControls.h"
#include "Keybinds.h"
#include "../../imgui/imgui.h"

bool HotKey(uint8_t& key)
{
	ImGui::Text("[ %s ]", KeyBinds::ToString(key));

	if (!ImGui::IsItemHovered())
		return false;

	ImGui::SetTooltip("Press any key to change the keybind, ESC to reset");
	for (uint8_t vKey : KeyBinds::GetValidKeys()) {
		if (KeyBinds::IsKeyDown(vKey)) {
			key = (vKey != VK_ESCAPE ? vKey : 0x00);
			return true;
		}
	}

	return false;
}