#pragma once

#include "UObject.h"
#include "../Utils/FVector.h"
#include "../Utils/FRotator.h"

namespace UnrealEngine4
{
	class APlayerCameraManager : public UObject
	{
	public:
		char pad_0x0028[0xE98]; //0x0028
		class UObject* VT_Target; //0x0EC0 
		char pad_0x0EC8[0x8]; //0x0EC8
		FVector VT_Location; //0x0ED0 
		FRotator VT_Rotation; //0x0EDC 
		char pad_0x0EE8[0xB38]; //0x0EE8
		float CacheTimestamp; //0x1A20 
		char pad_0x1A24[0xC]; //0x1A24
		FVector Location; //0x1A30 
		FRotator Rotation; //0x1A3C 
		float FOV; //0x1A48 
		float DesiredFOV; //0x1A4C 
		float OrthoWidth; //0x1A50 
		float OrthoNearClipPlane; //0x1A54 
		float OrthoFarClipPlane; //0x1A58 
		float AspectRatio; //0x1A5C
	};
	static_assert(sizeof(APlayerCameraManager) == 0x1A60);
}