// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_CoverActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class MONSIEURMONET_API AMM_CoverActor : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION(BlueprintCallable)
		void OnCompEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Returns true if the socket is close to the player.
	bool IsClosetoPlayer(FName SocketName);

	//Returns the name of the closest socket.
	FName GetClosestSocket();

	//Determines the movement and facing directions of the player.
	void DetermineMovementDirection(FVector& MovementDirection, FRotator& FacingDirection);


public:	
	// Sets default values for this actor's properties
	AMM_CoverActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The Box collision component that informs the player if the player can take cover or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CoverMesh;

	//Retrieves movement direction and facing rotation of the player.
	UFUNCTION()
	void RetrieveDirectionAndRotation(FVector& MovementDirection, FRotator& FacingRotation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayerCoverEvent();
};
