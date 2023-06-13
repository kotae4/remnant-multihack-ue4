#include "pch.h"
#include "Core.h"

#include "../../Utils/Memory.h"
#include "../../Utils/StringUtils.h"
#include "../../Utils/Logger.h"

namespace UnrealEngine4
{
	BucketUObjects* GObjects = NULL;
	BucketFNames* GNames = NULL;
	UEngine* GEngine = NULL;
	UWorld* GWorld = NULL;
	FCollisionResponseParams* GDefaultCollisionResponseParams = NULL;
	FName* GLineOfSight_FName = NULL;

	bool Initialize()
	{
		// TODO add signatures instead of hardcoded offsets.
		// could add several of each and try until one works for resiliency between engine versions too.
		GObjects = (BucketUObjects*)(Memory::ProcessBaseAddress + GOBJECTS_OFFSET);
		GNames = *(BucketFNames**)(Memory::ProcessBaseAddress + GNAMES_OFFSET);

		GEngine = *(UEngine**)(Memory::ProcessBaseAddress + GENGINE_OFFSET);
		GWorld = *(UWorld**)(Memory::ProcessBaseAddress + GWORLD_OFFSET);

		GDefaultCollisionResponseParams = (FCollisionResponseParams*)(Memory::ProcessBaseAddress + GDEFAULTCOLLISIONRESPONSEPARAMS_OFFSET);
		GLineOfSight_FName = (FName*)(Memory::ProcessBaseAddress + GLINEOFSIGHT_FNAME_OFFSET);

		// TODO add thorough memory validation here. make sure the address and the pointer values are in valid memory regions.
		if (GObjects == NULL)
		{
			LOG_ERROR("GObjects was invalid. Update Offsets_UE4.h");
			return false;
		}
		if (GNames == NULL)
		{
			LOG_ERROR("GNames was invalid. Update Offsets_UE4.h");
			return false;
		}
		if (GEngine == NULL)
		{
			LOG_ERROR("GEngine was invalid. Update Offsets_UE4.h");
			return false;
		}
		if (GWorld == NULL)
		{
			LOG_ERROR("GWorld was invalid. Update Offsets_UE4.h");
			return false;
		}
		if (GWorld->OwningGameInstance == NULL)
		{
			LOG_ERROR("GWorld->OwningGameInstance was invalid. Update structs");
			return false;
		}
		if ((GWorld->OwningGameInstance->LocalPlayers == NULL) || (GWorld->OwningGameInstance->LocalPlayers[0] == NULL))
		{
			LOG_ERROR("GWorld->OwningGameInstance->LocalPlayers was invalid. Update structs");
			return false;
		}
		if (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController == NULL)
		{
			LOG_ERROR("GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController was invalid. Update structs");
			return false;
		}
		if (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->HUD == NULL)
		{
			LOG_ERROR("GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->HUD was invalid. Update structs");
			return false;
		}
		if (GDefaultCollisionResponseParams == NULL)
		{
			LOG_ERROR("GDefaultCollisionResponseParams was invalid. Update structs");
			return false;
		}
		if (GLineOfSight_FName == NULL)
		{
			LOG_ERROR("GLineOfSight_FName was invalid. Update structs");
			return false;
		}

		LOG_INFO("Saw:\n\t--GObjects at {:#x}\n\t--GNames at {:#x}\n\t--GEngine at {:#x}\n\t--GWorld at {:#x}\n\t--Local player at {:#x}\n\t--PlayerController at {:#x}\n\t--HUD at {:#x}",
				 (uintptr_t)GObjects,
				 (uintptr_t)GNames,
				 (uintptr_t)GEngine,
				 (uintptr_t)GWorld,
				 (uintptr_t)UnrealEngine4::GWorld->OwningGameInstance->LocalPlayers[0],
				 (uintptr_t)UnrealEngine4::GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController,
				 (uintptr_t)UnrealEngine4::GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->HUD);
		return true;
	}

	bool HookPostRender(UnrealEngine4::tPostRender hkPostRender, UnrealEngine4::tPostRender* originalFunction)
	{
		// TODO add proper, thread-safe hooking
		// VMT Hook: hook the PostRender function in our localplayer's HUD object
		PDWORD64* VTablePointer = (PDWORD64*)UnrealEngine4::GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->HUD;
		PDWORD64 VTableFunctionBase = *VTablePointer;
		LOG_TRACE("MyHUD VTablePtr: [{:#x}]\nMyHUD VTableBase: [{:#x}]\nMyHUDVTableFunctionBase[PostRenderIndex]: [{:#x}] (entry at [{:#x}])",
				  (uintptr_t)VTablePointer,
				  (uintptr_t)VTableFunctionBase,
				  (uintptr_t)VTableFunctionBase[UnrealEngine4::POSTRENDER_VTABLEINDEX],
				  (uintptr_t)&VTableFunctionBase[UnrealEngine4::POSTRENDER_VTABLEINDEX]);
		DWORD dwProtect;
		VirtualProtect(VTableFunctionBase, (UnrealEngine4::POSTRENDER_VTABLEINDEX + 1) * 8, PAGE_EXECUTE_READWRITE, &dwProtect);
		*originalFunction = (UnrealEngine4::tPostRender)VTableFunctionBase[UnrealEngine4::POSTRENDER_VTABLEINDEX];
		LOG_TRACE("oPostRender now points to [{:#x}]\n", (uintptr_t)originalFunction);
		VTableFunctionBase[UnrealEngine4::POSTRENDER_VTABLEINDEX] = (unsigned long long)hkPostRender;
		VirtualProtect(VTableFunctionBase, (UnrealEngine4::POSTRENDER_VTABLEINDEX + 1) * 8, dwProtect, NULL);
		LOG_INFO("Hooked PostRender() [{:#x}] -> [{:#x}]\n", (uintptr_t)originalFunction, (uintptr_t)VTableFunctionBase[UnrealEngine4::POSTRENDER_VTABLEINDEX]);
		return true;
	}
}