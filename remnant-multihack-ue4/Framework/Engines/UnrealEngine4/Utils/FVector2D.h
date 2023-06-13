#pragma once

namespace UnrealEngine4
{
	struct FVector2D
	{
		float X;
		float Y;

		FORCEINLINE FVector2D() {}

		FORCEINLINE FVector2D(float InX, float InY)
			: X(InX), Y(InY) {}
	};
}