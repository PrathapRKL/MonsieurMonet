// This code belongs to Learning Machines.
#include "Regular_GuardCharacter.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Regular_GuardController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"

// Sets default values
ARegular_GuardCharacter::ARegular_GuardCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComp->SensingInterval = 0.1f;
	PawnSensingComp->bOnlySensePlayers = false;
	PawnSensingComp->bEnableSensingUpdates = true;
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000.0f;
	PawnSensingComp->HearingThreshold = 800.0f;
	PawnSensingComp->LOSHearingThreshold = 900.0f;

	//Ignore this channelor it will absorb the trace impacts instead of the skeletal mesh.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(43.0f);
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;
	GetMovementComponent()->NavAgentProps.AgentRadius = 43.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 140.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	bUseControllerRotationYaw = false;
	MeleeDamage = 10.0f;
}

// Called when the game starts or when spawned
void ARegular_GuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ARegular_GuardCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ARegular_GuardCharacter::OnHearPlayer);
	}
}

// Called every frame
void ARegular_GuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FVector RegularGuardLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	Distance = (RegularGuardLocation - PlayerLocation).Size();
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(GetController());
	if (RegularGuard_Controller)
	{
		/*bSensedtarget = false;*/
		RegularGuard_Controller->SetEnemy(nullptr);
	}
}

void ARegular_GuardCharacter::OnSeePlayer(APawn * Pawn)
{
	if (CanSeePlayer == true)
	{
		/*LastSeenTime = GetWorld()->GetTimeSeconds();
		bSensedTarget = true;*/
		bPawnSensing_Combat = false;
		bPawnSensing_Alert = false;
		bPawnSensing_Caution = false;

		ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(GetController());
		AMonsieurMonet_Character* SensedPawn = Cast<AMonsieurMonet_Character>(Pawn);
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = SensedPawn->GetActorLocation();
		FHitResult HitResult;
		FCollisionQueryParams PlayerCQP;
		PlayerCQP.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn, PlayerCQP);
		if (RegularGuard_Controller && SensedPawn == HitResult.GetActor())
		{
			if (Distance < 1000)
			{
				CanSeePlayer = true;
				bPawnSensing_Caution = true;
				RegularGuard_Controller->SetEnemy(SensedPawn);
			}
			else
			{
				bPawnSensing_Caution = false;
				RegularGuard_Controller->SetEnemy(nullptr);
				if (PawnSensingComp->CouldSeePawn(SensedPawn, false))
				{
					bPawnSensing_Caution = false;
					CanSeePlayer = false;
				}
			}
		}
	}
}

void ARegular_GuardCharacter::OnHearPlayer(APawn * PawnInstigator, const FVector & Location, float Volume)
{
	ARegular_GuardController* RegularGuard_Controller = Cast<ARegular_GuardController>(GetController());
	if (RegularGuard_Controller)
	{
		RegularGuard_Controller->SetEnemy(PawnInstigator);
	}
}

void ARegular_GuardCharacter::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void ARegular_GuardCharacter::Frozen()
{
	IsFrozen = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetMesh()->bPauseAnims = true;
	CanSeePlayer = false;
	IsPatrolling = false;
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(FreezeTimerHandle, this, &ARegular_GuardCharacter::UnFreeze, FrozenTime);
}

void ARegular_GuardCharacter::UnFreeze()
{
	IsFrozen = false;
	CanSeePlayer = true;
	IsPatrolling = true;
	GetMesh()->bPauseAnims = false;
}

void ARegular_GuardCharacter::LookForPlayerState()
{
}



