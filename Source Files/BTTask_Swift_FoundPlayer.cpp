// This code belongs to Learning Machines.

#include "BTTask_Swift_FoundPlayer.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_Swift_FoundPlayer::UBTTask_Swift_FoundPlayer()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Swift_FoundPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	FoundPlayerCount += 1;
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (SwiftGuard_Controller)
	{
		SwiftGuard_Controller->BlackboardDataComp->SetValueAsObject(SwiftGuard_Controller->TargetCharacter, PlayerCharacter);
		SwiftGuard_Character->FoundPlayer = true;
		SwiftGuard_Character->FoundPlayerState();
		/*return EBTNodeResult::Succeeded;*/

		if (FoundPlayerCount >= 4)
		{
			SwiftGuard_Character->SweepRadius = 650.0f;
		}
		return EBTNodeResult::Succeeded;
	}
	SwiftGuard_Character->FoundPlayer = false;
	return EBTNodeResult::Failed;
}
