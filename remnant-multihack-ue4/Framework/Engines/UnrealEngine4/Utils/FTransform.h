#pragma once

#include "FQuat.h"
#include "FVector.h"

namespace UnrealEngine4
{
	struct FTransform
	{
	public:
		/** Rotation of this transformation, as a quaternion. */
		FQuat	Rotation;
		/** Translation of this transformation, as a vector. */
		FVector	Translation;
		/** 3D scale (always applied in local space) as a vector. */
		FVector	Scale3D;
	};
}