#pragma once

#include "UObject.h"
#include "../Utils/FLinearColor.h"
#include "../Utils/FVector.h"
#include "ArrayTypes.h"


namespace UnrealEngine4
{
	class AHUD : public UObject
	{
	public:
		char pad_0028[8]; //0x0028
		// actually called DrawText in source, but visual studio has a macro for DrawText that gets in the way too often
		void DrawString(void* gameHUD, const FString& Text, FLinearColor TextColor, float ScreenX, float ScreenY, void* Font = NULL, float Scale = 1.f, bool bScalePosition = false);
		FVector Project(void* gameHUD, FVector Location);
	}; //Size: 0x030
	static_assert(sizeof(AHUD) == 0x30);
	typedef void(__thiscall* tPostRender)(void* gameHUD);
}