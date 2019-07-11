// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/MM_Interactables_Base.h"
#include "MM_Pickups.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API AMM_Pickups : public AMM_Interactables_Base
{
	GENERATED_BODY()
public:
		AMM_Pickups();
	virtual void BeginPlay() override;

	virtual void PUInteract_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
		void PUDiscard();
	virtual void PUDiscard_Implementation();

	UPROPERTY(EditAnywhere)
		UTexture2D* Pickup_Thumbnail;

	UPROPERTY(EditAnywhere)
		FString Pickup_Name;

	UPROPERTY(EditAnywhere)
		int32 Pickup_Value;

	UPROPERTY(EditAnywhere)
		int32 Pickup_value_Null = 0;
	/*UFUNCTION()*/

	void OnPickedUp();

	//Blueprint Event: On pickedup.
	UFUNCTION(BlueprintImplementableEvent)
		void OnPickedupEvent();
		
};
