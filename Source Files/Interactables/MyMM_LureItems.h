// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/MM_LureItems_Base.h"
#include "MyMM_LureItems.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API AMyMM_LureItems : public AActor
{
	GENERATED_BODY()

public:
	AMyMM_LureItems();
	
	virtual void BeginPlay() override;

	void LThrow();

	UPROPERTY(EditAnywhere)
		UTexture2D* LureThumbnail;

	UPROPERTY(EditAnywhere)
		FString Lure_Name;

	UPROPERTY(EditAnywhere)
		float NoiseAmount;

	UFUNCTION(BlueprintCallable)
		void OnPickedUp();

	UPROPERTY()
		FVector TossVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* CollisionComponentL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* LureMesh;

	UPROPERTY(EditAnywhere)
		FString LureHelpText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* ProjectileMovement;
};
