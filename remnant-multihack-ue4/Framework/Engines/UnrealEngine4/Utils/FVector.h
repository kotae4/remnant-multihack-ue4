#pragma once

#include "FRotator.h"
#include <cmath>

namespace UnrealEngine4
{
	struct FVector
	{
	public:
		float X;
		float Y;
		float Z;

		FORCEINLINE FVector() {}

		FORCEINLINE FVector(float InX, float InY, float InZ)
			: X(InX), Y(InY), Z(InZ) {}

		FORCEINLINE float DistanceTo(FVector other)
		{
			FVector vector = FVector(X - other.X, Y - other.Y, Z - other.Z);
			return sqrtf(((vector.X * vector.X) + (vector.Y * vector.Y) + (vector.Z * vector.Z)));
		}

		FORCEINLINE FVector operator+(const FVector& V) const
		{
			return FVector(X + V.X, Y + V.Y, Z + V.Z);
		}

		FORCEINLINE FVector operator-(const FVector& V) const
		{
			return FVector(X - V.X, Y - V.Y, Z - V.Z);
		}

		FORCEINLINE FVector operator*(float Scale) const
		{
			return FVector(X * Scale, Y * Scale, Z * Scale);
		}

		FORCEINLINE FVector operator*(const FVector& V) const
		{
			return FVector(X * V.X, Y * V.Y, Z * V.Z);
		}

		FORCEINLINE FVector operator+=(const FVector& V)
		{
			X += V.X; Y += V.Y; Z += V.Z;
			return *this;
		}

		FORCEINLINE FVector operator-=(const FVector& V)
		{
			X -= V.X; Y -= V.Y; Z -= V.Z;
			return *this;
		}

		FORCEINLINE FVector operator*=(float Scale)
		{
			X *= Scale; Y *= Scale; Z *= Scale;
			return *this;
		}

		FORCEINLINE FVector operator*=(const FVector& V)
		{
			X *= V.X; Y *= V.Y; Z *= V.Z;
			return *this;
		}

		FORCEINLINE bool operator==(const FVector& V) const
		{
			return (this->X == V.X) && (this->Y == V.Y) && (this->Z == V.Z);
		}

		FRotator ToOrientationRotator()
		{
			FRotator R;

			// Find yaw.
			R.Yaw = atan2(Y, X) * (57.295779513082320876798154814105f);

			// Find pitch.
			R.Pitch = atan2(Z, sqrtf(X * X + Y * Y)) * (57.295779513082320876798154814105f);

			// Find roll.
			R.Roll = 0;
			return R;
		}
	};
}