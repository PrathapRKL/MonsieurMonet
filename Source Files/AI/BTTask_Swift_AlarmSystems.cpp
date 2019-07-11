// This code belongs to Learning Machines.

#include "BTTask_Swift_AlarmSystems.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_AlarmSystem.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

EBTNodeResult::Type UBTTask_Swift_AlarmSystems::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	SwiftGuard_Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	if (SwiftGuard_Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	AActor* AlarmSystems = SwiftGuard_Controller->GetAlarmSystems();
	if (AlarmSystems)
	{
		//To find a position that is close to the alarm systems.
		const float SearchRadius = 600.0f;
		const FVector SearchOrigin = AlarmSystems->GetActorLocation();
		SwiftGuard_Controller->SetEnemy(nullptr);
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(SwiftGuard_Controller);
		if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(SearchOrigin, SearchRadius, ResultLocation))
		{
			//The selected key should be "Alarm systems" in the Behavior Tree setup.
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), ResultLocation.Location);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}

UBTTask_Swift_AlarmSystems::UBTTask_Swift_AlarmSystems()
{
	bCreateNodeInstance = true;
}
