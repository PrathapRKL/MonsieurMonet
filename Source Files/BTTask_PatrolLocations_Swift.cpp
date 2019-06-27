// This code belongs to Learning Machines.

#include "BTTask_PatrolLocations_Swift.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_PatrolLocations_Swift::UBTTask_PatrolLocations_Swift()
{
	bCreateNodeInstance = true;
	/*bNotifyTick = true;
	bNotifyTaskFinished = true;*/
}

EBTNodeResult::Type UBTTask_PatrolLocations_Swift::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	SwiftGuard_Character->GetCharacterMovement()->MaxWalkSpeed = 160.0f;
	if (SwiftGuard_Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	/*else
	{
	}*/
	AActor* NewWaypoint = nullptr;

	//Iterate all the bot waypoints in the current level and find a new random waypoint to set as destination.
	TArray<AMM_PatrolPoints*> AllWayPoints = SwiftGuard_Character->PatrolPoints;


	if (SwiftGuard_Character->PatrolPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("jfds;d;lkdsahfl;sadjf"));
	int32 CurrentWayPointNum = 0;
	/*FVector PatrolDistance = SwiftGuard_Character->GetActorLocation() - NewWaypoint->GetActorLocation();*/
	//NewWaypoint = SwiftGuard_Character->PatrolPoints[FMath::RandRange(0, SwiftGuard_Character->PatrolPoints.Num() - 1)];
	if (AllWayPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}
	NewWaypoint = SwiftGuard_Controller->GetWaypoint();


	//Assign the new waypoint to the blackboard.
	int Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(SwiftGuard_Controller->IndexKey);
	/*OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);*/
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(SwiftGuard_Controller->Waypoints, AllWayPoints[Index]);

	//Cycle the Index.
	int NextIndex = (Index + 1) % AllWayPoints.Num();
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(SwiftGuard_Controller->IndexKey, NextIndex);

	return EBTNodeResult::Succeeded;
}

