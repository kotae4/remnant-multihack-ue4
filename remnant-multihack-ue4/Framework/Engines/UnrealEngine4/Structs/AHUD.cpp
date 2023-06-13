#include "pch.h"
#include "AHUD.h"

#include "../Offsets_UE4.h"
#include "../../../Utils/Memory.h"

namespace UnrealEngine4
{
	//typedef FVector(__thiscall *tProject)(FVector Location);
	FVector AHUD::Project(void* HUD, FVector Location)
	{
		FVector retVal(0.f, 0.f, 0.f);
		((void(__thiscall*)(void* gameHUD, FVector * outScreenPos, FVector worldPos))(Memory::ProcessBaseAddress + AHUD_PROJECT_OFFSET))(HUD, &retVal, Location);
		return retVal;
	}

	void AHUD::DrawString(void* gameHUD, const FString& Text, FLinearColor TextColor, float ScreenX, float ScreenY, void* Font /* = NULL*/, float Scale /* = 1.f*/, bool bScalePosition /* = false*/)
	{
		return ((void(__thiscall*)(void* HUD, const FString & Text, FLinearColor TextColor, float ScreenX, float ScreenY, void* Font, float Scale, bool bScalePosition))(Memory::ProcessBaseAddress + AHUD_DRAWTEXT_OFFSET))(gameHUD, Text, TextColor, ScreenX, ScreenY, Font, Scale, bScalePosition);
	}
}