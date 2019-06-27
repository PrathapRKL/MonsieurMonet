// This code belongs to Learning Machines.

#include "MyBTTask_Patroltask.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Regular_GuardCharacter.h"
#include "Regular_GuardController.h"

EBTNodeResult::Type UMyBTTask_Patroltask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(OwnerComp.GetAIOwner());
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(RegularGuard_Controller->GetPawn());
	RegularGuard->GetCharacterMovement()->MaxWalkSpeed = 140.0f;
	if (RegularGuard_Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AMM_PatrolPoints* Waypoints = RegularGuard_Controller->GetWaypoint();
	if (Waypoints)
	{
		//To Find a position that is close to the waypoint.
		const float SearchRadius = 300.0f;
		const FVector SearchOrigin = Waypoints->GetActorLocation();
		RegularGuard_Controller->SetEnemy(nullptr);
		RegularGuard->bPawnSensing_Caution = false;
		RegularGuard->bPawnSensing_Alert = false;
		RegularGuard->bPawnSensing_Combat = false;
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(RegularGuard_Controller);
		if (RegularGuard->bPawnSensing_Caution == false || RegularGuard->bPawnSensing_Alert == false || RegularGuard->bPawnSensing_Combat == false)
		{
			if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(SearchOrigin, SearchRadius, ResultLocation))
			{
				//The selected key should be "Patrol points in the Behavior Tree Setup."
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), ResultLocation.Location);
				RegularGuard_Controller->BlackboardDataComp->SetValueAsEnum(RegularGuard_Controller->State, uint8(AI_States::Patrolling_State));
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
