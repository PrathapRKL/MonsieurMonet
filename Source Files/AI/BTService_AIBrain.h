// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AIBrain.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API UBTService_AIBrain : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_AIBrain();

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
