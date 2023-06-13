#include "pch.h"
#include "ULineBatchComponent.h"
#include "../Utils/FVector.h"
#include "../Utils/FLinearColor.h"
#include "../Offsets_UE4.h"

namespace UnrealEngine4
{
	void ULineBatchComponent::DrawLineDirect(void* LineBatchCpnt, const FVector& Start, const FVector& End, const FLinearColor& Color, unsigned char DepthPriority, float Thickness, float LifeTime)
	{
		return ((void(__thiscall*)(void* _LineBatchCpnt, const FVector & _Start, const FVector & _End, const FLinearColor & _Color, unsigned char _DepthPriority, float _Thickness, float _LifeTime))(Memory::ProcessBaseAddress + ULINEBATCHCPNT_DRAWLINE_OFFSET))(LineBatchCpnt, Start, End, Color, DepthPriority, Thickness, LifeTime);
	}
}