// This code belongs to Learning Machines.

#include "BTTask_Swift_CautionState.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_Swift_CautionState::UBTTask_Swift_CautionState()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Swift_CautionState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (/*SwiftGuard_Character == nullptr || */PlayerCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (SwiftGuard_Controller)
	{
		if (SwiftGuard_Character == nullptr)
			return EBTNodeResult::Failed;
		/*SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->TargetCharacter, PlayerCharacter);
		SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Caution_State));*/
		SwiftGuard_Character->CautionState();
		int32 SuspicionCounter = SwiftGuard_Controller->BlackboardDataComp->GetValueAsInt(SwiftGuard_Controller->SuspicionCounter);;
		SuspicionCounter += 1;
		if (SwiftGuard_Character->CautionThreshold >= 4)
		{
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->TargetCharacter, PlayerCharacter);
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsInt(SwiftGuard_Controller->SuspicionCounter, SuspicionCounter);
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::Alert_State));
			return EBTNodeResult::Aborted;
		}
			return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
