#pragma once

namespace RemnantGame
{
	class StateMachine_State
	{
	public:
		char pad_0x0000[0x3C]; //0x0000
		__int32 StateNameIndex; //0x003C 
		char pad_0x0040[0x8]; //0x0040

	}; //Size=0x0048

	class StateMachineCpnt
	{
	public:
		char pad_0x0000[0x2D8]; //0x0000
		class StateMachine_State* CurrentState; //0x02D8 

	}; //Size=0x02E0
}