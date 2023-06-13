#pragma once

#include "FRotationTranslationMatrix.h"

namespace UnrealEngine4
{
	struct FRotationMatrix : FRotationTranslationMatrix
	{
		FRotationMatrix(const FRotator& Rot)
			: FRotationTranslationMatrix(Rot, FVector(0.f, 0.f, 0.f))
		{ }
	};
}