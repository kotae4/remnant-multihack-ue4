#include "pch.h"
#include "Config.h"
#include "Framework/Utils/Logger.h"
#include "Framework/Utils/json.hpp"
#include "Framework/Utils/FileUtils.h"
#include "Framework/Engines/UnrealEngine4/Structs/UObject.h"

bool CConfig::Load()
{
    if (std::filesystem::exists("settings.json") == false)
    {
        LOG_ERROR("settings.json doesn't exist");
        return false;
    }

#define JSON_TRYGET(key, value) \
        try { \
            j.at(key).get_to(value); \
        } catch (nlohmann::detail::out_of_range& e) { \
            LOG_ERROR(e.what()); \
        }

    try 
    {
        std::ifstream inSettings("settings.json");
        nlohmann::ordered_json j = nlohmann::ordered_json::parse(inSettings, NULL, false);

        JSON_TRYGET("ESPNamesEnabled", this->ESPNamesEnabled);
        JSON_TRYGET("ESPBonesEnabled", this->ESPBonesEnabled);
        JSON_TRYGET("ESPBoneDrawDistance", this->ESPBoneDrawDistance);
        JSON_TRYGET("AimbotEnabled", this->AimbotEnabled);
        JSON_TRYGET("AimbotUseCharacterDistance", this->AimbotUseCharacterDistance);
        JSON_TRYGET("Keybinds", this->Keybinds);
    }
    catch (...) {
        LOG_ERROR("Unable to parse settings.json");
        return false;
    }

    if (LoadLooseFiles() == false)
    {
        LOG_ERROR("Failed to load loose config files (are they in the game directory?)\n\t--{}\n\t--{}\n\t--{}", IgnoredItemsFilename, IgnoredCharactersFilename, NameSubstitutionsFilename);
        return false;
    }

    if (LoadClasses() == false)
    {
        LOG_ERROR("Failed to load ESPTargetType classes");
        return false;
    }

    return true;
}

bool CConfig::Save()
{
    try {
        nlohmann::ordered_json j = nlohmann::ordered_json{
            {"ESPNamesEnabled", this->ESPNamesEnabled},
            {"ESPBonesEnabled", this->ESPBonesEnabled},
            {"ESPBoneDrawDistance", this->ESPBoneDrawDistance},
            {"AimbotEnabled", this->AimbotEnabled},
            {"AimbotUseCharacterDistance", this->AimbotUseCharacterDistance},
            {"Keybinds", this->Keybinds}
        };
        std::ofstream outSettings("settings.json");
        outSettings << std::setw(4) << j << std::endl;
        return true;
    }
    catch (...) {
        LOG_ERROR("Unable to save settings.json");
        return false;
    }
    return false;
}

bool CConfig::LoadLooseFiles()
{
    // Load ignored class & character names from files
    if (Utils::Files::ReadLinesFromFileW(IgnoredItemsFilename, ESPClassTypeData[static_cast<int>(ETargetType::Item)].IgnoredNames) == false)
    {
        LOG_ERROR("Could not read {}. Is it in the game directory?", IgnoredItemsFilename);
        return false;
    }
    if (Utils::Files::ReadLinesFromFileW(IgnoredCharactersFilename, ESPClassTypeData[static_cast<int>(ETargetType::Character)].IgnoredNames) == false)
    {
        LOG_ERROR("Could not read {}. Is it in the game directory?", IgnoredCharactersFilename);
        return false;
    }

    // Load name replacements from files (ugly name -> pretty name)
    if (Utils::Files::ParseStringReplacementsFile(NameSubstitutionsFilename, NameSubstitutions) == false)
    {
        LOG_ERROR("Could not read {}. Is it in the game directory?", NameSubstitutionsFilename);
        return false;
    }
    return true;
}

bool CConfig::LoadClasses()
{
    // TODO make this better. usages of the class should access it through a helper function which wraps this logic.
    // possibly make it a framework function.
    bool success = true;
    for (int whitelistIndex = 0; whitelistIndex < static_cast<int>(ETargetType::MAX); whitelistIndex++)
    {
        ESPClassTypeData[whitelistIndex].Class = UnrealEngine4::UObject::GetStaticClass(ESPClassTypeData[whitelistIndex].ClassName);
        if (ESPClassTypeData[whitelistIndex].Class == nullptr)
        {
            LOG_ERROR("WARNING: Could not find static class for WHITELISTED '{}'", ESPClassTypeData[whitelistIndex].ClassName);
            success = false;
        }
    }
    return success;
}