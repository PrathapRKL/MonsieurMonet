// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Swift_GuardCharacter.generated.h"

class AMM_PatrolPoints;
class AMM_AlarmSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDownSignature, AActor*, Guard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHearPawn, APawn*, PlayerPawn, FVector, Location, float, NoiseVolume);

UCLASS()
class MONSIEURMONET_API ASwift_GuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwift_GuardCharacter();

	UPROPERTY(BlueprintAssignable)
		FOnTakeDownSignature OnTakeDown;

	UPROPERTY(BlueprintAssignable)
		FOnHearPawn OnHearPawn;

	//Raytracing radius (To alert guards).
	UPROPERTY()
		float SweepRadius = 400.0f;

	//Character specific properties.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrozenTimer")
		float FrozenTime = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsFrozen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrozenTimer")
		bool IsStunned;

	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	/*float Distance;*/

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
		int AlertThreshold = 0;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool NewPatrolLoc;

	UPROPERTY(BlueprintReadWrite)
		bool FoundPlayer;

	bool IsAttacking;
	//Character specific properties.

	//Animation Montages to play when performing certain tasks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LookAround_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DisableInteractables_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Stunned_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Scared_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* KnockedOut_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Attack_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Confused_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Alert_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SearchingForPlayer_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AlertingGuards_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* TalkingOnRadio_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AlarmSystem_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* CheckingCameraStates_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StaringAtPaintings_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* NoticingArtifacts_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DisablingHidingLocations_Montage;
	//Animation Montages to play when performing certain tasks.

	UPROPERTY(EditAnywhere)
		class UBehaviorTree *BTTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* MeleeCollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMM_PatrolPoints*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> AlarmSystems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AlarmIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDistracted;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void OnHearPlayer(APawn* PawnInstigator, FVector Location, float Volume);

	UFUNCTION(BlueprintCallable)
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void Frozen();

	UFUNCTION(BlueprintCallable)
		void UnFreeze();

	UFUNCTION(BlueprintCallable)
		void PlayerDistance();


	//Character functions - based on the AI states.
	UFUNCTION(BlueprintCallable)
		void CautionState();

	UFUNCTION(BlueprintCallable)
		void PatrollingState();

	UFUNCTION(BlueprintCallable)
		void LookingForPlayerState();

	UFUNCTION(BlueprintCallable)
		void RestoringArtifactState();

	UFUNCTION(BlueprintCallable)
		void FoundPlayerState();

	UFUNCTION(BlueprintCallable)
		void GuardingState();

	UFUNCTION(BlueprintCallable)
		void AlertState();

	UFUNCTION(BlueprintCallable)
		void NewPatrolState();

	UFUNCTION(BlueprintCallable)
		void ScaredState();

	UFUNCTION(BlueprintCallable)
		void AlertGuardsState();

	UFUNCTION(BlueprintCallable)
		void StunnedState();

	UFUNCTION(BlueprintCallable)
		void DistractedState(AActor* Actor);

	UFUNCTION(BlueprintCallable)
		void LuredState();
	//Character functions - based on the AI states.


	//For player take down mechanics.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* TakedownArea;

	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnEndOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	/*UFUNCTION(BlueprintCallable)
		void TakenDown();*/

	UPROPERTY(EditAnywhere, BlueprintreadWrite)
		bool IsTakenDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TakendownMontage;

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<ASwift_GuardCharacter> SwiftGuard;

	//bool to use for the hear player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanHearPlayer = false;
};
