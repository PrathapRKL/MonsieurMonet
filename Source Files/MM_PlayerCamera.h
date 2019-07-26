// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_PlayerCamera.generated.h"

class UCameraComponent;
class UBoxComponent;
class USceneComponent;

UCLASS()
class MONSIEURMONET_API AMM_PlayerCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_PlayerCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* PlayerContCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Delegate function for OnComponentBeginOverlap.
	UFUNCTION(Blueprintcallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);


	//Delegate function for OnComponentEndOverlap.
	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
