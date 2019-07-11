// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RG_BTTask_CautionTask.generated.h"

/**
 *
 */
UCLASS()
class MONSIEURMONET_API URG_BTTask_CautionTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
