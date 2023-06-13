#include "Offsets_UE4.h"

#include "Structs/BucketUObjects.h"
#include "Structs/BucketFNames.h"
#include "Structs/ArrayTypes.h"
#include "Structs/UObject.h"
#include "Structs/UClass.h"

#include "Structs/UEngine.h"
#include "Structs/UWorld.h"

#include "Utils/FLinearColor.h"
#include "Utils/FVector.h"
#include "Utils/FVector2D.h"
#include "Utils/FMath.h"
#include "Utils/FMatrix.h"
#include "Utils/FQuat.h"
#include "Utils/FRotationMatrix.h"
#include "Utils/FRotationTranslationMatrix.h"
#include "Utils/FRotator.h"
#include "Utils/FTransform.h"

namespace UnrealEngine4
{
	bool Initialize();

	bool HookPostRender(UnrealEngine4::tPostRender hkPostRender, UnrealEngine4::tPostRender* originalFunction);
}