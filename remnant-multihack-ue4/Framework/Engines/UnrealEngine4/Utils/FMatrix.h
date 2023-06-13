#pragma once

#include "FVector.h"

namespace UnrealEngine4
{
	namespace EAxis
	{
		enum Type
		{
			None,
			X,
			Y,
			Z,
		};
	}

	struct FMatrix
	{
	public:
		union
		{
			__declspec(align(16)) float M[4][4];
		};

		FORCEINLINE FMatrix() {};

		FORCEINLINE void GetScaledAxes(FVector& X, FVector& Y, FVector& Z) const
		{
			X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
			Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
			Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
		}

		FORCEINLINE FVector GetScaledAxis(EAxis::Type InAxis) const
		{
			switch (InAxis)
			{
			case EAxis::X:
				return FVector(M[0][0], M[0][1], M[0][2]);

			case EAxis::Y:
				return FVector(M[1][0], M[1][1], M[1][2]);

			case EAxis::Z:
				return FVector(M[2][0], M[2][1], M[2][2]);

			default:
				return FVector(0.f, 0.f, 0.f);
			}
		}

		FORCEINLINE FVector GetOrigin() const
		{
			return FVector(M[3][0], M[3][1], M[3][2]);
		}
	};
}