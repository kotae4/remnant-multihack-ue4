#pragma once

#include "UObject.h"
#include "../Utils/FVector.h"
#include "../Utils/FLinearColor.h"
#include "../../../Utils/Memory.h"
#include "../Offsets_UE4.h"

namespace UnrealEngine4
{
	class ULineBatchComponent : public UObject
	{
	public:
		// TODO move vtable offset to config, or pattern scan for it
		void DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, unsigned char DepthPriority = 0, float Thickness = 0.0f, float LifeTime = 0.0f) // offset 850h, index 266
		{
			typedef void(__thiscall* OriginalFunc)(void* self, const FVector& start, const FVector& end, const FLinearColor& color, unsigned char depthPriority, float thickness, float lifeTime);
			(Memory::CallVirtual<OriginalFunc>(this, ULINEBATCHCPNT_DRAWLINE_VTABLEINDEX))(this, Start, End, Color, DepthPriority, Thickness, LifeTime);
		}

		void DrawLineDirect(void* LineBatchCpnt, const FVector& Start, const FVector& End, const FLinearColor& Color, unsigned char DepthPriority = 0, float Thickness = 0.0f, float LifeTime = 0.0f);
	};
}