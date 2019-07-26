// This code belongs to Learning Machines.

#include "BTTask_Swift_PatrolPoints.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_Swift_PatrolPoints::UBTTask_Swift_PatrolPoints()
{
	bCreateNodeInstance = true;
	//bNotifyTick = true;
	//bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Swift_PatrolPoints::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	if (SwiftGuard_Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (SwiftGuard_Character == nullptr)
		return EBTNodeResult::Failed;
	SwiftGuard_Character->GetCharacterMovement()->MaxWalkSpeed = 160.0f;
	AMM_PatrolPoints* Waypoints = SwiftGuard_Controller->GetWaypoint();
	if (Waypoints)
	{
		//To find a position that is close to the way point.
		const float SearchRadius = 6400.0f;
		const FVector SearchOrigin = Waypoints->GetActorLocation();
		SwiftGuard_Controller->SetEnemy(nullptr);
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(SwiftGuard_Controller);
		if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(SearchOrigin, SearchRadius, ResultLocation))
		{
			//The selected key should be "Patrol/Way points" in the BehaviorTree setup.
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), ResultLocation.Location);
			//SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Patrolling_State));
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

