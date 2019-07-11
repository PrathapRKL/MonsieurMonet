// This code belongs to Learning Machines.

#include "BTTask_Swift_AlertState.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Public/NavMesh/RecastNavMesh.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"

UBTTask_Swift_AlertState::UBTTask_Swift_AlertState()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Swift_AlertState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UWorld* World = GetWorld();
	/*if (World == nullptr)
	{
		return ;
	}*/
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SwiftGuard_Controller == nullptr || SwiftGuard_Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	FTimerHandle DelayHandle;
	//Play animation montage.
	SwiftGuard_Character->PlayAnimMontage(SwiftGuard_Character->Scared_Montage, 1.0f);
	//Delay to make the cahracter wait a while before performing any further actions.
	World->GetTimerManager().SetTimer(DelayHandle, 2.0f, false, 1.0f);

	//TSubclassOf<ARecastNavMesh> RecastNav;
	//TArray<AActor*> OutActors;
	//UGameplayStatics::GetAllActorsOfClass(World, RecastNav, OutActors);
	/*ANavigationData* NavData;*/

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(SwiftGuard_Controller);
	/*for (AActor* RNavMesh : OutActors)
	{*/FNavLocation RandomLocation;
		NavSystem->GetRandomPointInNavigableRadius(SwiftGuard_Character->GetActorLocation(), 1000.0f, RandomLocation);
	if (SwiftGuard_Character->FoundPlayer == false && SwiftGuard_Character->CanSeePlayer == false && SwiftGuard_Character->CanHearPlayer == false)
	{
		SwiftGuard_Controller->MoveToLocation(RandomLocation.Location, 30.0f, true, true, true, true);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Aborted;
	}
	/*}*/
	return EBTNodeResult::Succeeded;
}
