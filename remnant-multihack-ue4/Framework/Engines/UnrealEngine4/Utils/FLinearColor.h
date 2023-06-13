#pragma once

namespace UnrealEngine4
{
	struct FLinearColor
	{
		float R,G,B,A;

		FORCEINLINE FLinearColor(float InR, float InG, float InB, float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA) {}
	};
}