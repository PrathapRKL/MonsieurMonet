// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolLocations_Swift.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API UBTTask_PatrolLocations_Swift : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

public:
	UBTTask_PatrolLocations_Swift();
};
