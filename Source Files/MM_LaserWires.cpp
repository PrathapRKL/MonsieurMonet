// This code belongs to Learning Machines.

#include "MM_LaserWires.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"
#include "Components/SceneComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Materials/MaterialInterface.h"
#include "MonsieurMonetGameModeBase.h"
#include "ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include<thread>

// Sets default values
AMM_LaserWires::AMM_LaserWires()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SceneComponent and setting it up as the new root component.
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = Scene;

	//Setting up the base meshes for the laser wires.
	LaserStartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserBaseMesh"));
	LaserStartMesh->SetupAttachment(Scene);

	//Particle emitters for the laser wires.
	P_LaserWire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_LaserWire"));
	P_LaserWire->SetupAttachment(Scene);
	P_LaserWire->SetMaterial(0, ParticleMaterial);
	P_LaserWire->bAutoActivate = true;

	//Setting up collisionComponents for the laser wires and setting up delegates for overlapping functions.
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComp->SetupAttachment(Scene);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMM_LaserWires::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMM_LaserWires::OnOverlapEnd);

	//Setting up the end receiver meshes for the laser wires.
	LaserEndReceiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserEndReceiverMesh"));
	LaserEndReceiverMesh->SetupAttachment(Scene);

	//Creating a timeline component.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("CurveFloat'/Game/Curves/C_Animation_LW.C_Animation_LW'"));
	if (Curve.Object)
	{
		AnimCurve = Curve.Object;
	}

	/*LaserWireAnimation = NewObject<UTimelineComponent>(this, FName("LaserWireAnimation"));*/
	LaserWireAnimation = FTimeline{};
	FOnTimelineFloat PlayAnimFN{};
	PlayAnimFN.BindUFunction(this, "TickTimeline");
	FinishAnimFN.BindUFunction(this, "PlayAnimation");

	LaserWireAnimation.AddInterpFloat(Curve.Object, PlayAnimFN, FName{ TEXT("LaserWireMovement") });
}

// Called when the game starts or when spawned
void AMM_LaserWires::BeginPlay()
{
	Super::BeginPlay();
	bLaserWireEnabled = true;
	ToggleLaserWire(bLaserWireEnabled);
	UpdateBeamLocations();

	//Initialalizing Timeline Components.
	FTimerHandle TimelineHandle;/*
	this->BlueprintCreatedComponents.Add(LaserWireAnimation);*/
	LaserWireAnimation.SetPropertySetObject(this);
	LaserWireAnimation.SetDirectionPropertyName(FName("TimelineDirection"));
	LaserWireAnimation.SetLooping(false);
	LaserWireAnimation.SetTimelineLength(1.0f);
	LaserWireAnimation.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	LaserWireAnimation.SetPlaybackPosition(0.0f, true, true);
	LaserWireAnimation.SetPlayRate(0.3f);
	LaserWireAnimation.SetTimelineFinishedFunc(FinishAnimFN);
	/*GetWorld()->GetTimerManager().SetTimer(TimelineHandle, this, &AMM_LaserWires::TickTimeline, DELTATIME, true, 0.0f);*/
}

// Called every frame
void AMM_LaserWires::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LaserWireAnimation.TickTimeline(DeltaTime);
}

void AMM_LaserWires::UpdateBeamLocations()
{
	//Edit this to fix the lasertripwire offset.
	P_LaserWire->SetBeamSourcePoint(0, LaserStartMesh->GetComponentLocation(), 0);
	P_LaserWire->SetBeamTargetPoint(0, (LaserStartMesh->GetComponentLocation() + (GetActorRotation().Vector()) * LaserEndPosition.X), 0);
}

void AMM_LaserWires::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bLaserWireEnabled == true)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
		if (PlayerCharacter == nullptr)
		{
			return;
		}
		if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
		{
			if (OtherComp == PlayerCharacter->GetCapsuleComponent())
			{
				bTargetinLaser = true;
				bTrippedAlarm = true;
				OnOverlapBeginEvent();
				UGameplayStatics::ApplyDamage(PlayerCharacter, 0.3, nullptr, this, NULL);
				PlayerCharacter->Health -= 0.3f;
			}
		}
	}
}

void AMM_LaserWires::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (bLaserWireEnabled == true)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
		if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
		{
			bTargetinLaser = false;
			bTrippedAlarm = false;
			OnOverlapEndEvent();
		}
	}
}

void AMM_LaserWires::PlayLaserWireAnimation()
{
	/*LaserWireAnimation.SetNewTime(0.0f);*/
	LaserWireAnimation.Play();
	PlayLaserWireAnimationEvent();
}

void AMM_LaserWires::ReverseLaserWireAnimation()
{
	/*LaserWireAnimation.SetNewTime(1.0f);*/
	LaserWireAnimation.Reverse();
	ReverseLaserWireAnimationEvent();
}

void AMM_LaserWires::ToggleLaserWire(bool bNewEnabled)
{
	/*std::thread t1(ToggleLaserWire, std::ref(bool), )*/
	if (bNewEnabled == true)
	{
		bLaserWireEnabled = true;
		P_LaserWire->SetVisibility(true, false);
		UpdateBeamLocations();
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		if (bShouldMove == true)
			PlayLaserWireAnimation();
	}
	else
	{
		bLaserWireEnabled = false;
		P_LaserWire->SetVisibility(false, false);
		UpdateBeamLocations();
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (bShouldMove == false)
			StopTL();
		LaserWireAnimation.Stop();
	}
}

void AMM_LaserWires::OnConstruction(const FTransform & Transform)
{
	//User Construction Script Properties.
	//Finding the Laser end position.
	LaserEndPosition = FVector(LaserEndPosition.X, 0.0f, 0.0f);
	LaserEndPosition = FVector(LaserEndPosition.X, LaserStartMesh->RelativeLocation.Y, LaserStartMesh->RelativeLocation.Z);

	//Setting up Laser collions.
	FVector2D VecStruct = (FVector2D(LaserStartMesh->RelativeLocation) + FVector2D(LaserEndPosition));
	FVector ToVector = (FVector(VecStruct, 0.0f));
	FVector MakeVectorAxis = FVector((ToVector / 2.0f).Size(), 0.0f, LaserStartMesh->RelativeLocation.Z + 5);
	CollisionComp->SetRelativeLocation(MakeVectorAxis.RotateAngleAxis(0.0f, FVector(0.0f, 0.0f, 1.0f)), false, nullptr, ETeleportType::None);
	CollisionComp->SetBoxExtent(FVector((ToVector / 2.0f).Size(), 5.0f, 5.0f), true);

	//Set Mesh Location.
	LaserEndReceiverMesh->SetWorldLocation(LaserStartMesh->GetComponentLocation() + (GetActorRotation().Vector() * LaserEndPosition.X), false, nullptr, ETeleportType::None);

	MovementStartLocation = Scene->GetComponentLocation();
	UpdateBeamLocations();
}

void AMM_LaserWires::TickTimeline(float Value)
{
	FVector LerpedValue = FMath::Lerp(MovementStartLocation, MovementStartLocation + (GetActorRotation().RotateVector(MovementEndLocation)), Value);
	Scene->SetWorldLocation(LerpedValue, false, nullptr, ETeleportType::None);
	UpdateBeamLocations();
}

void AMM_LaserWires::PlayAnimation()
{
	GetWorld()->GetTimerManager().SetTimer(TimelineFinishedHandle, PauseDelay, false, PauseDelay);
	switch (TimelineDirection)
	{
		case ETimelineDirection::Forward:
		{
			LaserWireAnimation.ReverseFromEnd();
			ReverseLaserWireAnimation();
		}
		case ETimelineDirection::Backward:
		{
			PlayLaserWireAnimation();
		}
	}
}

