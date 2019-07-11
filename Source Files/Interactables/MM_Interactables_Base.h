// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_Interactables_Base.generated.h"

UCLASS()
class MONSIEURMONET_API AMM_Interactables_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_Interactables_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
		void PUInteract();
	virtual void PUInteract_Implementation();

	/*UFUNCTION(BlueprintCallable)
		void PUDiscard();*/

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* InteractableMesh;

	UPROPERTY(EditAnywhere)
		FString InteractableHelpText;
};
