// This code belongs to Learning Machines.

#include "BTTask_Swift_SearchingForPlayer.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

EBTNodeResult::Type UBTTask_Swift_SearchingForPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SwiftGuard_Controller)
	{
		if (SwiftGuard_Character->CautionThreshold >= 3)
		{
			SwiftGuard_Character->NewPatrolLoc = true;
			return EBTNodeResult::Aborted;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTask_Swift_SearchingForPlayer::UBTTask_Swift_SearchingForPlayer()
{
	bCreateNodeInstance = true;
}
