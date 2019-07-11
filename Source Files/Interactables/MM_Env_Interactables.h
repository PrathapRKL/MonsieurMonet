// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/AkAudioEvent.h"
#include "MM_Env_Interactables.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, AActor*, OwnerActor);

UCLASS()
class MONSIEURMONET_API AMM_Env_Interactables : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_Env_Interactables();

	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteracted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* CollisionComponentE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* EnvMesh;

	UPROPERTY(EditAnywhere)
		FString EnvHelpText;

	UFUNCTION(BlueprintCallable)
		void OnInteract();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void OnInteractEvent();

	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap_Env();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAkAudioEvent* InteractSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAkComponent* InteractableEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NosieAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsMakingNoise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SearchRadius = 300.0f;
};
