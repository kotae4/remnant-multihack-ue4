## General / Code Design Checklist
- [ ] Do a review and favor C++ stl functions over winapi functions
- [ ] Thread safety review. Messing with game stuff from imgui handlers might be bad.
- [ ] Handle window resizing gracefully (currently crashes - only because of imgui buffers)
- [ ] Continually rethink framework design



## Nametag ESP Checklist
- [x] GObjects
	- [x] Structure, bucket size, total elements
- [x] GNames
	- [x] Bucket size, total elements
- [x] GEngine
	- [x] GEngine->SmallFont
	- [ ] (optional) GEngine->Viewport
		- Ended up going with the one on ULocalPlayer instead
- [x] GWorld
	- [x] GWorld->LevelList
		- [x] ULevel->ActorList
	- [x] GWorld->OwningGameInstance->LocalPlayers[]
		- [x] LocalPlayer->PlayerController
			- [x] PlayerController->HUD
				- [x] AHud->PostRender vtable index <--- Main hook target function
				- [x] AHud->DrawText()
				- [x] AHud->Project()
			- [x] PlayerController->Pawn
- [x] AActor->RootComponent->Position
- [x] UObject->GetFullName() <--- Pretty formatting, could implement by myself or could find the function address

## Aimbot Checklist
- [x] PlayerController->GetPlayerViewPoint()
- [x] PlayerController->SetControlRotation()
- [x] APawn->FaceRotation()
- [x] GWorld->LineTraceTestByChannel()
	- [x] GDefaultCollisionResponseParams address
	- [x] ECC_Visibility enum value (probably the same in every engine version: 3)
	- [x] FCollisionQueryParams struct

## Bone ESP Checklist
- [x] ACharacter->Mesh
	- [x] USkinnedMeshComponent->GetBoneName() <--- Gets the name index which we then have to read from GNames
	- [x] USkinnedMeshComponent->GetBoneMatrix()
	- [x] USkinnedMeshComponent->SkeletalMesh
		- [x] USkeletalMesh->RefSkeleton
		- [x] USkeletalMesh->RefSkeleton.NumBones
- [x] GWorld->LineBatcher
- [x] GWorld->LineBatcher->DrawLine()

## Box ESP Checklist
- [] GetActorBounds

## Nice-To-Haves Checklist
- [x] AActor->OwnedComponents
- [x] AActor->NumOwnedComponents
- [x] ACharacter->HealthPercentageTimes10
- [x] RemnantLootContainer->StateMachineCpnt
	- [x] RemnantStateMachineCpnt->CurrentState
		- [x] StateMachine_State->StateNameIndex

## Update Class Names Checklist
- [x] "Class /Script/Engine.Actor"
- [x] "Class /Script/Engine.Pawn"
- [x] "Class /Script/Engine.ActorComponent"
- [x] "Class /Script/Engine.SkinnedMeshComponent"