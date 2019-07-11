// Fill out your copyright notice in the Description page of Project Settings.

#include "Regular_GuardController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Regular_GuardCharacter.h"
#include "AI/MM_PatrolPoints.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ARegular_GuardController::ARegular_GuardController()
{
	BlackboardDataComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardData"));
	BTTree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	/*AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));*/

	//Initialize Blackboard Keys.
	TargetCharacter = "TargetCharacter";
	AlarmLocations = "AlarmLocations";
	ArtifactLocations = "ArtifactLocations";
	State = "State";
	PlayerHidingLocation_approx = "PlayerHidingLocation_approx";
	PlayerHidingLocation_exact = "PlayerHidingLocation_exact";
	MemoryLocation = "MemoryLocation";
	ArtifactCount = "ArtifactCount";
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

	bWantsPlayerState = true;
	CurrentWayPoint = 0;
}

void ARegular_GuardController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(InPawn);

	if (RegularGuard->BTTree->BlackboardAsset)
	{
		BlackboardDataComp->InitializeBlackboard(*RegularGuard->BTTree->BlackboardAsset);
	}
	BTTree_Component->StartTree(*RegularGuard->BTTree);
}

void ARegular_GuardController::SetWaypoints(AMM_PatrolPoints * NewPatrolPoint, AI_States Patrolling_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(Waypoints, NewPatrolPoint);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Patrolling_State);
	}
}

void ARegular_GuardController::SetPlayerCaught(APawn * Pawn, AI_States Combat_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Combat_State);
	}
}

void ARegular_GuardController::PlayerCaution(APawn * Pawn, AI_States Caution_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Caution_State);
	}
}

void ARegular_GuardController::PlayerAlert(APawn * Pawn, AI_States Alert_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)Alert_State);
	}
}

void ARegular_GuardController::LookForPlayer(APawn * Pawn, AI_States LookingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)LookingForPlayer_State);
	}
}

void ARegular_GuardController::SearchForPlayer(APawn * Pawn, AI_States SearchingForPlayer_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, (uint8)SearchingForPlayer_State);
	}
}

void ARegular_GuardController::FoundPlayer(APawn * Pawn, AI_States FoundPlayer_State)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
		BlackboardDataComp->SetValueAsEnum(State, uint8(FoundPlayer_State));
	}
}

void ARegular_GuardController::RestoringArtifact(APawn * Pawn, AI_States RestoringArtifact_State)
{
}

void ARegular_GuardController::Scared(APawn * Pawn, AI_States Scared_State)
{
}

AMM_PatrolPoints * ARegular_GuardController::GetWaypoint()
{
	if (BlackboardDataComp)
	{
		return Cast<AMM_PatrolPoints>(BlackboardDataComp->GetValueAsObject(Waypoints));
	}
	return nullptr;
}

void ARegular_GuardController::SetEnemy(APawn * Pawn)
{
	if (BlackboardDataComp)
	{
		BlackboardDataComp->SetValueAsObject(TargetCharacter, Pawn);
	}
}

AMonsieurMonet_Character * ARegular_GuardController::GetEnemy()
{
	if (BlackboardDataComp)
	{
		return Cast<AMonsieurMonet_Character>(BlackboardDataComp->GetValueAsObject(TargetCharacter));
	}
	return nullptr;
}

bool ARegular_GuardController::FindClosestEnemyWithLOS(AMonsieurMonet_Character * ExcludeEnemy)
{
	return false;
}

bool ARegular_GuardController::HasWeaponLOSToEnemy(AActor * InEnemyActor, const bool bAnyEnemy) const
{
	return false;
}

void ARegular_GuardController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
}

bool ARegular_GuardController::LOSTrace(AMonsieurMonet_Character * InEnemyChar) const
{
	return false;
}
