// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MM_LaserWires.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class USceneComponent;
class UMaterialInterface;
class UTimeline;

UCLASS()
class MONSIEURMONET_API AMM_LaserWires : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_LaserWires();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LaserStartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LaserEndReceiverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTrippedAlarm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* P_LaserWire;

	//Material interface for the particle system being created.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* ParticleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLaserWireEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTargetinLaser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SecuritySystemEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementPlayRate = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PauseDelay = 1.5f;

	//The location the laser wire moves along the X axis.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser End Pos", Meta = (MakeEditWidget = true))
		FVector LaserEndPosition; 

	//The location the laser wire moves along the Y axis.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser End Pos", Meta = (MakeEditWidget = true))
		FVector MovementEndLocation; 

	//The intial location of the laser wires.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MovementStartLocation; 

	UPROPERTY(EditAnywhere, BlueprintreadWrite)
		class AMM_AlarmSystem* AlarmSystems;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void UpdateBeamLocations();
	
	//Delegate function for OnComponentBeginOverlap of the boxcollision.
	UFUNCTION(BlueprintCallable)
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnOverlapBeginEvent();

	//Delegate function for OnComponentEndOverlap of the boxcollision.
	UFUNCTION(BlueprintCallable)
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnOverlapEndEvent();

	UFUNCTION(BlueprintCallable)
		void PlayLaserWireAnimation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void PlayLaserWireAnimationEvent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ReverseLaserWireAnimationEvent();

	UFUNCTION(BlueprintCallable)
		void ReverseLaserWireAnimation();

	UFUNCTION(BlueprintCallable)
		void ToggleLaserWire(bool bNewEnabled);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void StopTL();

	//User Construction Script.
	virtual void OnConstruction(const FTransform& Transform) override;


	//Timeline Component.
	//Timneline Component to Play the Laser trip wire animation.
	UPROPERTY()
	FTimeline LaserWireAnimation;

	//Deltatime(stepsize) for the timer calling the timeline tick.
	static const float DELTATIME;

	//Function which gets called from the Timer to call animTimeline.
	UFUNCTION()
	void TickTimeline(float Value);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	//Curve float for the timeline component.
	UPROPERTY()
		UCurveFloat* AnimCurve;

	UPROPERTY()
		FTimerHandle TimelineFinishedHandle;

	//Called when the timeline finishes playing.
	UFUNCTION()
		void PlayAnimation();

	FOnTimelineEventStatic FinishAnimFN{};
};
