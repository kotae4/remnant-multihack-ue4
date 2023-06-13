#pragma once

#include "Framework/Utils/Keybinds.h"
#include "Framework/Engines/UnrealEngine4/Utils/FLinearColor.h"
#include "Framework/Engines/UnrealEngine4/Structs/UClass.h"

enum class ETargetType
{
	Inspectable,
	Checkpoint,
	Item,
	Container,
	Character,
	MAX
};

struct ESPTypeData
{
public:
	UnrealEngine4::UClass* Class = nullptr;
	UnrealEngine4::FLinearColor Color = UnrealEngine4::FLinearColor(1.f, 0.f, 0.f, 1.f);
	const char* ClassName;
	std::vector<std::wstring> IgnoredNames;

	ESPTypeData(const char* className, UnrealEngine4::FLinearColor color) : ClassName(className), Color(color)
	{
		Class = nullptr;
	}
};

#pragma pack(push, 8)
class CConfig
{
private:
	const char* IgnoredCharactersFilename = "ignored_characters.txt";
	// NOTE this was used for debugging, but keeping it just in case
	const char* IgnoredClassesFilename = "ignored_classes.txt";
	const char* IgnoredItemsFilename = "ignored_items.txt";
	const char* NameSubstitutionsFilename = "pretty_name_substitutions.txt";

	bool LoadLooseFiles();
	bool LoadClasses();
public:
	// everything below is serialized
	bool ESPNamesEnabled = true;
	bool ESPBonesEnabled = true;
	int ESPBoneDrawDistance = 4000;
	bool AimbotEnabled = false;
	bool AimbotUseCharacterDistance = true;

	KeyBinds::Config Keybinds = {
		VK_DELETE,
		VK_XBUTTON2,
		VK_HOME
	};

	// everything below is not serialized
	bool MenuEnabled = false;
	std::vector<std::wstring> IgnoredItemNames;
	std::vector<std::wstring> IgnoredClassNames;
	std::vector<std::wstring> IgnoredCharacters;
	std::map<std::wstring, std::wstring> NameSubstitutions;

	// TODO make this better. vector container, read colors from config file, assignment by enum index.
	ESPTypeData ESPClassTypeData[static_cast<int>(ETargetType::MAX)] = {
		ESPTypeData("BlueprintGeneratedClass /Game/Interactives/Inspectable/BP_InspectableAbstract.BP_InspectableAbstract_C", UnrealEngine4::FLinearColor(1.f, 0.f, 0.f, 1.f)),
		ESPTypeData("Class /Script/Remnant.Checkpoint", UnrealEngine4::FLinearColor(1.f, 0.f, 0.f, 1.f)),
		ESPTypeData("Class /Script/GunfireRuntime.Item", UnrealEngine4::FLinearColor(0.f, 1.f, 0.f, 1.f)),
		ESPTypeData("Class /Script/Remnant.LootContainer", UnrealEngine4::FLinearColor(1.f, 1.f, 0.f, 1.f)),
		ESPTypeData("Class /Script/GunfireRuntime.AICharacter", UnrealEngine4::FLinearColor(1.f, 0.f, 0.f, 1.f))
	};

	bool Load();
	bool Save();
};
#pragma pack(pop)

inline CConfig Config;