#pragma once

#include "RemnantStateMachineCpnt.h"

namespace RemnantGame
{
	class RemnantLootContainer
	{
	public:
		char pad_0x0000[0x3F8]; //0x0000
		class StateMachineCpnt* StateMachineCpnt; //0x03F8 

	}; //Size=0x0400
}