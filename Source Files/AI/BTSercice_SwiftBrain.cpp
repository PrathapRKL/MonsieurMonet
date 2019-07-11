// This code belongs to Learning Machines.

#include "BTSercice_SwiftBrain.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BTAuxiliaryNode.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"

UBTSercice_SwiftBrain::UBTSercice_SwiftBrain()
{
	NodeName = "SwiftGuard_Service";
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true;
}

void UBTSercice_SwiftBrain::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Idle_State));
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Idle_State));
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(SwiftGuard_Controller->State, uint8(AI_States_Swift::Idle_State));
}

void UBTSercice_SwiftBrain::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	FVector LineToPlayer = PlayerCharacter->GetActorLocation() - SwiftGuard_Character->GetActorLocation();
	FVector NormalizedVector = LineToPlayer.GetSafeNormal(1.0f);
	float VisionConeAngle = (acosf(FVector::DotProduct(NormalizedVector, SwiftGuard_Character->GetActorForwardVector())));

	if (VisionConeAngle <= 64.0f)
	{
		ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
		AActor* ActorsToIgnore = SwiftGuard_Character->GetCapsuleComponent()->GetOwner();

		FHitResult HitResult;
		FVector StartLocation = SwiftGuard_Character->GetActorLocation();
		FVector EndLocation = PlayerCharacter->GetActorLocation();
		FCollisionQueryParams PawnCQP;
		PawnCQP.AddIgnoredActor(ActorsToIgnore);

		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, PawnCQP);
		if (HitResult.Actor == PlayerCharacter)
		{
			AActor* Mem;
			SwiftGuard_Controller->SetEnemy(PlayerCharacter);
			//Setting the blackboard value to the player character.
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->TargetCharacter, PlayerCharacter);
			if (SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker) != nullptr)
			{
				//Memory Marker for last known location.
				AActor* MemoryMarker = Cast<AActor>(SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker));
				MemoryMarker->SetActorLocation(PlayerCharacter->GetActorLocation(), false, nullptr, ETeleportType::None);
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->MemoryMarker, MemoryMarker);
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Caution_State));
				SwiftGuard_Controller->PlayerCaution(PlayerCharacter, AI_States_Swift::Caution_State);
			}
			else
			{
				FActorSpawnParameters SpawnInfo;
				FVector MemLocation = PlayerCharacter->GetActorLocation();
				FRotator MemRotation = PlayerCharacter->GetActorRotation();
				TSubclassOf<ATargetPoint> MemActor;
				UWorld* World1 = GetWorld();
				Mem = World1->SpawnActor(MemActor, &MemLocation, &MemRotation, SpawnInfo);
				SwiftGuard_Controller->SetMemoryMarkers(Mem);
				AActor* MemoryMarker = Cast<AActor>(SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker));
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->MemoryMarker, MemoryMarker);
				SwiftGuard_Controller->PlayerCaution(PlayerCharacter, AI_States_Swift::Caution_State); 
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Caution_State));
			}
		}
		else
		{
			if (SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker) != nullptr)
			{
				AActor* MemoryMarker = Cast<AActor>(SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker));
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::SearchingForPlayer_State));
				SwiftGuard_Controller->SearchForPlayer(MemoryMarker, AI_States_Swift::SearchingForPlayer_State);
			}
			else
			{
				SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Idle_State));
			}
		}
	}
	else
	{
		if (SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker) != nullptr)
		{
			AActor* MemoryMarker = Cast<AActor>(SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker));
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::SearchingForPlayer_State));
			SwiftGuard_Controller->SearchForPlayer(MemoryMarker, AI_States_Swift::SearchingForPlayer_State);
		}
		else
		{
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Idle_State));
		}
	}
	if (SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker))
	{
		AActor* MemoryMarker = Cast<AActor>(SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject(SwiftGuard_Controller->MemoryMarker));
		UWorld* World = GetWorld();
		DrawDebugSphere(World, MemoryMarker->GetActorLocation(), 0.3f, 7, FColor::Cyan, false, 0.6f, 0.0f);
	}
}

