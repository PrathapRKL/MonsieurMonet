// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Regular_GuardCharacter.generated.h"

class AMM_PatrolPoints;

UCLASS()
class MONSIEURMONET_API ARegular_GuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARegular_GuardCharacter();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPoints")
	//	//TSubclassOf<AMM_PatrolPoints> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrozenTimer")
		float FrozenTime = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsFrozen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrozenTimer")
		bool IsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* MeleeCollisionComp;

	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	FTimerHandle FreezeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintreadWrite)
		float MeleeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPawnSensing_Caution = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPawnSensing_Alert = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPawnSensing_Combat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanSeePlayer = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanStillSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CautionThreshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAlarmTripped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasLOS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsGuarding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPatrolling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FoundArtifact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RestoringArtifact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsCommunicating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAlerted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAlertingGuards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSearchingForPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsLookingForPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsScared;

	bool FoundPlayer;

	UPROPERTY(EditAnywhere)
		class UBehaviorTree *BTTree;

	UPROPERTY(VisibleAnywhere)
		class UPawnSensingComponent* PawnSensingComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void OnHearPlayer(APawn* PawnInstigator, const FVector &Location, float Volume);

	UFUNCTION(BlueprintCallable)
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void Frozen();

	UFUNCTION(BlueprintCallable)
		void UnFreeze();

	UFUNCTION(BlueprintCallable)
		void LookForPlayerState();
};
