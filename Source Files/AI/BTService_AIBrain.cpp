// This code belongs to Learning Machines.

#include "BTService_AIBrain.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Regular_GuardCharacter.h"
#include "Regular_GuardController.h"
#include "Kismet/GameplayStatics.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"

UBTService_AIBrain::UBTService_AIBrain()
{
	bCreateNodeInstance = true;
}

void UBTService_AIBrain::OnSearchStart(FBehaviorTreeSearchData & SearchData)
{
}

void UBTService_AIBrain::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(OwnerComp.GetAIOwner());
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(RegularGuard_Controller->GetPawn());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (RegularGuard->bPawnSensing_Caution == true)
	{
		RegularGuard_Controller->PlayerCaution(PlayerCharacter, AI_States::Caution_State);
	}
	else if (RegularGuard->bPawnSensing_Alert == true)
	{
		RegularGuard_Controller->PlayerAlert(PlayerCharacter, AI_States::Alert_State);
	}
	else if (RegularGuard->FoundPlayer == true)
	{
		RegularGuard_Controller->FoundPlayer(PlayerCharacter, AI_States::FoundPlayer_State);
	}
	else if (RegularGuard->IsLookingForPlayer == true)
	{
		RegularGuard_Controller->LookForPlayer(PlayerCharacter, AI_States::LookingForPlayer_State);
	}
	else if (RegularGuard->IsSearchingForPlayer == true)
	{
		RegularGuard_Controller->SearchForPlayer(PlayerCharacter, AI_States::SearchingForPlayer_State);
	}
	else if (RegularGuard->IsScared == true)
	{
		RegularGuard_Controller->Scared(PlayerCharacter,  AI_States::Scared_State);
	}
	else
	{
		RegularGuard->IsScared = false;
		RegularGuard->IsSearchingForPlayer = false;
		RegularGuard->bPawnSensing_Caution = false;
		RegularGuard->bPawnSensing_Alert = false;
		RegularGuard->FoundPlayer = false;
		RegularGuard->IsLookingForPlayer = false;

		RegularGuard_Controller->BlackboardDataComp->SetValueAsEnum(RegularGuard_Controller->State, uint8(AI_States::Patrolling_State));
	}
}
