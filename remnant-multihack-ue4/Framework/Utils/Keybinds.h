/*
* Credit: std-nullptr, et al
* Source: https://github.com/BitCrackers/AmongUsMenu/blob/eaccb54b3bf91df6c9a59f46ab06516b49c7c3d3/user/keybinds.h
* License: GNU General Public License v3.0 [https://github.com/BitCrackers/AmongUsMenu/blob/eaccb54b3bf91df6c9a59f46ab06516b49c7c3d3/LICENSE]
* Changes: Can be seen by performing a diff against the above permalinked source
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include "json.hpp"

namespace KeyBinds {
    struct Config {
        uint8_t Toggle_Menu;
        uint8_t Toggle_Aimbot;
        uint8_t Toggle_Radar;
    };

    void WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    const char* ToString(uint8_t key);
    std::vector<uint8_t> GetValidKeys();
    bool IsKeyDown(uint8_t key);
    bool IsKeyPressed(uint8_t key);
    bool IsKeyReleased(uint8_t key);

    void to_json(nlohmann::ordered_json& j, KeyBinds::Config value);
    void from_json(const nlohmann::ordered_json& j, KeyBinds::Config& value);
}