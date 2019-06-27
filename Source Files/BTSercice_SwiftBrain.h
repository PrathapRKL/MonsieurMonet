// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTSercice_SwiftBrain.generated.h"

/**
 *
 */
UCLASS()
class MONSIEURMONET_API UBTSercice_SwiftBrain : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTSercice_SwiftBrain();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FBlackboardKeySelector State;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FBlackboardKeySelector LastknownLoc;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FBlackboardKeySelector TargetActor;
};
