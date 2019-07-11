// Fill out your copyright notice in the Description page of Project Settings.

#include "Brute_GuardController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Brute_GuardCharacter.h"
#include "MM_PatrolPoints.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Prediction.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Swift_GuardController.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"

ABrute_GuardController::ABrute_GuardController()
{
	BlackboardDataComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardData"));
	BTTree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	bWantsPlayerState = true;


	//Initialize Blackboard Keys.
	TargetCharacter = "TargetCharacter";
	AlarmLocations = "AlarmLocations";
	ArtifactLocations = "ArtifactLocations";
	State = "State";
	PlayerHidingLocation_approx = "PlayerHidingLocation_approx";
	PlayerHidingLocation_exact = "PlayerHidingLocation_exact";
	MemoryLocation = "MemoryLocation";
	LureCount = "LureCount";
	Player_LOS_Count = "Player_LOS_Count";
	IsAlarmTripped = "IsAlarmTripped";
	IsDetectedByCamera = "IsDetectedByCamera";
	IsDetectedByLaserwire = "IsDetectedByLaserwire";
	IsAllyHit = "IsAllyHit";
	IsShot = "IsShot";
	CanSeePlayer = "CanSeePlayer";
	CanStillSeePlayer = "CanStillSeePlayer";
	HasLineofSight = "HasLineofSight";
	IsGuarding = "IsGuarding";
	NewPatrolLocation = "NewPatrolLocation";
	NewGuardLocation = "NewGuardLocation";
	Waypoints = "WayPoints";
	MemoryMarker = "MemoryMarker";
	StunnedCount = "StunnedCount";
	Artifacts = "Artifacts";
	LostAndFound = "LostAndFound";
	AlarmSystems = "AlarmSystems";
	InteractableItems = "InteractableItems";
	HidingspotLocations = "HidingspotLocations";
	HidingSpots = "HidingSpots";
	IsAlone = "IsAlone";
	IsGuardNearby = "IsGuardNearby";
	IsAlarmSystemNearby = "IsAlarmNearby";
	IsLured = "IsLured";
	SuspicionCounter = "SuspicionCounter";
	IsCommunicating = "IsCommunicating";
	IsDistracted = "IsDistracted";
	IndexKey = "IndexKey";
	AlarmIndex = "AlarmIndex";

	bWantsPlayerState = true;
	CurrentWayPoint = 0;
}

AMM_PatrolPoints * ABrute_GuardController::GetWaypoint()
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
		if (!BruteGuard_Character->IsStunned)
		{
			for (int i = 0; i <= BruteGuard_Character->PatrolPoints.Num(); i++)
			{
				return BruteGuard_Character->PatrolPoints[i];
			}
		}
	}
	return nullptr;
}

AActor * ABrute_GuardController::GetAlarmSystems()
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
		if (!BruteGuard_Character->IsStunned)
		{
			for (int i = 0; i < BruteGuard_Character->AlarmSystems.Num(); i++)
				return BruteGuard_Character->AlarmSystems[i];
		}
	}
	return nullptr;
}

void ABrute_GuardController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
}

void ABrute_GuardController::GameHasEnded(AActor * EndGameFocus, bool bIsWinner)
{
}

void ABrute_GuardController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(InPawn);

	if (BruteGuard->BTTree->BlackboardAsset)
	{
		BlackboardDataComp->InitializeBlackboard(*BruteGuard->BTTree->BlackboardAsset);
	}
	BTTree_Component->StartTree(*BruteGuard->BTTree);
}

void ABrute_GuardController::UnPossess()
{
	Super::UnPossess();
}

void ABrute_GuardController::BeginInactiveState()
{
}

void ABrute_GuardController::SetIdleState(AI_States_Swift Idle_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, uint8(Idle_State));
	}
}

void ABrute_GuardController::SetEnemy(APawn * InPawn)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, InPawn);
	}
}

void ABrute_GuardController::PlayerCaution(APawn * Pawn, AI_States_Swift Caution_State)
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(GetPawn());
		/*SwiftGuard->bPawnSensing_Caution = true;*/
		BruteGuard->CautionState();
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Caution_State);
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
	}
}

void ABrute_GuardController::PlayerAlert(APawn * Pawn, AI_States_Swift Alert_State)
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(GetPawn());
		BruteGuard->bPawnSensing_Alert = true;
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Alert_State);
	}
}

void ABrute_GuardController::LookForPlayer(APawn * Pawn, AI_States_Swift LookingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(GetPawn());
		BruteGuard->IsLookingForPlayer = true;
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)LookingForPlayer_State);
	}
}

void ABrute_GuardController::SearchForPlayer(AActor * Pawn, AI_States_Swift SearchingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(GetPawn());
		BruteGuard->IsSearchingForPlayer = true;
		BlackboardDataComp->SetValueAsObject(MemoryMarker, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)SearchingForPlayer_State);
	}
}

void ABrute_GuardController::FoundPlayer(APawn * Pawn, AI_States_Swift FoundPlayer_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)FoundPlayer_State);
	}
}

void ABrute_GuardController::RestoringArtifact(APawn * Pawn, AI_States_Swift RestoringArtifact_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)RestoringArtifact_State);
	}
}

void ABrute_GuardController::Scared(APawn * Pawn, AI_States_Swift Scared_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Scared_State);
	}
}

void ABrute_GuardController::Lured(AI_States_Swift Lured_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Lured_State);
	}
}

void ABrute_GuardController::Stunned(AI_States_Swift Stunned_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Stunned_State);
	}
}

void ABrute_GuardController::Communicating(AI_States_Swift Communicating_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Communicating_State);
	}
}

void ABrute_GuardController::IntenseSearch(APawn * Pawn, AI_States_Swift IntenseSearch_State)
{
	if (BlackboardDataComp)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(Pawn);
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)IntenseSearch_State);
	}
}

void ABrute_GuardController::DisablingInteractables(AActor * Actor, AI_States_Swift DisablingInteractables_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(InteractableItems, Actor);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)DisablingInteractables_State);
	}
}

void ABrute_GuardController::Observing(AActor * Actor, AI_States_Swift Observing_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(Artifacts, Actor);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Observing_State);
	}
}

void ABrute_GuardController::AlertingOtherGuards(TArray<AActor*> Actors, AI_States_Swift AlertingGuards_State)
{
}

void ABrute_GuardController::Distracted(AActor * Actors, AI_States_Swift Lured_State)
{
	ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(AlarmSystems, Actors);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Lured_State);
		BruteGuard_Character->DistractedState(Actors);
	}
}

void ABrute_GuardController::Patrolling(APawn * Pawn, AI_States_Swift Patrolling_State)
{
	ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
	BruteGuard_Character->IsPatrolling = true;
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Patrolling_State);
	}
}

void ABrute_GuardController::Guarding(APawn * Pawn, AI_States_Swift Guarding_State)
{
	ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
	BruteGuard_Character->IsGuarding = true;
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Guarding_State);
	}
}

void ABrute_GuardController::NewPatrol(APawn * Pawn, AI_States_Swift NewPatrol_State)
{
	ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
	if (BruteGuard_Character->NewPatrolLoc == true)
	{
		if (BlackboardDataComp)
		{
			BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
			BlackboardDataComp->SetValueAsEnum(State, (uint8)NewPatrol_State);
		}
	}
}

void ABrute_GuardController::SetWaypoints(AMM_PatrolPoints * NewPatrolPoint, AI_States_Swift Patrolling_State)
{
	if (BlackboardDataComp)
	{
		ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(GetPawn());
		for (int i = 0; i <= BruteGuard_Character->PatrolPoints.Num(); i++)
			NewPatrolPoint = BruteGuard_Character->PatrolPoints[i];
		BlackboardDataComp->SetValueAsObject(Waypoints, NewPatrolPoint);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Patrolling_State);
	}
}

void ABrute_GuardController::SetMemoryMarkers(AActor * MemMarker)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(MemoryMarker, MemMarker);
	}
}

void ABrute_GuardController::GetMemoryMarkers()
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->GetValueAsObject(MemoryMarker);
	}
}
