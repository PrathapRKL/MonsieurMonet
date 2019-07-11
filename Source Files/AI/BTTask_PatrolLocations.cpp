// This code belongs to Learning Machines.

#include "BTTask_PatrolLocations.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Regular_GuardCharacter.h"
#include "Regular_GuardController.h"

EBTNodeResult::Type UBTTask_PatrolLocations::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(OwnerComp.GetAIOwner());
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(RegularGuard_Controller->GetPawn());
	RegularGuard->GetCharacterMovement()->MaxWalkSpeed = 140.0f;
	if (RegularGuard == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AMM_PatrolPoints* CurrentWaypoint = RegularGuard_Controller->GetWaypoint();
	AActor* NewWaypoint = nullptr;

	//Iterate all the bot waypoints in the current level and find a new random waypoint to set as destination.
	TArray<AActor*> AllWayPoints;
	UGameplayStatics::GetAllActorsOfClass(RegularGuard_Controller, AMM_PatrolPoints::StaticClass(), AllWayPoints);

	if (AllWayPoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	//For more complex or human-like AI, add more weights based on distance.
	NewWaypoint = AllWayPoints[FMath::RandRange(0, AllWayPoints.Num() - 1)];

	//Assign the new waypoint to the blackboard.
	if (NewWaypoint)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
