// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/AkAudioEvent.h"
#include "MM_LureItems_Base.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class AMonsieurMonet_Character;

UCLASS()
class MONSIEURMONET_API AMM_LureItems_Base : public AActor
{
	GENERATED_BODY()

		//Pickup on touch.
		virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;

	//Check if pawn can use this pickup.
	virtual bool CanBePickedUp(class AMonsieurMonet_Character* PlayerPawn) const;

public:	
	// Sets default values for this actor's properties
	AMM_LureItems_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//sound played on pickup.
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		UAkAudioEvent* PickedUpSound;

	//check to see if it is ready for interactions.
	UPROPERTY(Transient)
		uint32 bIsActive : 1;

	//The character to pick it up.
	UPROPERTY(Transient)
		AMonsieurMonet_Character* PickedUpBy;

	//Handle touches.
	void PickedUpOnTouch(class AMonsieurMonet_Character* Pawn);

	//on picked up.
	virtual void OnPickedUp();

	//Blueprint Event: On pickedup.
	UFUNCTION(BlueprintImplementableEvent)
		void OnPickedupEvent();

	UFUNCTION(BlueprintCallable)
		virtual void LThrow();

	//Create collision component for the actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* CollisionComponentL;

	//Craete static mesh for the actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LureItemMesh;

	UPROPERTY(EditAnywhere)
		FString LureHelpText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int ItemID;

	//Amount of noise produced.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NoiseImpact;

	UPROPERTY(EditDefaultsOnly)
		FString Name;

	UPROPERTY(EditDefaultsOnly)
		FString Action;

	////Lure item thumbnail.
	//UPROPERTY(EditAnywhere)
	//	UTexture2D* LureThumbnail;

	//ProjectileComponent.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent* ProjectileMovement;

	//Notify player of noise made.
	UFUNCTION(BlueprintCallable)
		void MakeNoiseEve(int32 NoiseAmount, APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		FString GetUseText() const { return FString::Printf(TEXT("%s : Press the Interact button to %s"), *Name, *Action); }

	//Called when the player interacts with the lure items.
	UFUNCTION() 
		void Interact(AMonsieurMonet_Character* PlayerCharacter);
};