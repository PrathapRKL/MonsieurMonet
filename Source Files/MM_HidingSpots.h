// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_HidingSpots.generated.h"

UCLASS()
class MONSIEURMONET_API AMM_HidingSpots : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_HidingSpots();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* HidingMesh;

	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnEndOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void OnInteract();

	UFUNCTION(BlueprintImplementableEvent)
		void OnInteractEvent();
};
