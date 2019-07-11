// This code belongs to Learning Machines.

#include "Swift_GuardCharacter.h"
#include "Brute_GuardCharacter.h"
#include "Regular_GuardCharacter.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MM_PatrolPoints.h"
#include "Swift_GuardController.h"
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
ASwift_GuardCharacter::ASwift_GuardCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Ignore this channelor it will absorb the trace impacts instead of the skeletal mesh.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(43.0f);
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;
	GetMovementComponent()->NavAgentProps.AgentRadius = 43.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	bUseControllerRotationYaw = false;
	MeleeDamage = 7.5f;

	TakedownArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TakedownTrigger"));
	TakedownArea->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ASwift_GuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (TakedownArea)
	{
		//Delegate functions for OnBegin and OnEnd overlaps of the box collision component.
		TakedownArea->OnComponentBeginOverlap.AddDynamic(this, &ASwift_GuardCharacter::OnBeginOverlap_TDB);
		TakedownArea->OnComponentEndOverlap.AddDynamic(this, &ASwift_GuardCharacter::OnEndOverlap_TDB);

		GTraceDelegate.BindUObject(this, &ASwift_GuardCharacter::OnTraceCompleted);
	}
}

// Called every frame.
void ASwift_GuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter->CanTakeDown == true)
		PlayerCharacter->TakeDown(this);*/
}

// Called to bind functionality to input.
void ASwift_GuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASwift_GuardCharacter::OnSeePlayer(APawn * Pawn)
{

}

void ASwift_GuardCharacter::OnHearPlayer(APawn* PawnInstigator, FVector Location, float Volume)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnInstigator = PlayerCharacter;
	OnHearPawn.Broadcast(PawnInstigator, PawnInstigator->GetActorLocation(), PlayerCharacter->NoiseLevel);
}

void ASwift_GuardCharacter::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void ASwift_GuardCharacter::Frozen()
{
	IsFrozen = true;
	GetCharacterMovement()->StopMovementImmediately();
	GetMesh()->bPauseAnims = true;
	/*CanSeePlayer = false;
	IsPatrolling = false;*/
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(FreezeTimerHandle, this, &ASwift_GuardCharacter::UnFreeze, FrozenTime);
}

void ASwift_GuardCharacter::UnFreeze()
{
	IsFrozen = false;
	GetMesh()->bPauseAnims = false;
}

void ASwift_GuardCharacter::PlayerDistance()
{
	if (IsAttacking == false && IsFrozen == false)
	{
		FVector SwiftGuard_Location = GetActorLocation();
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		FVector Player_Location = GetActorLocation();
		Distance = (SwiftGuard_Location - Player_Location).Size();
		if (Distance <= 1500)
		{

		}
	}
}

void ASwift_GuardCharacter::CautionState()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	CautionThreshold += 1;
}

void ASwift_GuardCharacter::PatrollingState()
{
	if (IsPatrolling == true)
	{
		ASwift_GuardController* SwiftGuard_Controller = Cast<ASwift_GuardController>(GetController());
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(SwiftGuard_Controller);
		const FVector SearchOrigin;
		const float SearchRadius = 300.0f;
		FNavLocation ResultLocation;
	}
	/*for (int i = 0; i <= PatrolPoints.Num(); i++)
	{
		SearchOrigin = PatrolPoints[i]->GetActorLocation();
	}
	if (NavigationSystem && NavigationSystem->GetRandomPointInNavigableRadius(SearchRadius, SearchOrigin, ResultLocation))
	{
		SwiftGuard_Controller->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>();
		SwiftGuard_Controller->BlackboardDataComp->GetValueAsObject();
	}*/
}

void ASwift_GuardCharacter::LookingForPlayerState()
{
}

void ASwift_GuardCharacter::RestoringArtifactState()
{
}

void ASwift_GuardCharacter::FoundPlayerState()
{
	/*World->GetTimerManager().SetTimer(FreezeTimerHandle, this, &ASwift_GuardCharacter::UnFreeze, FrozenTime);*/
	//GetWorld()->GetTimerManager().SetTimer(FreezeTimerHandle, this, &ASwift_GuardCharacter::AlertGuardsState());
	AlertGuardsState();
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//PlayAnimMontage(Attack_Montage, 1.0f, NAME_None);
}

void ASwift_GuardCharacter::GuardingState()
{
}

void ASwift_GuardCharacter::AlertState()
{
	AlertThreshold += 1;
	AlertGuardsState();
}

void ASwift_GuardCharacter::NewPatrolState()
{
}

void ASwift_GuardCharacter::ScaredState()
{
}

void ASwift_GuardCharacter::AlertGuardsState()
{
	SetWantsTrace();
	bWantsTrace = true;
	if (LastTraceHandle._Data.FrameNumber != 0)
	{
		FTraceDatum OutData;
		if (GetWorld()->QueryTraceData(LastTraceHandle, OutData))
		{
			//Clearing out handle.
			LastTraceHandle._Data.FrameNumber = 0;
			//Trace is now finished.
			GuardsHitResults(OutData);
		}
	}
	if (bWantsTrace)
	{
		LastTraceHandle = RequestTrace();
		bWantsTrace = false;
	}
}

void ASwift_GuardCharacter::StunnedState()
{
}

void ASwift_GuardCharacter::DistractedState(AActor* Actor)
{
	/*AlarmSystems = Cast<AMM_AlarmSystem>(Actor);*/
	for (int i = 0; i <= AlarmSystems.Num(); i++)
	{
		Actor = AlarmSystems[i];
	}/*
	bIsDistracted = true;*/
}

void ASwift_GuardCharacter::LuredState()
{
}

void ASwift_GuardCharacter::OnBeginOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr && IsStunned == false)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			PlayerCharacter->bTriggered = true;
			OnTakeDown.Broadcast(this);
		}
	}
	else
	{
		return;
	}
}

void ASwift_GuardCharacter::OnEndOverlap_TDB(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			PlayerCharacter->CanTakeDown = false;
			PlayerCharacter->IsAttacking = false;
			PlayerCharacter->bTriggered = false;
		}
	}
	else
	{
		return;
	}
}

FTraceHandle ASwift_GuardCharacter::RequestTrace()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return FTraceHandle();
	}

	auto Channel = UEngineTypes::ConvertToCollisionChannel(EnemyTraceType);
	FCollisionObjectQueryParams ObjectQueryParams(Channel);

	bool bTraceComplex = false;
	bool bIgnoreSelf = true;

	FVector StartLocation;
	FVector EndLocation;
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	ObjectQueryParams.AllDynamicObjects;
	FCollisionShape SCollisionShape = FCollisionShape::MakeSphere(SweepRadius);

	return World->AsyncSweepByObjectType(EAsyncTraceType::Single, StartLocation, EndLocation, ObjectQueryParams, SCollisionShape, QueryParams, &GTraceDelegate);
}

void ASwift_GuardCharacter::OnTraceCompleted(const FTraceHandle & Handle, FTraceDatum & Data)
{
	ensure(Handle == LastTraceHandle);
	GuardsHitResults(Data);
	LastTraceHandle._Data.FrameNumber = 0;
}

void ASwift_GuardCharacter::GuardsHitResults(const FTraceDatum & TraceData)
{
	TArray<FHitResult> Hits = TraceData.OutHits;
	//Parellel For loop for optimization purposes.
	FCriticalSection Mutex;
	ParallelFor(Hits.Num(), [&](int32 i)
	{
		//Cast to the regular and brute guards and call their respective alert functions.
		ARegular_GuardCharacter* RegularGuard_Character = Cast<ARegular_GuardCharacter>(Hits[i].GetActor());
		ABrute_GuardCharacter* BruteGuard_Character = Cast<ABrute_GuardCharacter>(Hits[i].GetActor());

		if (Hits[i].Actor == RegularGuard_Character)
		{
			RegularGuard_Character->LookForPlayerState();
		}
		else if (Hits[i].Actor == BruteGuard_Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("BruteGuardAlerted."));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Couldn't find anyone to alert."));
		}
		i++;
	});
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Couldn't find anyone to alert."));
}

void ASwift_GuardCharacter::SetWantsTrace()
{
	//Don't allow overlapping traces.
	if (!GetWorld()->IsTraceHandleValid(LastTraceHandle, false))
	{
		bWantsTrace = true;
	}
}


