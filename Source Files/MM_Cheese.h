// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_Cheese.generated.h"

UCLASS()
class MONSIEURMONET_API AMM_Cheese : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_Cheese();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInteracted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void EatCheese();

	UFUNCTION(BlueprintCallable)
		void OnEatCheese();
};
