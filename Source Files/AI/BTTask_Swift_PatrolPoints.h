// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Swift_PatrolPoints.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API UBTTask_Swift_PatrolPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

public:
	UBTTask_Swift_PatrolPoints();
};
