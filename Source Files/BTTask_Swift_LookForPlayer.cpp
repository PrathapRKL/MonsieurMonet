// This code belongs to Learning Machines.

#include "BTTask_Swift_LookForPlayer.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "MonsieurMonet_Character.h"
#include "Swift_GuardCharacter.h"
#include "Swift_GuardController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Swift_LookForPlayer::UBTTask_Swift_LookForPlayer()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Swift_LookForPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(OwnerComp.GetAIOwner());
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(SwiftGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (SwiftGuard_Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	FRotator FacePlayer = UKismetMathLibrary::FindLookAtRotation(SwiftGuard_Character->GetActorLocation(), PlayerCharacter->GetActorLocation());
	FRotator NewRot = FMath::RInterpTo(SwiftGuard_Character->GetActorRotation(), FacePlayer, GetWorld()->GetDeltaSeconds(), 3.0f);
	SwiftGuard_Character->SetActorRotation(FRotator(0.0f, NewRot.Yaw, 0.0f));
	return EBTNodeResult::Succeeded;
}
