// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_HidingSpots.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;
class UCameraComponent;

UCLASS()
class MONSIEURMONET_API AMM_HidingSpots : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isCurrentlyHid = false;

	// Sets default values for this actor's properties
	AMM_HidingSpots();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTriggering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTriggering_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanHide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector InitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerHidingLocation;

	//The Blend camera properties to edit in the editor(engine).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Controls", Meta = (MakeEditWidget = true))
		FVector CameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Controls")
		FRotator CameraRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Controls")
		float CameraFOV = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Controls")
		float CameraMaxYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Controls")
		float CameraMaxPitch;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Setting up components for the Hiding spot actor to use.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* HidingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* HSCamera;


	//Delegate functions for the collision component's overlap begin and end.
	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnEndOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);


	//Functions to call upon the player interacting with the hiding spot actor.
	UFUNCTION(BlueprintCallable)
		void OnInteract();

	UFUNCTION(BlueprintImplementableEvent)
		void OnInteractEvent();

	UFUNCTION(BlueprintCallable)
		void HidePlayer();

	UFUNCTION(BlueprintCallable)
		void UnHidePlayer();

	//User Construction Script.
	virtual void OnConstruction(const FTransform& Transform) override;
};
