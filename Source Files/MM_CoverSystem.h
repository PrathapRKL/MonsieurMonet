// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_CoverSystem.generated.h"

class UBoxComponent;
class UArrowComponent;
class UBillboardComponent;

UCLASS()
class MONSIEURMONET_API AMM_CoverSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_CoverSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* CoverTriggerVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* CoverEndPoint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* CoverEndPoint2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* CoverMidPoint;

	//Collision boxes to black the player movement upon covering.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BlockingVol_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BlockingVol_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BlockingVolume_Front;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//On begin overlap delegate functions.
	UFUNCTION(BlueprintCallable)
		void OnCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//On end overlap delegate functions.
	UFUNCTION(BlueprintCallable)
		void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
