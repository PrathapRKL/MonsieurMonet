// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_CircuitBoxes_Camera.generated.h"

UCLASS()
class MONSIEURMONET_API AMM_CircuitBoxes_Camera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_CircuitBoxes_Camera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* CBMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Enabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Disabled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
