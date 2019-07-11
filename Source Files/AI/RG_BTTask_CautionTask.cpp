// This code belongs to Learning Machines.

#include "RG_BTTask_CautionTask.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Regular_GuardCharacter.h"
#include "Regular_GuardController.h"

EBTNodeResult::Type URG_BTTask_CautionTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(OwnerComp.GetAIOwner());
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(RegularGuard_Controller->GetPawn());
	if (RegularGuard->bPawnSensing_Caution == true)
	{
		RegularGuard->CautionThreshold += 1;
		RegularGuard->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		return EBTNodeResult::Succeeded;
		if (RegularGuard->CautionThreshold == 3)
		{
			RegularGuard->bPawnSensing_Alert = true;
			return EBTNodeResult::Aborted;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
