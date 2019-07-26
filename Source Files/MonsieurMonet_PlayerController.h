// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MonsieurMonet_PlayerController.generated.h"

/**
 *
 */
UCLASS()
class MONSIEURMONET_API AMonsieurMonet_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Crosshair_Offset = 0;

	UFUNCTION(BlueprintCallable)
		virtual void BeginPlay() override;

};
