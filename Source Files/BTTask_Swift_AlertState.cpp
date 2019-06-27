// This code belongs to Learning Machines.

#include "BTTask_Swift_AlertState.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_Swift_AlertState::UBTTask_Swift_AlertState()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Swift_AlertState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SwiftGuard_Controller)
	{
		SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->TargetCharacter, PlayerCharacter);
		SwiftGuard_Character->AlertState();
		if (SwiftGuard_Character->AlertThreshold >= 4)
		{
			SwiftGuard_Character->IsSearchingForPlayer = true;
			SwiftGuard_Controller->BlackboardDataComp->SetValueAsEnum(SwiftGuard_Controller->State, uint8(AI_States_Swift::IntenseSearch_State));
			return EBTNodeResult::Aborted;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
