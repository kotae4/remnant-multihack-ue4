#include "pch.h"
#include "CheatMain.h"
#include "Framework/framework.h"
#include "Framework/Engines/UnrealEngine4/Structs/ULevel.h"
#include "Framework/Engines/UnrealEngine4/Structs/USkeletalMesh.h"
#include "Framework/Engines/UnrealEngine4/Structs/USkinnedMeshComponent.h"
#include "Framework/Engines/UnrealEngine4/Structs/UWorld.h"
#include "Framework/Engines/UnrealEngine4/Structs/APlayerController.h"

#include "Framework/Utils/Memory.h"
#include "Framework/Utils/Logger.h"
#include "Framework/Utils/FileUtils.h"
#include "Framework/Utils/Profiler.h"

#include "RemnantGame/RemnantLootContainer.h"
#include "RemnantGame/RemnantCharacter.h"
#include "RemnantGame/RemnantPlayerInventoryComponent.h"
#include "RemnantGame/RemnantItem.h"
#include "RemnantGame/RemnantItemInstanceData.h"

#include "Config.h"

#include "gui/main_tab.h"
#include "gui/player_tab.h"
#include "gui/aim_tab.h"
#include "gui/visuals_tab.h"
#include "gui/radar_tab.h"
#include "gui/debug_tab.h"

#include "imgui/imgui.h"


HMODULE Memory::Module;
DWORD64 Memory::ProcessBaseAddress;

std::wstringstream logWideStream;
UnrealEngine4::tPostRender oPostRender = NULL;
// static classes that we use frequently in UObject::IsA or UObject::GetTypedOuter calls
UClass *m_ActorClass, * m_CharacterClass, *m_LevelClass, *m_RemnantLootContainerClass;

// variables we use frequently or for state
UnrealEngine4::ULocalPlayer* LocalPlayer = NULL;
UnrealEngine4::APawn* LocalPlayerPawn = NULL;
UnrealEngine4::FVector LocalPlayerPosition = FVector(0.f, 0.f, 0.f);

CheatMain::ESPTarget m_CurESPTarget = CheatMain::ESPTarget();

std::vector<CheatMain::ESPTarget> m_AimTargets = std::vector<CheatMain::ESPTarget>();
int m_NumAimTargets = 0;
CheatMain::ESPTarget* m_AimbotTarget = nullptr;

namespace CheatMain
{
	bool AttachDebugConsole(void)
	{
		if (FreeConsole())
		{
			if (!AllocConsole())
				return false;
		}
		else if (!AllocConsole())
			return false;

		FILE* pConOutput = nullptr;
		return (freopen_s(&pConOutput, "CONOUT$", "w", stdout) == 0);
	}

	void Entrypoint(HMODULE selfMod)
	{
		AttachDebugConsole();
		Utils::Profiler::InitProfiling();

		if (CheatFramework::Init(selfMod, ETargetEngine::UnrealEngine4) == false)
		{
			LOG_ERROR("Could not initialize framework");
		}

		if (Config.Load() == false)
		{
			LOG_ERROR("Failed to load config");
			// save config right away to generate the default settings.json file
			if (Config.Save() == false)
			{
				LOG_ERROR("Failed to save config");
				return;
			}
		}

		if (LoadCommonClasses() == false)
		{
			LOG_ERROR("Failed to load common classes");
			return;
		}

		if (UnrealEngine4::HookPostRender((tPostRender)hkPostRender, &oPostRender) == false)
		{
			LOG_ERROR("Failed to hook post render (crash)");
			return;
		}

		CheatFramework::RegisterWndProcHandler(MyWndProc);
		CheatFramework::RegisterImGuiFunction(ImGuiDraw);
	}

	bool LoadCommonClasses()
	{
		// TODO make this better. usages of the class should access it through a helper function which wraps this logic.
		// possibly make it a framework function.
		bool success = true;
		m_ActorClass = UObject::GetStaticClass("Class /Script/Engine.Actor");
		if (m_ActorClass == nullptr)
		{
			LOG_ERROR("CRITICAL: Could not find Actor class!");
			success = false;
		}
		//m_CharacterClass = UObject::GetStaticClass("Class /Script/Engine.Character");
		m_CharacterClass = UObject::GetStaticClass("Class /Script/GunfireRuntime.AICharacter");
		if (m_CharacterClass == nullptr)
		{
			LOG_ERROR("CRITICAL: Could not find Character class!");
			success = false;
		}
		m_LevelClass = UObject::GetStaticClass("Class /Script/Engine.Level");
		if (m_LevelClass == nullptr)
		{
			LOG_ERROR("CRITICAL: Could not find Level class!");
			success = false;
		}
		m_RemnantLootContainerClass = UObject::GetStaticClass("Class /Script/Remnant.LootContainer");
		if (m_RemnantLootContainerClass == nullptr)
		{
			LOG_ERROR("CRITICAL: Could not find Remnant.LootContainer class!");
			success = false;
		}
		return success;
	}

	bool IsValidGameState()
	{
		if ((GWorld == nullptr) || (GWorld->OwningGameInstance == nullptr) || (GWorld->OwningGameInstance->LocalPlayers[0] == nullptr) 
			|| (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController == nullptr)
			|| (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn == nullptr) 
			|| (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->Pawn->RootComponent == nullptr)
			|| (GWorld->LevelList == nullptr) || (GWorld->PersistentLevel == nullptr) || (GWorld->NumLevels <= 0))
			return false;
		return true;
	}

	std::wstring GetFriendlyObjectName(UObject* obj)
	{
		FString objName = obj->GetFullName(NULL);
		std::wstring friendlyName = std::wstring(objName.c_str());
		friendlyName = friendlyName.substr(0, friendlyName.find(' '));
		for (std::pair<const std::wstring, std::wstring>& kv : Config.NameSubstitutions)
		{
			if (friendlyName.find(kv.first) != std::string::npos)
			{
				friendlyName = std::wstring(kv.second);
				break;
			}
		}
		
		return friendlyName;
	}

	bool IsRelevantActor(AActor* actor)
	{
		// do all the IsA checks and ignore list checks
		// check remnant loot container state
		// do NOT check screen position or distance (do this in draw function instead)
		for (const ESPTypeData& targetTypeData : Config.ESPClassTypeData)
		{
			// skip if it's not on the whitelist
			if ((targetTypeData.Class == nullptr) || (actor->IsA(targetTypeData.Class) == false))
				continue;
			// skip if it's an opened loot container
			if (targetTypeData.Class == m_RemnantLootContainerClass)
			{
				RemnantLootContainer* chestObj = (RemnantLootContainer*)actor;
				if (chestObj->StateMachineCpnt)
				{
					if (chestObj->StateMachineCpnt->CurrentState)
					{
						FNameEntry* stateName = GNames->GetNameFromIndex(chestObj->StateMachineCpnt->CurrentState->StateNameIndex);
						std::string outputStateName;
						if (stateName->IsWide())
						{
							if (Utils::Strings::TryConvertUtf16ToUtf8(std::wstring(stateName->WideName), outputStateName) == false)
							{
								LOG_ERROR("Could not convert state name to narrow string");
								return false;
							}
						}
						else
						{
							outputStateName = std::string(stateName->AnsiName);
						}
						if (outputStateName.find("Opened") != std::string::npos)
						{
							return false;
						}
					}
				}
			}
			// skip if it's dead
			if (targetTypeData.Class == m_CharacterClass)
			{
				RemnantGame::CharacterGunfire* charObj = (RemnantGame::CharacterGunfire*)actor;
				if (charObj->HealthPercentageTimes10 <= 0.f)
					return false;
			}
			std::wstring actorName = GetFriendlyObjectName(actor);
			// skip if it's on the ignore list
			for (const std::wstring& ignoredName : targetTypeData.IgnoredNames)
			{
				if (actorName.find(ignoredName) != std::string::npos)
				{
					return false;
				}
			}
			// if we get here then it's passed all checks, so just return true
			return true;
		}
		return false;
	}

	void DrawTarget(AHUD* hud, ESPTarget* targetInfo)
	{
		if ((targetInfo->haveBonesMask == 127) && (GWorld->LineBatcher != nullptr))
		{
			// we have a head AND a body, so we can draw a line
			// Skeleton line path:
			// Head -> Spine1 -> Both Hands
			// Spine1 -> Pelvis -> Both Feet
			//Profiler::BeginSample("DrawSkeleton");
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->headPos, targetInfo->spinePos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->spinePos, targetInfo->leftHandPos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->spinePos, targetInfo->rightHandPos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->spinePos, targetInfo->pelvisPos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->pelvisPos, targetInfo->leftFootPos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			GWorld->LineBatcher->DrawLineDirect(GWorld->LineBatcher, targetInfo->pelvisPos, targetInfo->rightFootPos, FLinearColor(1.f, 0.f, 0.f, 1.f), 1, 1.f, 0.f);
			//Profiler::EndSample("DrawSkeleton");
		}

		if (Config.ESPNamesEnabled == false)
			return;

		std::wostringstream woss;
		float textScale = 1.f;
		//woss << L"[" << std::hex << targetInfo->Actor << L"] ";
		woss.precision(2);
		woss << targetInfo->FriendlyName.c_str();
		if (targetInfo->Distance > 3000.f)
		{
			woss.precision(0);
			woss << L" - " << std::fixed << targetInfo->Distance << L"m";
			textScale = (targetInfo->Distance > 10000.f ? 0.45f : (1.f - ((targetInfo->Distance - 3000.f) * 0.000078573112281f)));
		}
		if (targetInfo->Character != nullptr)
		{
			float charHealth = targetInfo->Character->RemnantCharacterData.GetCurrentHealth();
			float charMaxHealth = targetInfo->Character->RemnantCharacterData.GetMaxHealth();
			woss << "\n" << std::fixed << charHealth << L" / " << std::fixed << charMaxHealth;
		}
		if ((m_NumAimTargets > 0) && (m_AimbotTarget) && (targetInfo->Actor == m_AimbotTarget->Actor))
			hud->DrawString(hud, FString(woss.str().c_str()), FLinearColor(0.43f, 0.86f, 1.f, 1.f), targetInfo->ScreenPos.X, targetInfo->ScreenPos.Y, GEngine->SmallFont, textScale, false);
		else
			hud->DrawString(hud, FString(woss.str().c_str()), targetInfo->TypeData->Color, targetInfo->ScreenPos.X, targetInfo->ScreenPos.Y, GEngine->SmallFont, textScale, false);
	}

	void LoadBoneInfo(ESPTarget* targetInfo)
	{
		if (targetInfo->Character->Mesh)
		{
			USkeletalMesh* skeletalMesh = targetInfo->Character->Mesh->SkeletalMesh;
			if (skeletalMesh)
			{
				FVector bonePos;
				UnrealEngine4::FCollisionQueryParams_DerpModeActive colQuery = UnrealEngine4::FCollisionQueryParams_DerpModeActive(LocalPlayerPawn, targetInfo->Actor);
				bool isBoneVisible = false;
				for (int boneIndex = 0; boneIndex < skeletalMesh->NumBones; boneIndex++)
				{
					// TODO should we keep going to collect all visible bones or nah?
					if (targetInfo->haveBonesMask == 127) break;
					// call GetBoneName(boneIndex) and store retVal
					//LOG_INFO("Trying to get name index for bone#{}", boneIndex);
					__int64 boneName = targetInfo->Character->Mesh->GetBoneName(boneIndex);
					//LOG_INFO("Trying to get name from index {}", boneName);
					FNameEntry* readableBoneName = GNames->GetNameFromIndex((int)boneName);
					if (readableBoneName == nullptr)
					{
						//LOG_WARN("-----Could not get bone name from [{}]", boneName);
						continue;
					}
					std::string stringBoneName;
					if (readableBoneName->IsWide())
					{
						if (Utils::Strings::TryConvertUtf16ToUtf8(std::wstring(readableBoneName->WideName), stringBoneName) == false)
						{
							LOG_ERRORW(L"Could not convert bone name to narrow string '{}'", readableBoneName->WideName);
						}
					}
					else
					{
						stringBoneName = std::string(readableBoneName->AnsiName);
					}
					//LOG_INFO("Attempting to get location for bone '{}'", stringBoneName.c_str());
					bonePos = targetInfo->Character->Mesh->GetBoneLocation_MyImpl(boneIndex);
					isBoneVisible = !GWorld->LineTraceTestByChannel(LocalPlayerPosition, bonePos, ECC_Visibility, colQuery, *GDefaultCollisionResponseParams);
					if (isBoneVisible)
					{
						targetInfo->VisibleBones.push_back(BoneTarget(stringBoneName, bonePos));
					}
					// map out our skeleton
					//Profiler::BeginSample("FindBones");
					// TODO storing the name **index** and comparing the integers here will give significant speed increase
					if ((stringBoneName.find("Head") != std::string::npos) || (stringBoneName.find("Neck") != std::string::npos) || (stringBoneName.find("Bone_OB_Obelisk") != std::string::npos)
						|| (stringBoneName.find("Bone_Turret_Barrel") != std::string::npos))
					{
						targetInfo->headPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 64;
					}
					else if (stringBoneName.find("_Spine") != std::string::npos)
					{
						targetInfo->spinePos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 32;
					}
					else if (stringBoneName.find("Pelvis") != std::string::npos)
					{
						targetInfo->pelvisPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 16;
					}
					else if ((stringBoneName.find("Hand_L") != std::string::npos) || (stringBoneName.find("Fingers_L") != std::string::npos))
					{
						targetInfo->leftHandPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 8;
					}
					else if ((stringBoneName.find("Hand_R") != std::string::npos) || (stringBoneName.find("Fingers_R") != std::string::npos))
					{
						targetInfo->rightHandPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 4;
					}
					else if ((stringBoneName.find("Foot_L") != std::string::npos) || (stringBoneName.find("Ankle_L") != std::string::npos))
					{
						targetInfo->leftFootPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 2;
					}
					else if ((stringBoneName.find("Foot_R") != std::string::npos) || (stringBoneName.find("Ankle_R") != std::string::npos))
					{
						targetInfo->rightFootPos = bonePos;
						targetInfo->haveBonesMask = targetInfo->haveBonesMask | 1;
					}
					//Profiler::EndSample("FindBones");
				}
				// NOTE:
				// i was going to do something else with the bonemask but forgot
				if ((targetInfo->haveBonesMask & 64) == 0)
				{
					LOG_WARNW(L"ERROR: Character '{}' has no head! [{:x}]\n", targetInfo->Character->GetFullName(NULL).c_str(), (uintptr_t)targetInfo->Character);
					return;
				}
			}
		}
	}

	void LoadTargetInfo(ESPTarget* targetInfo, AActor* actor, AHUD* hud)
	{
		targetInfo->Reset();
		targetInfo->Actor = actor;
		targetInfo->FriendlyName = GetFriendlyObjectName(actor);
		if (actor->RootComponent)
		{
			targetInfo->ScreenPos = hud->Project(hud, actor->RootComponent->Position);
			targetInfo->Distance = LocalPlayerPosition.DistanceTo(actor->RootComponent->Position);
		}

		// TODO figure out if better matching logic is possible, instead of using explicit for-loop.
		// otherwise just move this to a framework function to clean this part up.
		for (const ESPTypeData& targetTypeData : Config.ESPClassTypeData)
		{
			if ((targetTypeData.Class != nullptr) && (actor->IsA(targetTypeData.Class)))
			{
				targetInfo->TypeData = &targetTypeData;
				if (targetTypeData.Class == m_CharacterClass)
				{
					targetInfo->Character = (RemnantGame::CharacterGunfire*)actor;
					if ((Config.ESPBonesEnabled == true) && (targetInfo->Distance <= Config.ESPBoneDrawDistance))
					{
						LoadBoneInfo(targetInfo);
					}
				}
				// can only be 1 class, so if we match once we can stop looking at others
				break;
			}
		}
	}

	ESPTarget* GetBestAimTarget()
	{
		float shortestDistance = INFINITY;
		ESPTarget* closestTarget = nullptr;

		for (int targetIndex = 0; targetIndex < m_NumAimTargets; ++targetIndex)
		{
			if ((Config.AimbotUseCharacterDistance == true) || (true))
			{
				if (m_AimTargets[targetIndex].Distance < shortestDistance)
				{
					shortestDistance = m_AimTargets[targetIndex].Distance;
					closestTarget = &m_AimTargets[targetIndex];
				}
			}
			// TODO else check screen distance (effectively the aim angle)
		}
		if (closestTarget)
		{
			BoneTarget* boneTarget = &closestTarget->VisibleBones[0];
			if (closestTarget->VisibleBones.size() > 1)
			{
				bool foundNameMatch = false;
				for (std::vector<BoneTarget>::iterator boneIter = closestTarget->VisibleBones.begin(); boneIter != closestTarget->VisibleBones.end(); ++boneIter)
				{
					if ((boneIter->Name.find("Face") != std::string::npos) || (boneIter->Name.find("Jaw") != std::string::npos) || (boneIter->Name.find("Mouth") != std::string::npos) ||
						(boneIter->Name.find("Head") != std::string::npos) || (boneIter->Name.find("Neck") != std::string::npos))
					{
						// these mobs have weird heads that we don't actually want to aim at
						if ((closestTarget->FriendlyName.find(L"Impaler") == std::string::npos) && (closestTarget->FriendlyName.find(L"Detonator") == std::string::npos))
						{
							boneTarget = boneIter._Ptr;
							foundNameMatch = true;
						}
					}
					// alternative "head" aim points for those weird mobs
					if ((boneIter->Name.find("Spine") != std::string::npos) || (boneIter->Name.find("Pelvis") != std::string::npos))
					{
						boneTarget = boneIter._Ptr;
						foundNameMatch = true;
					}
				}
				if (!foundNameMatch)
				{
					// if an ideal aim point isn't found, then just choose the closest bone
					// TODO wrap this into the same for-loop. and set boneTarget to the tracked closest bone if no name match is found.
					float shortestBoneDistance = INFINITY;
					for (std::vector<BoneTarget>::iterator sboneIter = closestTarget->VisibleBones.begin(); sboneIter != closestTarget->VisibleBones.end(); ++sboneIter)
					{
						float boneDistance = LocalPlayerPosition.DistanceTo(sboneIter->Position);
						if (boneDistance < shortestBoneDistance)
						{
							shortestBoneDistance = boneDistance;
							boneTarget = sboneIter._Ptr;
						}
					}
				}
			}
			closestTarget->AimBone = *boneTarget;
			closestTarget->AimPos = boneTarget->Position;
		}
		return closestTarget;
	}

	void ProcessTargets(AHUD* hud)
	{
		LocalPlayer = GWorld->OwningGameInstance->LocalPlayers[0];
		LocalPlayerPawn = LocalPlayer->PlayerController->Pawn;
		LocalPlayerPosition = LocalPlayerPawn->RootComponent->Position;
		// each frame we clear the aim targets
		m_NumAimTargets = 0;

		for (int levelIndex = 0; levelIndex < GWorld->NumLevels; levelIndex++)
		{
			ULevel* level = (ULevel*)GWorld->LevelList[levelIndex];
			if (level == NULL) continue;
			for (int actorIndex = 0; actorIndex < level->NumActors; actorIndex++)
			{
				AActor* actor = level->ActorList[actorIndex];
				if ((actor == LocalPlayerPawn) || (actor == nullptr))
				{
					continue;
				}
				if (IsRelevantActor(actor) == false)
				{
					continue;
				}
				LoadTargetInfo(&m_CurESPTarget, actor, hud);
				// filter by visibility and minimum distance
				// the min distance filters out our own equipped items which exist in the world for some reason
				if ((m_CurESPTarget.ScreenPos.Z <= 0.f) || (m_CurESPTarget.Distance < 5.f))
					continue;
				if ((Config.AimbotEnabled == true) && (m_CurESPTarget.VisibleBones.size() > 0))
				{
					// if they have visible bones then they are an eligible character to aim at
					// to avoid expensive re-allocations, we reuse slots in the vector as much as possible
					// TODO apply this optimization to VisibleBones vectors too...
					if (m_AimTargets.size() <= m_NumAimTargets)
					{
						m_AimTargets.push_back(m_CurESPTarget);
					}
					else
					{
						m_AimTargets[m_NumAimTargets].Distance = m_CurESPTarget.Distance;
						m_AimTargets[m_NumAimTargets].FriendlyName = m_CurESPTarget.FriendlyName;
						m_AimTargets[m_NumAimTargets].VisibleBones = m_CurESPTarget.VisibleBones;
					}
					m_NumAimTargets++;
				}
				DrawTarget(hud, &m_CurESPTarget);
			}
		}
		// aimbot logic
		if ((Config.AimbotEnabled == true) && (m_NumAimTargets > 0))
		{
			// TO-DO research a bunch of different implementations of TPS cameras and figure out how this one works
			// NOTE:
			// Third Person Cameras require something else! This will set the character mesh to look at the target, but the camera remains where it is (kind of like silent aim, but unintentional)
			// Class /Script/Engine.CameraActor might be the solution. it has no components, but it might be the anchor for the camera.

			APlayerController* myPlayerController = GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
			FVector playerEyeLoc;
			FRotator playerEyeRot;

			// NOTE:
			// so, the aimbot is *slightly* inaccurate, and here's why:
			// the playerEyeLoc here will always 100% be where your bullets end up (confirmed via drawline)
			// HOWEVER, it's not where bullets START, so if there's anything in front of the weapon but not in front of the player's eyes then it'll miss
			// this usually only happens at very close range, or when there's a lot geometry where you're aiming (like at fences, where the boards are blocking but the spaces between them aren't)
			// one solution is to, in the function that traces from player to each target bone, is to trace from the WeaponCast component instead of player eye position
			// WeaponCast cpnt is in the OtherComponents collection on the PlayerCharacter.

			myPlayerController->GetPlayerViewPoint(playerEyeLoc, playerEyeRot);
			//Profiler::BeginSample("BoneTargeting");
			m_AimbotTarget = GetBestAimTarget();
			//Profiler::EndSample("BoneTargeting");
			if (m_AimbotTarget == nullptr) return;
			FVector aimDir = m_AimbotTarget->AimPos - playerEyeLoc;

			//FVector aimDir = m_AimbotTarget->RootComponent->Position - playerEyeLoc;

			FRotator aimAtRot = aimDir.ToOrientationRotator();
			myPlayerController->SetControlRotation(aimAtRot);
			LocalPlayerPawn->FaceRotation(aimAtRot, 0.f);
			myPlayerController->TPSPlayerCameraManager->Rotation = aimAtRot;
			std::wostringstream woss;
			woss << L"AIMING " << m_AimbotTarget->FriendlyName << "\n" << m_AimbotTarget->AimBone.Name.c_str();
			// TODO get display length of string and subtract from X pos to properly center the text
			FVector2D screenDims = LocalPlayer->ViewportClient->Viewport->Size;
			hud->DrawString(hud, FString(woss.str().c_str()), FLinearColor(1.f, 0.f, 1.f, 1.f), (screenDims.X * 0.5f) - 100.f, (screenDims.Y * 0.5f) + 100.f, GEngine->SmallFont, 0.65f, false);
		}
	}

	void DbgIterateGObjects()
	{
		// NOTE this will make the game freeze for a few minutes. Console prints are expensive.
		LOG_INFO("Iterating GObjects...");
		LOG_INFO("--Addr: {:x}", (uintptr_t)GObjects);
		LOG_INFO("--NumBucketsAddr: {:x}", (uintptr_t)&GObjects->NumBuckets);
		LOG_INFO("--NumBuckets: {}\n--MaxBuckets: {}\n--NumObjects: {}\n--MaxObjects: {}", GObjects->NumBuckets, GObjects->MaxBuckets, GObjects->NumObjects, GObjects->MaxObjects);
		for (int bucketIndex = 0; bucketIndex < GObjects->NumBuckets; bucketIndex++)
		{
			for (int entityIndex = 0; ((entityIndex < OBJECT_BUCKET_SIZE) && (((bucketIndex * OBJECT_BUCKET_SIZE) + entityIndex) < GObjects->NumObjects)); entityIndex++)
			{
				UObject* obj = GObjects->Buckets[bucketIndex][entityIndex].Object;
				LOG_INFO("UObject: [{:x}]", (uintptr_t)obj);
				if (obj)
				{
					FString objName = obj->GetFullName(NULL);
					LOG_INFOW(L"--Name: {}", objName.c_str());
				}
			}
		}
	}

	void DbgPrintLevelActorCounts()
	{
		for (int levelIndex = 0; levelIndex < GWorld->NumLevels; levelIndex++)
		{
			ULevel* level = (ULevel*)GWorld->LevelList[levelIndex];
			if (level == NULL) continue;
			LOG_INFO("Level[{}] has {} actors", levelIndex, level->NumActors);
		}
	}

	void hkPostRender(UnrealEngine4::AHUD* gameHUD)
	{
		if (oPostRender != NULL)
		{
			oPostRender(gameHUD);
		}

		Utils::Profiler::BeginSample("PostRender");
		if (IsValidGameState())
		{
			Utils::Profiler::BeginSample("ProcessTargets");
			// NOTE heart of the cheat functionality
			// TODO break this up more - KISS
			ProcessTargets(gameHUD);
			Utils::Profiler::EndSample("ProcessTargets");
		}
		Utils::Profiler::EndSample("PostRender");

		std::wstringstream profilingWSS;
		Utils::Profiler::WriteStatsToStream(profilingWSS);
		gameHUD->DrawString(gameHUD, FString(profilingWSS.str().c_str()), FLinearColor(1.f, 0.f, 0.f, 1.f), 10.f, 70.f, UnrealEngine4::GEngine->SmallFont, 1.f, false);
	}

	void ImGuiDraw(ImDrawList* drawList)
	{
		Utils::Profiler::BeginSample("ImGuiDraw");
		if (Config.MenuEnabled)
		{
			ImGui::Begin("Remnant Multihack");
			ImGui::BeginTabBar("Hack#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);

			MainTab::Render();
			PlayerTab::Render();
			AimTab::Render();
			VisualsTab::Render();
			RadarTab::Render();
			DebugTab::Render();


			ImGui::EndTabBar();
			ImGui::End();
		}
		Utils::Profiler::EndSample("ImGuiDraw");
	}

	bool MyWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (KeyBinds::IsKeyPressed(Config.Keybinds.Toggle_Menu))
		{
			Config.MenuEnabled = !Config.MenuEnabled;
		}
		if (KeyBinds::IsKeyPressed(Config.Keybinds.Toggle_Aimbot))
			Config.AimbotEnabled = !Config.AimbotEnabled;

		// try to prevent the game from receiving the input by returning true
		if (Config.MenuEnabled)
		{
			return true;
		}
	}
}