// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Swift_CautionState.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API UBTTask_Swift_CautionState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

		UBTTask_Swift_CautionState();

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
