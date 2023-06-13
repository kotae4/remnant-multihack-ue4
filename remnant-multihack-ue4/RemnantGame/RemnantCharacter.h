#pragma once

#include "Framework/Engines/UnrealEngine4/Structs/ACharacter.h"
#include "Framework/Utils/Memory.h"

namespace RemnantGame
{
	class RemnantCharacter
	{
	public:

		__int64 pad;

		float GetCurrentHealth() // offset 30h, index 6
		{
			float retValue = 0.f;
			typedef float(__thiscall* OriginalFunc)(void* self, float& outValue);
			retValue = (Memory::CallVirtual<OriginalFunc>(this, 6))(this, retValue);
			return retValue;
		}

		float GetMaxHealth() // offset 38h, index 7
		{
			float retValue = 0.f;
			typedef float(__thiscall* OriginalFunc)(void* self, float& outValue);
			retValue = (Memory::CallVirtual<OriginalFunc>(this, 7))(this, retValue);
			return retValue;
		}
	};

	class CharacterGunfire : public UnrealEngine4::ACharacter
	{
	public:
		char pad_0x03B0[0x398]; //0x03B0
		class RemnantCharacter RemnantCharacterData; //0x0748 
		char pad_0x0750[0x10]; //0x0750
		class UObject* StatsComponent; //0x0760 
		char pad_0x0768[0xD0]; //0x0768
		float SomethingWithDeath_NegTimer; //0x0838 
		char pad_0x083C[0x34]; //0x083C
		float HealthPercentageTimes10; //0x0870 
		char pad_0x0874[0xD4]; //0x0874
		class UObject* StateMachineCpnt; //0x0948 
		class RemnantPlayerInventoryComponent* InventoryCpnt; //0x0950 

	}; //Size=0x0400
}