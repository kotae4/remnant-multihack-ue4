#pragma once

#include <cmath>

namespace UnrealEngine4
{
	struct FRotator
	{
	public:
		float Pitch;
		float Yaw;
		float Roll;

		FORCEINLINE FRotator() { }

		FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll)
			: Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}

		FORCEINLINE float ClampAxis(float Angle)
		{
			// returns Angle in the range (-360,360)
			Angle = fmod(Angle, 360.f);

			if (Angle < 0.f)
			{
				// shift to [0,360) range
				Angle += 360.f;
			}

			return Angle;
		}

		FORCEINLINE float NormalizeAxis(float Angle)
		{
			// returns Angle in the range [0,360)
			Angle = ClampAxis(Angle);

			if (Angle > 180.f)
			{
				// shift to (-180,180]
				Angle -= 360.f;
			}

			return Angle;
		}

		FORCEINLINE void Normalize()
		{
			Pitch = NormalizeAxis(Pitch);
			Yaw = NormalizeAxis(Yaw);
			Roll = NormalizeAxis(Roll);
		}
	};
}