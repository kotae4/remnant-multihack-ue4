#pragma once

#include "Framework/Engines/UnrealEngine4/Core.h"
#include "Framework/Engines/UnrealEngine4/Structs/AActor.h"
#include "RemnantGame/RemnantCharacter.h"
#include "Config.h"
#include "imgui/imgui.h"
#include <string>
#include <vector>

using namespace UnrealEngine4;
using namespace RemnantGame;

namespace CheatMain
{

	struct BoneTarget
	{
	public:
		std::string Name;
		FVector Position;

		BoneTarget() { Name = "ERROR"; Position = FVector(0.f, 0.f, 0.f); }
		BoneTarget(std::string _Name, FVector _Position) : Name(_Name), Position(_Position) {}
	};

	struct ESPTarget
	{
	public:
		const ESPTypeData* TypeData; // filled by FilterObject()
		float Distance; // filled by FilterObject()
		AActor* Actor; // filled by DoFrame()
		CharacterGunfire* Character; // filled by DoFrame()
		FVector ScreenPos; // filled by FilterObject()
		FVector headPos = FVector(0.f, 0.f, 0.f), pelvisPos = FVector(0.f, 0.f, 0.f), spinePos = FVector(0.f, 0.f, 0.f), leftClaviclePos = FVector(0.f, 0.f, 0.f),
			rightClaviclePos = FVector(0.f, 0.f, 0.f), leftHandPos = FVector(0.f, 0.f, 0.f), rightHandPos = FVector(0.f, 0.f, 0.f), leftFootPos = FVector(0.f, 0.f, 0.f),
			rightFootPos = FVector(0.f, 0.f, 0.f); // filled by PrepareSkeleton()
		__int8 haveBonesMask = 0; // filled by PrepareSkeleton()
		FVector AimPos; // filled by PrepareSkeleton()
		BoneTarget AimBone;
		std::wstring FriendlyName; // filled by GetObjectName()
		std::vector<BoneTarget> VisibleBones;

		ESPTarget(ESPTypeData* _typeData, float _distance, AActor* _actor, CharacterGunfire* _character, FVector _screenPos, FVector _aimPos, std::wstring _friendlyName)
		{
			TypeData = _typeData;
			Distance = _distance;
			Actor = _actor;
			Character = _character;
			ScreenPos = _screenPos;
			haveBonesMask = 0;
			AimPos = _aimPos;
			AimBone = BoneTarget();
			FriendlyName = _friendlyName;
			VisibleBones = std::vector<BoneTarget>();
		}

		ESPTarget()
		{
			TypeData = nullptr;
			Distance = 0.f;
			Actor = nullptr;
			Character = nullptr;
			ScreenPos = FVector(0.f, 0.f, 0.f);
			haveBonesMask = 0;
			AimPos = FVector(0.f, 0.f, 0.f);
			AimBone = BoneTarget();
			FriendlyName = std::wstring(64, 0x0);
			VisibleBones = std::vector<BoneTarget>();
		}

		void Reset()
		{
			this->TypeData = nullptr;
			this->Distance = 0.f;
			this->Actor = nullptr;
			this->Character = nullptr;
			this->ScreenPos = FVector(0.f, 0.f, 0.f);
			this->haveBonesMask = 0;
			this->AimPos = FVector(0.f, 0.f, 0.f);
			this->AimBone.Name = std::string("ERROR");
			this->AimBone.Position = FVector(0.f, 0.f, 0.f);
			this->FriendlyName = std::wstring(L"ERROR");
			this->VisibleBones.clear();
		}
	};

	void Entrypoint(HMODULE selfMod);
	bool LoadCommonClasses();
	bool IsValidGameState();
	std::wstring GetFriendlyObjectName(UObject* obj);
	bool IsRelevantActor(AActor* actor);
	void DrawTarget(AHUD* hud, ESPTarget* targetInfo);
	void LoadBoneInfo(ESPTarget* targetInfo);
	void LoadTargetInfo(ESPTarget* targetInfo, AActor* actor, AHUD* hud);
	ESPTarget* GetBestAimTarget();
	void ProcessTargets(AHUD* hud);
	void hkPostRender(UnrealEngine4::AHUD* gameHUD);
	void ImGuiDraw(ImDrawList* drawList);
	bool MyWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}