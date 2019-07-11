// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Regular_GuardController.generated.h"

class AMonsieurMonet_Character;
class AMM_PatrolPoints;
class UAIPerceptionComponent;

UENUM(BlueprintType)
enum class AI_States : uint8
{
	Idle_State,
	Patrolling_State,
	LookingForPlayer_State,
	RestoringArtifact_State,
	Combat_State,
	SearchingForPlayer_State,
	Guarding_State,
	Scared_State,
	Caution_State,
	Alert_State,
	FoundPlayer_State,
	Stunned_State,
	NewPatrol_State
};


UCLASS()
class MONSIEURMONET_API ARegular_GuardController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY()
		class UBlackboardComponent* BlackboardDataComp;

	UPROPERTY()
		class UBehaviorTreeComponent* BTTree_Component;
	
	UPROPERTY()
		UAIPerceptionComponent* AIPerception;

	/*UPROPERTY()
		UAISenseConfig_Sight* AISense_Config;

	UPROPERTY()
		UAISenseConfig_Hearing* AIHearing_Config;*/

	ARegular_GuardController();

	virtual void Possess(APawn *InPawn)override;

	//Blackboard Keys.
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName TargetCharacter;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName AlarmLocations;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName ArtifactLocations;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName State;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName PlayerHidingLocation_approx;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName PlayerHidingLocation_exact;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName MemoryLocation;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName ArtifactCount;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName Player_LOS_Count;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsAlarmTripped;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsDetectedByCamera;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsDetectedByLaserwire;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsAllyHit;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsShot;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName CanSeePlayer;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName CanStillSeePlayer;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName HasLineofSight;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsGuarding;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName NewPatrolLocation;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName NewGuardLocation;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName Waypoints;

	void SetWaypoints(AMM_PatrolPoints* NewPatrolPoint, AI_States Patrolling_State);


	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardDataComp; }

	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BTTree_Component; }

	void SetPlayerCaught(APawn* Pawn, AI_States Combat_State);

	void PlayerCaution(APawn* Pawn, AI_States Caution_State);
	void PlayerAlert(APawn* Pawn, AI_States Alert_State);
	void LookForPlayer(APawn* Pawn, AI_States LookingForPlayer_State);
	void SearchForPlayer(APawn* Pawn, AI_States SearchingForPlayer_State);
	void FoundPlayer(APawn* Pawn, AI_States FoundPlayer_State);
	void RestoringArtifact(APawn* Pawn, AI_States RestoringArtifact_State);
	void Scared(APawn* Pawn, AI_States Scared_State);

	class AMM_PatrolPoints* GetWaypoint();

	int32 CurrentWayPoint = 0;

	void SetEnemy(class APawn* Pawn);
	class AMonsieurMonet_Character* GetEnemy();


	UFUNCTION(BlueprintCallable, Category = AI_Behavior)
		bool FindClosestEnemyWithLOS(AMonsieurMonet_Character* ExcludeEnemy);

	bool HasWeaponLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const;

	/** Update direction AI is looking based on FocalPoint */
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	bool LOSTrace(AMonsieurMonet_Character* InEnemyChar) const;


};
