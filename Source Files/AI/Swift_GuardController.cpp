// Fill out your copyright notice in the Description page of Project Settings.

#include "Swift_GuardController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Swift_GuardCharacter.h"
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
#include "Runtime/Core/Public/Async/ParallelFor.h"

ASwift_GuardController::ASwift_GuardController()
{
	BlackboardDataComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardData"));
	BTTree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	bWantsPlayerState = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIPerception->bEditableWhenInherited = true;

	AISenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AISenseHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	AISenseDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));

	AIPerception->ConfigureSense(*AISenseSight);
	AIPerception->ConfigureSense(*AISenseHearing);
	AIPerception->ConfigureSense(*AISenseDamage);
	AIPerception->SetDominantSense(AISenseSight->GetSenseImplementation());

	AISenseSight->SightRadius = 2000.0f;
	AISenseSight->LoseSightRadius = 2600.0f;
	AISenseSight->PeripheralVisionAngleDegrees = 64.0f;
	AISenseSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseSight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseSight->SetMaxAge(3.0f);

	/*AISenseDamage->;*/


	/*AIPerception->SetDominantSense();
	for (int i = 0; i <= AISenseConfig->Get(); i++)
	{
		AISenseConfig[i]->
			AISenseConfig->Sense
	}*/

	/*AIPerception->ConfigureSense();*/

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
	/*IsStunned = "IsStunned";*/
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

AMM_PatrolPoints * ASwift_GuardController::GetWaypoint()
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
		if (!SwiftGuard_Character->IsStunned)
		{
			for (int i = 0; i <= SwiftGuard_Character->PatrolPoints.Num(); i++)
			{
			/*	if (i == SwiftGuard_Character->PatrolPoints.Max())
				{
					for (int j = SwiftGuard_Character->PatrolPoints.Max(); j < SwiftGuard_Character->PatrolPoints.Num(); j--)
						return SwiftGuard_Character->PatrolPoints[j];
				}*/
				return SwiftGuard_Character->PatrolPoints[i];
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Parellel For
			//FCriticalSection Mutex;
			//ParallelFor(SwiftGuard_Character->PatrolPoints.Num(), [&](int32 i = 0)
			//{
			//	check(i <= SwiftGuard_Character->PatrolPoints.Num());
			//	Mutex.Lock();
			//	SwiftGuard_Character->PatrolPoints[i]++;
			//	/*if (i <= SwiftGuard_Character->PatrolPoints.Num())*/
			//	return SwiftGuard_Character->PatrolPoints[i];
			//	Mutex.Unlock();
			//});
		}
	}
	return nullptr;
}

AActor * ASwift_GuardController::GetAlarmSystems()
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
		if (!SwiftGuard_Character->IsStunned)
		{
			for (int i = 0; i < SwiftGuard_Character->AlarmSystems.Num(); i++)
				return SwiftGuard_Character->AlarmSystems[i];
		}
	}
	return nullptr;
}

void ASwift_GuardController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
}

void ASwift_GuardController::GameHasEnded(AActor * EndGameFocus, bool bIsWinner)
{
}

void ASwift_GuardController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(InPawn);

	if (SwiftGuard->BTTree->BlackboardAsset)
	{
		BlackboardDataComp->InitializeBlackboard(*SwiftGuard->BTTree->BlackboardAsset);
	}
	BTTree_Component->StartTree(*SwiftGuard->BTTree);
}

void ASwift_GuardController::UnPossess()
{
	Super::UnPossess();
}

void ASwift_GuardController::BeginInactiveState()
{
}

AMonsieurMonet_Character * ASwift_GuardController::GetEnemy()
{
	if (BlackboardDataComp)
	{
		return Cast<AMonsieurMonet_Character>(BlackboardDataComp->GetValueAsObject(TargetCharacter));
	}
	return nullptr;
}

void ASwift_GuardController::SetIdleState(AI_States_Swift Idle_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, uint8(Idle_State));
	}
}

void ASwift_GuardController::SetEnemy(APawn * InPawn)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, InPawn);
	}
}

void ASwift_GuardController::PlayerCaution(APawn * Pawn, AI_States_Swift Caution_State)
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(GetPawn());
		/*SwiftGuard->bPawnSensing_Caution = true;*/
		SwiftGuard->CautionState();
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Caution_State);
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
	}
}

void ASwift_GuardController::PlayerAlert(APawn * Pawn, AI_States_Swift Alert_State)
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(GetPawn());
		SwiftGuard->bPawnSensing_Alert = true;
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Alert_State);
	}
}

void ASwift_GuardController::LookForPlayer(APawn * Pawn, AI_States_Swift LookingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(GetPawn());
		SwiftGuard->IsLookingForPlayer = true;
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)LookingForPlayer_State);
	}
}

void ASwift_GuardController::SearchForPlayer(AActor * Pawn, AI_States_Swift SearchingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(GetPawn());
		SwiftGuard->IsSearchingForPlayer = true;
		BlackboardDataComp->SetValueAsObject(MemoryMarker, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)SearchingForPlayer_State);
	}
}

void ASwift_GuardController::FoundPlayer(APawn * Pawn, AI_States_Swift FoundPlayer_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)FoundPlayer_State);
	}
}

void ASwift_GuardController::RestoringArtifact(APawn * Pawn, AI_States_Swift RestoringArtifact_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)RestoringArtifact_State);
	}
}

void ASwift_GuardController::Scared(APawn * Pawn, AI_States_Swift Scared_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Scared_State);
	}
}

void ASwift_GuardController::Lured(AI_States_Swift Lured_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Lured_State);
	}
}

void ASwift_GuardController::Stunned(AI_States_Swift Stunned_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Stunned_State);
	}
}

void ASwift_GuardController::Communicating(AI_States_Swift Communicating_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Communicating_State);
	}
}

void ASwift_GuardController::IntenseSearch(APawn * Pawn, AI_States_Swift IntenseSearch_State)
{
	if (BlackboardDataComp)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(Pawn);
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)IntenseSearch_State);
	}
}

void ASwift_GuardController::DisablingInteractables(AActor * Actor, AI_States_Swift DisablingInteractables_State)
{
	if (BlackboardDataComp)
	{


		BlackboardDataComp->SetValueAsObject(InteractableItems, Actor);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)DisablingInteractables_State);
	}
}

void ASwift_GuardController::Observing(AActor * Actor, AI_States_Swift Observing_State)
{
	if (BlackboardDataComp)
	{


		BlackboardDataComp->SetValueAsObject(Artifacts, Actor);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Observing_State);
	}
}

void ASwift_GuardController::AlertingOtherGuards(TArray<AActor*> Actors, AI_States_Swift AlertingGuards_State)
{
}

void ASwift_GuardController::Distracted(AActor* Actors, AI_States_Swift Lured_State)
{
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(AlarmSystems, Actors);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Lured_State);
		SwiftGuard_Character->DistractedState(Actors);
	}
}

void ASwift_GuardController::Patrolling(APawn * Pawn, AI_States_Swift Patrolling_State)
{
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
	SwiftGuard_Character->IsPatrolling = true;
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Patrolling_State);
	}
}

void ASwift_GuardController::Guarding(APawn * Pawn, AI_States_Swift Guarding_State)
{
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
	SwiftGuard_Character->IsGuarding = true;
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Guarding_State);
	}
}

void ASwift_GuardController::NewPatrol(APawn * Pawn, AI_States_Swift NewPatrol_State)
{
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
	if (SwiftGuard_Character->NewPatrolLoc == true)
	{
		if (BlackboardDataComp)
		{
			BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
			BlackboardDataComp->SetValueAsEnum(State, (uint8)NewPatrol_State);
		}
	}
}

void ASwift_GuardController::SetWaypoints(AMM_PatrolPoints * NewPatrolPoint, AI_States_Swift Patrolling_State)
{
	if (BlackboardDataComp)
	{
		ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(GetPawn());
		for (int i = 0; i <= SwiftGuard_Character->PatrolPoints.Num(); i++)
			NewPatrolPoint = SwiftGuard_Character->PatrolPoints[i];
		BlackboardDataComp->SetValueAsObject(Waypoints, NewPatrolPoint);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Patrolling_State);
	}
}

void ASwift_GuardController::SetMemoryMarkers(AActor * MemMarker)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(MemoryMarker, MemMarker);
	}
}

void ASwift_GuardController::GetMemoryMarkers()
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->GetValueAsObject(MemoryMarker);
	}
}
