// This code belongs to Learning Machines.

#include "BTTask_Swift_AlarmSystemLoc.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_AlarmSystem.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

EBTNodeResult::Type UBTTask_Swift_AlarmSystemLoc::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	SwiftGuard_Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	if (SwiftGuard_Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* NewAlarmSystem = nullptr;

	//Iterate all the bot waypoints in the current level and find a new random waypoint to set as destination.
	TArray<AActor*> AllAlarmSystems = SwiftGuard_Character->AlarmSystems;

	if (SwiftGuard_Character->AlarmSystems.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	NewAlarmSystem = AllAlarmSystems[SwiftGuard_Character->AlarmIndex];

	//Assign the new waypoint to the blackboard.
	int Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt(SwiftGuard_Controller->AlarmIndex);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(SwiftGuard_Controller->AlarmIndex, SwiftGuard_Character->AlarmIndex);
	/*OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);*/
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(SwiftGuard_Controller->AlarmSystems, AllAlarmSystems[SwiftGuard_Character->AlarmIndex]);

	//Cycle the Index.
	int NextIndex = (Index + 1) % AllAlarmSystems.Num();
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(SwiftGuard_Controller->AlarmIndex, NextIndex);

	return EBTNodeResult::Succeeded;
}

UBTTask_Swift_AlarmSystemLoc::UBTTask_Swift_AlarmSystemLoc()
{
	bCreateNodeInstance = true;
}
