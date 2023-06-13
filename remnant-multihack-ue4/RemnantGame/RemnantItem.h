#pragma once

namespace RemnantGame
{
	class RemnantItem
	{
	public:
		char pad_0x0000[0x30]; //0x0000
		class UObject** SomethingArr; //0x0030
		__int32 NumSomething; //0x0038 
		char pad_0x003C[0xBC]; //0x003C
		class UObject* N00001677; //0x00F8 
		char pad_0x0100[0x250]; //0x0100
		class RemnantItemInstanceData* PickupItemInstanceData; //0x0350 
		char pad_0x0358[0xE8]; //0x0358

	}; //Size=0x0440
}