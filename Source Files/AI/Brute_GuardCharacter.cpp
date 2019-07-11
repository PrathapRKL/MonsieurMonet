// Fill out your copyright notice in the Description page of Project Settings.

#include "Brute_GuardCharacter.h"
#include "Regular_GuardCharacter.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Brute_GuardController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Hearing.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "MM_AlarmSystem.h"
#include "Interactables/MM_Env_Interactables.h"
#include "Runtime/Engine/Public/WorldCollision.h"

// Sets default values
ABrute_GuardCharacter::ABrute_GuardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Ignore this channelor it will absorb the trace impacts instead of the skeletal mesh.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(43.0f);
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;
	GetMovementComponent()->NavAgentProps.AgentRadius = 43.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 120.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	bUseControllerRotationYaw = false;
	MeleeDamage = 15.0f;

	TakedownArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TakedownTrigger"));
	TakedownArea->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ABrute_GuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Delegate functions for OnBegin and OnEnd overlaps of the box collision component.
	TakedownArea->OnComponentBeginOverlap.AddDynamic(this, &ABrute_GuardCharacter::OnBeginOverlap_TDB);
	TakedownArea->OnComponentEndOverlap.AddDynamic(this, &ABrute_GuardCharacter::OnEndOverlap_TDB);
}

// Called every frame
void ABrute_GuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABrute_GuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABrute_GuardCharacter::OnSeePlayer(APawn * Pawn)
{
}

void ABrute_GuardCharacter::OnHearPlayer(APawn * PawnInstigator, FVector Location, float Volume)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnInstigator = PlayerCharacter;
	OnHearPawn.Broadcast(PawnInstigator, PawnInstigator->GetActorLocation(), PlayerCharacter->NoiseLevel);
}

void ABrute_GuardCharacter::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void ABrute_GuardCharacter::PlayerDistance()
{
}

void ABrute_GuardCharacter::CautionState()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	CautionThreshold += 1;
}

void ABrute_GuardCharacter::PatrollingState()
{
	if (IsPatrolling == true)
	{
		ABrute_GuardController* BruteGuard_Controller = Cast<ABrute_GuardController>(GetController());
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(BruteGuard_Controller);
		const FVector SearchOrigin;
		const float SearchRadius = 300.0f;
		FNavLocation ResultLocation;
	}
}

void ABrute_GuardCharacter::LookingForPlayerState()
{
}

void ABrute_GuardCharacter::RestoringArtifactState()
{
}

void ABrute_GuardCharacter::FoundPlayerState()
{
	AlertGuardsState();
	GetCharacterMovement()->MaxWalkSpeed = 430.0f;
}

void ABrute_GuardCharacter::GuardingState()
{
}

void ABrute_GuardCharacter::AlertState()
{
	AlertThreshold += 1;
	AlertGuardsState();
}

void ABrute_GuardCharacter::NewPatrolState()
{
}

void ABrute_GuardCharacter::ScaredState()
{
}

void ABrute_GuardCharacter::AlertGuardsState()
{
}

void ABrute_GuardCharacter::StunnedState()
{
}

void ABrute_GuardCharacter::DistractedState(AActor * Actor)
{
	for (int i = 0; i <= AlarmSystems.Num(); i++)
	{
		Actor = AlarmSystems[i];
	}
}

void ABrute_GuardCharacter::LuredState()
{
}

void ABrute_GuardCharacter::OnBeginOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr && IsStunned == false)
	{
		PlayerCharacter->bTriggered = true;
		OnTakeDown.Broadcast(this);
	}
	else
	{
		return;
	}
}

void ABrute_GuardCharacter::OnEndOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
	{
		PlayerCharacter->CanTakeDown = false;
		PlayerCharacter->IsAttacking = false;
		PlayerCharacter->bTriggered = false;
	}
	else
	{
		return;
	}
}
