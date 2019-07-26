// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MM_CheckpointSystem.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API UMM_CheckpointSystem : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 CurrentGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ASwift_GuardCharacter*> Guards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> GuardLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLoading;
};
