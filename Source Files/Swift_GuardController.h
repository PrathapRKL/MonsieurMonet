// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"
#include "Swift_GuardController.generated.h"

class AMonsieurMonet_Character;
class AMM_PatrolPoints;
class UAIPerceptionComponent;

//AI States
UENUM(BlueprintType)
enum class AI_States_Swift : uint8
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
	NewPatrol_State,
	AlertingGuards_State,
	Observing_State,
	DisablingInteractables_State,
	Communicating_State,
	IntenseSearch_State,
	Lured_State,
	Distracted_State,
	Dummy_State
};

UCLASS()
class MONSIEURMONET_API ASwift_GuardController : public AAIController
{
	GENERATED_BODY()

public:
	//Constructor - Construction Script.
	ASwift_GuardController();

	//Blackboard and BTree Components.
	UPROPERTY()
		class UBlackboardComponent* BlackboardDataComp;

	UPROPERTY()
		class UBehaviorTreeComponent* BTTree_Component;

	UPROPERTY()
		class UAIPerceptionComponent* AIPerception;

	TSubclassOf<class UAISenseConfig>* AISenseConfig;

	class UAISenseConfig_Sight* AISenseSight;
	class UAISenseConfig_Damage* AISenseDamage;
	class UAISenseConfig_Hearing* AISenseHearing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> NavMesh;


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
		FName LureCount;
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
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName MemoryMarker;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName StunnedCount;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName Artifacts;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName LostAndFound;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName AlarmSystems;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName InteractableItems;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName HidingspotLocations;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName HidingSpots;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsAlarmSystemNearby;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsGuardNearby;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsAlone;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsLured;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName SuspicionCounter;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsCommunicating;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IsDistracted;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName IndexKey;
	UPROPERTY(EditDefaultsOnly, Category = "AI_Keys")
		FName AlarmIndex;

	class AMM_PatrolPoints* GetWaypoint();
	class AActor* GetAlarmSystems();

	int32 CurrentWayPoint = 0;
	/** Update direction AI is looking based on FocalPoint */
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;


	// Begin AController interface.
	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;
	virtual void BeginInactiveState() override;
	// End APlayerController interface.

	class AMonsieurMonet_Character* GetEnemy();
	void SetIdleState(AI_States_Swift Idle_State);
	void SetEnemy(class APawn* InPawn);
	void PlayerCaution(APawn* Pawn, AI_States_Swift Caution_State);
	void PlayerAlert(APawn* Pawn, AI_States_Swift Alert_State);
	void LookForPlayer(APawn* Pawn, AI_States_Swift LookingForPlayer_State);
	void SearchForPlayer(AActor* Pawn, AI_States_Swift SearchingForPlayer_State);
	void FoundPlayer(APawn* Pawn, AI_States_Swift FoundPlayer_State);
	void RestoringArtifact(APawn* Pawn, AI_States_Swift RestoringArtifact_State);
	void Scared(APawn* Pawn, AI_States_Swift Scared_State);
	void Lured(AI_States_Swift Lured_State);
	void Stunned(AI_States_Swift Stunned_State);
	void Communicating(AI_States_Swift Communicating_State);
	void IntenseSearch(APawn* Pawn, AI_States_Swift IntenseSearch_State);
	void DisablingInteractables(AActor* Actor, AI_States_Swift DisablingInteractables_State);
	void Observing(AActor* Actor, AI_States_Swift Observing_State);
	void AlertingOtherGuards(TArray<AActor*> Actors, AI_States_Swift AlertingGuards_State);
	void Distracted(AActor* Actors, AI_States_Swift Lured_State);

	void Patrolling(APawn* Pawn, AI_States_Swift Patrolling_State);
	void Guarding(APawn* Pawn, AI_States_Swift Guarding_State);
	void NewPatrol(APawn* Pawn, AI_States_Swift NewPatrol_State);

	void SetWaypoints(AMM_PatrolPoints* NewPatrolPoint, AI_States_Swift Patrolling_State);

	void SetMemoryMarkers(AActor* MemMarker);
	void GetMemoryMarkers();

public:
	/** Returns BlackboardComp subobject **/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardDataComp; }
	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BTTree_Component; }
};
