// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsieurMonet_Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Interactables/MM_Interactables_Base.h"
#include "Interactables/MM_Pickups.h"
#include "MonsieurMonetGameModeBase.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include "WidgetBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactables/MM_CoverActor.h"
#include "MonsieurMonet_PlayerController.h"
#include "Animation/AnimMontage.h"
#include "MM_FreezeProjectile.h"
#include "TimerManager.h"
#include "Perception/PawnSensingComponent.h"
#include "AI/Regular_GuardController.h"
#include "AI/Regular_GuardCharacter.h"
#include "Perception/AISense.h"
#include "Perception/AISense_Hearing.h"
#include "Engine/DataTable.h"
#include "Engine.h"
#include "AI/Brute_GuardCharacter.h"
#include "AI/Brute_GuardController.h"
#include "AI/Swift_GuardCharacter.h"
#include "AI/Swift_GuardController.h"
#include "AI/Regular_GuardCharacter.h"
#include "AI/Regular_GuardController.h"
#include "Interactables/MM_LureItems_Base.h"
#include "Interactables/MM_Pickups.h"
#include "ProceduralMeshComponent.h"
#include "Interactables/MM_CircuitBoxes_Camera.h"
#include "MM_CoverSystem.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

//ENGINE_API void AMonsieurMonet_Character::AddDia(FName DiaName, const FTableDiaBase & DiaData)
//{
//	DATATABLE_CHANGE_SCOPE();
//
//	UScriptStruct& EmptyUsingStruct = UDataTable::GetEm
//}

// Sets default values
AMonsieurMonet_Character::AMonsieurMonet_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 85.0f);
	TurnRate = 55.0f;
	LookUpRate = 55.0f;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 260.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 280.0f;
	NoiseLevel = 700.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm_CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 340.0f;
	CameraBoom->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom);
	PlayerCamera->bUsePawnControlRotation = false;
	PostProcessSettings.bOverride_ColorGamma = true;
	PlayerCamera->PostProcessSettings = PostProcessSettings;

	FreezeGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FreezeGun"));
	FreezeGun->SetupAttachment(CameraBoom);

	SearchRange = CreateDefaultSubobject<USphereComponent>(TEXT("SearchRange_Interactables"));
	SearchRange->SetupAttachment(CameraBoom);
	SearchRange->InitSphereRadius(100.0f);
	SearchRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Gun_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile_Sphere"));
	Gun_Sphere->SetupAttachment(FreezeGun);
	Gun_Sphere->SetWorldScale3D(FVector(0.12f, 0.12f, 0.12f));

	//Collision Component for the noise radius to search for and notify guards;
	NoiseRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("NoiseRadiusComp"));
	NoiseRadiusComp->SetupAttachment(RootComponent);
	NoiseRadius = 250.0f;
	NoiseRadiusComp->SetSphereRadius(NoiseRadius);

	//Enable crouch functionality.
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	//Timeline Component.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curves/AimAndCrouchCameras/Aim_Camera_float"));
	check(Curve.Succeeded());
	Aim_FloatCurve = Curve.Object;

	TakedownCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TakedownCapsule"));
	TakedownCapsule->SetupAttachment(GetMesh());
	/*TakedownCapsule->SetGenerateOverlapEvents(true);*/
	/*TakedownCapsule->AttachTo(GetMesh(), "RightHand", EAttachLocation::KeepRelativeOffset, true);*/

	// Create the line of sight mesh
	LOSMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineOfSightMesh"));
	LOSMesh->bUseAsyncCooking = true;
	LOSMesh->ContainsPhysicsTriMeshData(false);
	//LOSMesh->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	LOSMesh->SetupAttachment(GetCapsuleComponent());


	//Timeline Component for the crouch camera transition.
	//Creating a float curve for the crouch camera timeline.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CrouchCurve(TEXT("CurveFloat'/Game/Curves/Crouch_Camera_float.Crouch_Camera_float'"));
	if (CrouchCurve.Object)
	{
		CrouchCamCurve = CrouchCurve.Object;
	}

	CrouchCameraAnimation = FTimeline{};
	FOnTimelineFloat PlayCrouchAnim{};
	PlayCrouchAnim.BindUFunction(this, "TickCrouchTimeline");
	FOnTimelineEventStatic FinishedCrouchAnim{};
	FinishedCrouchAnim.BindUFunction(this, "PlayCrouchCameraAnim");
	CrouchCameraAnimation.AddInterpFloat(CrouchCurve.Object, PlayCrouchAnim, FName{ TEXT("CrouchCameraTransition") });


	//Timeline Component for the dynamic sprint camera transition.
	//Creating a float curve for the sprint camera timeline.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> SprintCurve(TEXT("CurveFloat'/Game/Curves/Sprint_Camera_float.Sprint_Camera_float'"));
	if (SprintCurve.Object)
	{
		SprintCamCurve = SprintCurve.Object;
	}

	SprintCameraAnimation = FTimeline{};
	FOnTimelineFloat PlaySprintAnim{};
	PlaySprintAnim.BindUFunction(this, "TickSprintTimeline");
	FOnTimelineEventStatic FinishedSprintAnim{};
	FinishedSprintAnim.BindUFunction(this, "PlaySprintCameraAnim");
	SprintCameraAnimation.AddInterpFloat(SprintCurve.Object, PlaySprintAnim, FName{ TEXT("SprintCameraTransition") });

	//Timeline Component for the dynamic crouch walk camera transition.
	//Creating a float curve for the crouch camera timeline.
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CWCurve(TEXT("CurveFloat'/Game/Curves/CrouchWalk_Camera_float.CrouchWalk_Camera_float'"));
	if (CWCurve.Object)
	{
		CWCamCurve = CWCurve.Object;
	}

	CrouchWalkTL = FTimeline{};
	FOnTimelineFloat PlayCWAnimation{};
	PlayCWAnimation.BindUFunction(this, "TickCWTimeline");
	FOnTimelineEventStatic FinishedCWAnim{};
	FinishedCWAnim.BindUFunction(this, "PlayCWCameraAnim");
	CrouchWalkTL.AddInterpFloat(CWCurve.Object, PlayCWAnimation, FName{ TEXT("CrouchWalkCameraTransition") });
}

void AMonsieurMonet_Character::InitLOSMesh()
{
	int NumVertices = FMath::RoundToInt(ArcAngle / AngleStep) + 2;
	LOSVertices.Init(FVector::ZeroVector, NumVertices);


	int NumTriangles = (ArcAngle == 360) ? ((NumVertices - 1) * 3) : ((NumVertices - 2) * 3);
	LOSTriangles.Init(0, NumTriangles);

	FVector LineStartLocation = GetActorLocation();
	FVector CurrentActorForward = GetActorForwardVector();
	float MinAngle = -ArcAngle / 2;
	float MaxAngle = ArcAngle / 2;
	int VertexIndex = 1;
	for (float CurrentAngle = MinAngle;
		CurrentAngle <= MaxAngle;
		CurrentAngle += AngleStep)
	{
		FVector CurrentAngleDirection = CurrentActorForward.RotateAngleAxis(CurrentAngle, FVector(0, 0, 1));
		FVector LineEndLocation = LineStartLocation + CurrentAngleDirection * Radius;

		FVector HitResultInCharacterLocalSpace = GetActorTransform().InverseTransformPosition(LineEndLocation);
		LOSVertices[VertexIndex] = HitResultInCharacterLocalSpace;
		VertexIndex++;
	}

	VertexIndex = 0;
	for (int Triangle = 0; Triangle < LOSTriangles.Num(); Triangle += 3)
	{
		LOSTriangles[Triangle] = 0;
		LOSTriangles[Triangle + 1] = VertexIndex + 1;

		if (Triangle == (NumVertices - 2) * 3) // the arc has 360 angle, or in other words, it's a circle
		{
			LOSTriangles[Triangle + 2] = 1;
		}
		else
		{
			LOSTriangles[Triangle + 1] = VertexIndex + 2;
		}

		VertexIndex++;
	}

	UpdateLOSMeshData(LOSVertices, LOSTriangles);

	LOSMesh->SetRelativeLocation(FVector(0.0, 0.0, LOSHeight));
	LOSMesh->SetMaterial(0, LOSMaterial);
}

void AMonsieurMonet_Character::TickLOSMesh()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	const FName TraceTag("LoSTraceTag");
	FCollisionQueryParams TraceParams = FCollisionQueryParams(TraceTag, false, this);
	//TraceParams.AddIgnoredComponents();

	FVector LineStartLocation = GetActorLocation();
	FVector CurrentActorForward = GetActorForwardVector();

	float MinAngle = -ArcAngle / 2;
	float MaxAngle = ArcAngle / 2;
	int VertexIndex = 1;
	for (float CurrentAngle = MaxAngle;
		CurrentAngle >= MinAngle;
		CurrentAngle -= AngleStep)
	{
		FVector CurrentAngleDirection = CurrentActorForward.RotateAngleAxis(CurrentAngle, FVector(0, 0, 1));
		FVector LineEndLocation = LineStartLocation + CurrentAngleDirection * Radius;

		FHitResult HitResult;
		FVector HitPoint;

		// In DefaultEngine.ini: Channel=ECC_GameTraceChannel1,Name="Obstacle"
		bool bHit = World->LineTraceSingleByChannel(HitResult, LineStartLocation, LineEndLocation, ECollisionChannel::ECC_GameTraceChannel1, TraceParams, FCollisionResponseParams());
		if (bHit)
		{
			HitPoint = HitResult.ImpactPoint;
		}
		else
		{
			HitPoint = LineEndLocation;
		}

		FVector HitResultInCharacterLocalSpace = GetActorTransform().InverseTransformPosition(HitPoint);
		LOSVertices[VertexIndex] = HitResultInCharacterLocalSpace;
		VertexIndex++;
	}

	VertexIndex = 0;
	int NumVertices = LOSVertices.Num();
	for (int Triangle = 0; Triangle < LOSTriangles.Num(); Triangle += 3)
	{
		LOSTriangles[Triangle] = 0;
		LOSTriangles[Triangle + 1] = VertexIndex + 1;

		if (Triangle == (NumVertices - 2) * 3) // the arc has 360 angle, or in other words, it's a circle
		{
			LOSTriangles[Triangle + 2] = 1;
		}
		else
		{
			LOSTriangles[Triangle + 2] = VertexIndex + 2;
		}

		VertexIndex++;
	}

	UpdateLOSMeshData(LOSVertices, LOSTriangles);
}


void AMonsieurMonet_Character::UpdateLOSMeshData(const TArray<FVector>& Vertices, const TArray<int32>& Triangles)
{
	TArray<FVector> TempNormals;
	TArray<FVector2D> TempUV0;
	TArray<FProcMeshTangent> TempTangents;
	TArray<FLinearColor> TempVertexColors;
	LOSMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TempNormals, TempUV0, TempVertexColors, TempTangents, false);
	LOSMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AMonsieurMonet_Character::BeginPlay()
{
	//Timeline properties for the Aim_CameraTimeline and Crouch_Cameratimeline.
	FOnTimelineFloat onTimelineCallback_Aim;
	FOnTimelineFloat onTimelineCallback_Crouch;
	FOnTimelineEventStatic onTimelineFinishedCallback_Aim;
	FOnTimelineEventStatic onTimelineFinishedCallback_Crouch;

	Super::BeginPlay();

	//Binding Noise Overlap delegates.
	NoiseRadiusComp->OnComponentBeginOverlap.AddDynamic(this, &AMonsieurMonet_Character::OnNoiseBeginOverlap);
	NoiseRadiusComp->OnComponentEndOverlap.AddDynamic(this, &AMonsieurMonet_Character::OnNoiseEndOverlap);

	/*LimitCamera();*/

	//Inventory Slots.
	Inventory.SetNum(4);

	//Current interactable item (Artifact).
	CurrentInteractable = nullptr;
	PickupItemss = nullptr;

	//Nearest to pick up
	NearestLureItem = nullptr;


	//Current lure item (Lure Item).
	CurrentLureItem = "Coins";

	if (Aim_FloatCurve != NULL)
	{
		Aim_CameraTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation_Aim"));
		Aim_CameraTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(Aim_CameraTimeline);
		Aim_CameraTimeline->SetNetAddressable();
		Aim_CameraTimeline->SetPropertySetObject(this);
		//Aim_CameraTimeline->SetDirectionPropertyName(FName("TimelineDirection_Aim"));
		Aim_CameraTimeline->SetLooping(false);
		Aim_CameraTimeline->SetTimelineLength(2.0f);
		Aim_CameraTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		Aim_CameraTimeline->SetPlaybackPosition(0.0f, true);
		onTimelineCallback_Aim.BindUFunction(this, FName{ TEXT("Aim_TimelineCallback") });
		onTimelineFinishedCallback_Aim.BindUFunction(this, FName{ TEXT("Aim_TimelineFinishedCallback") });
		Aim_CameraTimeline->RegisterComponent();
	}

	//Delegates for the takedown capsule of the player character.
	if (TakedownCapsule)
	{
		TakedownCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMonsieurMonet_Character::OnBeginOverlap);
		TakedownCapsule->OnComponentEndOverlap.AddDynamic(this, &AMonsieurMonet_Character::OnEndOverlap);
	}

	//Initializing Timeline components for the crouch camera transition/Animation.
	CrouchCameraAnimation.SetPropertySetObject(this);
	CrouchCameraAnimation.SetDirectionPropertyName(FName("CrouchCamTimelineDir"));
	CrouchCameraAnimation.SetLooping(false);
	CrouchCameraAnimation.SetTimelineLength(1.0f);
	CrouchCameraAnimation.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	CrouchCameraAnimation.SetPlaybackPosition(0.0f, true, true);
	CrouchCameraAnimation.SetPlayRate(2.0f);

	//Initializing Timeline components for the dynamic sprint camera transition/Animation.
	SprintCameraAnimation.SetPropertySetObject(this);
	SprintCameraAnimation.SetDirectionPropertyName(FName("TlineFinishHandle_Sprint"));
	SprintCameraAnimation.SetLooping(false);
	SprintCameraAnimation.SetTimelineLength(1.0f);
	SprintCameraAnimation.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	SprintCameraAnimation.SetPlaybackPosition(0.0f, true, true);
	SprintCameraAnimation.SetPlayRate(1.3f);

	//Initializing Timeline components for the dynamic crouch walk camera transition/Animation.
	CrouchWalkTL.SetPropertySetObject(this);
	CrouchWalkTL.SetDirectionPropertyName(FName("CWCamTimelineDir"));
	CrouchWalkTL.SetLooping(false);
	CrouchWalkTL.SetTimelineLength(1.0f);
	CrouchWalkTL.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	CrouchWalkTL.SetPlaybackPosition(0.0f, true, true);
	CrouchWalkTL.SetPlayRate(1.3f);

	//LOS for minimap.
	if (IsLOSOn)
	{
		ArcAngle = 360.0F;
		AngleStep = 1.0F;
		Radius = 2000.0F;

		InitLOSMesh();
	}

	LOSMesh->bOwnerNoSee = true;

	if (bIsAiming)
	{


	}

	//Set Timer by Function name alternative (To notify the enemy pawn of the noise produced by the player character).
	GetWorld()->GetTimerManager().SetTimer(NotifyHandle, this, &AMonsieurMonet_Character::NotifyPawnNoise, 0.3f, true, 0.3f);
}

// Called every frame
void AMonsieurMonet_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*CheckForInteractables();*/
	CrouchRate = FMath::FInterpConstantTo(50.0f, 85.0f, 100.0f, 10.0f);
	/*CrouchCamera_F = FMath::FInterpTo(0.0f, -40.0f, 3.0f, 0.3f);
	CrouchCamera_R = FMath::FInterpTo(-40.0f, 0.0f, 3.0f, 0.3f);*/

	if (Aim_CameraTimeline != NULL)
	{
		Aim_CameraTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	if (IsLOSOn)
	{
		TickLOSMesh();
	}

	UpdateStamina();

	//Tick crouch timeline.
	CrouchCameraAnimation.TickTimeline(DeltaTime);
	//Tick Sprint Camera timeline.
	SprintCameraAnimation.TickTimeline(DeltaTime);
	//Tick CW camera timeline.
	CrouchWalkTL.TickTimeline(DeltaTime);

	if (CurrentStamina <= 0.2f && SprintCameraAnimation.IsReversing() == false)
	{
		SprintCameraAnimation.Reverse();
		StaminaIsLow = false;
	}
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf<bool>(bCanTakeCover));*/

	if (GetVelocity().Size() > 0 && GetCharacterMovement()->IsCrouching() == true)
		CrouchWalkTL.Play();
	else
		CrouchWalkTL.Reverse();
}


void AMonsieurMonet_Character::Aim_TimelineCallback(float Value)
{
	//Aim_LerpedLoc = FMath::Lerp(Aim_CamLoc_A, Aim_CamLoc_B, Value);
	PlayerCamera->SetFieldOfView(Value);
}

void AMonsieurMonet_Character::Aim_TimelineFinishedCallback()
{
	//Aim_CameraTimeline->Stop();
}

void AMonsieurMonet_Character::Aim_PlayTimeline()
{
	if (Aim_CameraTimeline != NULL)
	{
		Aim_CameraTimeline->Play();
	}
	Aim_CameraTimeline->Reverse();
}

void AMonsieurMonet_Character::Aim_ReverseTimeline()
{
	if (Aim_CameraTimeline != NULL)
	{
		Aim_CameraTimeline->Reverse();
	}
	Aim_CameraTimeline->Play();
}


// Called to bind functionality to input
void AMonsieurMonet_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMonsieurMonet_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMonsieurMonet_Character::MoveRight);
	PlayerInputComponent->BindAxis("YawInput", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PitchInput", this, &ACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpGP", this, &AMonsieurMonet_Character::ControllerPitch);
	PlayerInputComponent->BindAxis("TurnGP", this, &AMonsieurMonet_Character::ControllerYaw);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMonsieurMonet_Character::Interact);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMonsieurMonet_Character::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMonsieurMonet_Character::Sprint_Stop);
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMonsieurMonet_Character::ToggleInventory);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMonsieurMonet_Character::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMonsieurMonet_Character::AimStop);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMonsieurMonet_Character::Shoot);
	PlayerInputComponent->BindAction("CoverFN", IE_Pressed, this, &AMonsieurMonet_Character::ToggleCover);

	//this is for selecting the current lure item
	PlayerInputComponent->BindAction("SelectCoin", IE_Pressed, this, &AMonsieurMonet_Character::SelectCoin);
	PlayerInputComponent->BindAction("SelectPeel", IE_Pressed, this, &AMonsieurMonet_Character::SelectPeel);
	PlayerInputComponent->BindAction("SelectHealthRegen", IE_Pressed, this, &AMonsieurMonet_Character::SelectHealthRegen);
	PlayerInputComponent->BindAction("SelectBomb", IE_Pressed, this, &AMonsieurMonet_Character::SelectBomb);


	FInputActionBinding& Toggle_PauseGame = PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &AMonsieurMonet_Character::PauseGame);
	Toggle_PauseGame.bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMonsieurMonet_Character::PlayerCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMonsieurMonet_Character::PlayerCrouch_Stop);
}

void AMonsieurMonet_Character::MoveForward(float Value)
{
	if (Controller != NULL && Value != 0.0f && !bIsInCover && IsHiding == false)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		if (Value == 0.0f)
		{
			NoiseLevel = 0.0f;
		}
	}
}

void AMonsieurMonet_Character::MoveRight(float Value)
{
	CoverValue = Value;

	if (Controller != NULL && Value != 0.0f && IsHiding == false)
	{
		if (!bIsInCover)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
		else
		{
			/*if (GetActorLocation().Y != (CoverActors->CoverEndPoint1->RelativeLocation.Y) && GetActorLocation().Y != (CoverActors->CoverEndPoint2->RelativeLocation.Y))
			{*/
			float Dir_Y = CoverActors->CoverTriggerVolume->GetScaledBoxExtent().Y;
			//AddMovementInput(CoverMovementDirection, Value);
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
			/*}*/
			/*else*/
			//{
			//	FMath::Clamp(Value, 0.0f, 1.0f);
			//	/*GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Clamping Works"));*/
			//}
		}
		if (Value == 0.0f)
		{
			NoiseLevel = 0.0f;
		}
	}
}

void AMonsieurMonet_Character::ControllerPitch(float Pitch)
{
	float BaseLookUpRate = 70.0f;
	float Value = Pitch * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(Value);
}

void AMonsieurMonet_Character::ControllerYaw(float Yaw)
{
	float BaseTurnrate = 70.0f;
	float Value = Yaw * BaseTurnrate * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(Value);
}

void AMonsieurMonet_Character::Sprint()
{
	if (GetCharacterMovement()->IsCrouching() == false && CanSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = 520.0f;
		if (GetVelocity().Size() > 0)
		{
			NoiseLevel = 1400.0f;
			Issprinting = true;
			SprintCameraAnimation.Play();
			NoiseRadius = 500.0f;
			NoiseRadiusComp->SetSphereRadius(NoiseRadius);
		}
		//CurrentStamina -= CurrentStamina*world->GetDeltaSeconds();
	}

}

void AMonsieurMonet_Character::Sprint_Stop()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (GetCharacterMovement()->IsCrouching() == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = 260.0f;
		NoiseLevel = 700.0f;
		Issprinting = false;
		SprintCameraAnimation.Reverse();
		NoiseRadius = 250.0f;
		NoiseRadiusComp->SetSphereRadius(NoiseRadius);
		//world->GetTimerManager().SetTimer(RecoverStaminaHandle, &AMonsieurMonet_Character::RecoverStamina(), 1.0f, false, 4.8f);
	}
}

void AMonsieurMonet_Character::UpdateStamina()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	//reduces sprint bar
	if (CurrentStamina > 0)
	{
		CanSprint = true;
	}

	if (Issprinting)
	{
		CurrentStamina -= StaminaConsumeRate;
	}

	if (CurrentStamina <= 0)
	{
		CanSprint = false;
		Sprint_Stop();
		StaminaIsLow = true;
	}

	//recovers sprint bar
	if (CurrentStamina < MaxStamina && !Issprinting)
	{
		CurrentStamina += StaminaRecoverRate;
	}
}

void AMonsieurMonet_Character::NotifyPawnNoise()
{
	for (AActor* Act : RegisteredGuards)
	{
		ASwift_GuardCharacter* SwiftGuardCharacter = Cast<ASwift_GuardCharacter>(Act);
		SwiftGuardCharacter->OnHearPlayer(this, GetActorLocation(), NoiseLevel);
		NotifyPawnNoise_Event();
	}
}

void AMonsieurMonet_Character::TickCrouchTimeline(float Value)
{
	float LerpedValue = FMath::Lerp(0.0f, -50.0f, Value);
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, LerpedValue), false, nullptr, ETeleportType::None);
}

void AMonsieurMonet_Character::PlayCrouchCameraAnim()
{
}

void AMonsieurMonet_Character::TickSprintTimeline(float Value)
{
	float LerpedValue = FMath::Lerp(340.0f, 820.0f, Value);
	CameraBoom->TargetArmLength = LerpedValue;
}

void AMonsieurMonet_Character::PlaySprintCameraAnim()
{
}

void AMonsieurMonet_Character::OnNoiseBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult & SweepResult)
{
	ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(OtherActor);
	if (OtherActor == SwiftGuard)
	{
		SwiftGuard->CanHearPlayer = true;
	}
}

void AMonsieurMonet_Character::OnNoiseEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(OtherActor);
	if (OtherActor == SwiftGuard)
	{
		SwiftGuard->CanHearPlayer = false;
	}
}

void AMonsieurMonet_Character::TickCWTimeline(float Value)
{
	float LLerpedValue = FMath::Lerp(340.0f, 850.0f, Value);
	float OLerpedValue = FMath::Lerp(0.0f, 200.0f, Value);
	CameraBoom->TargetArmLength = LLerpedValue;
	CameraBoom->TargetOffset = FVector(0.0f, 0.0f, OLerpedValue);
}

void AMonsieurMonet_Character::PlayCWCameraAnim()
{
}

void AMonsieurMonet_Character::Interact()
{
	UWorld* TheWorld = GetWorld();
	if (TheWorld != nullptr)
	{
		CheckForInteractables();
		if (CurrentInteractable != nullptr)
		{
			/*PlayAnimMontage(PickupMontage, 1.0f, NAME_None);*/
			//TheWorld->GetTimerManager().SetTimer(PickupHandle, &AMonsieurMonet_Character::CurrentInteractable->PUInteract_Implementation(), 1.0f, false, 4.8f);
			CurrentInteractable->PUInteract_Implementation();
		}
	}

	//Cover System.
	if (IsInteracting == false)
	{
		if (bCanTakeCover == true)
		{
			TArray<AActor*> Cover;
			GetOverlappingActors(Cover, CoverSystems);
			for (AActor* Act : Cover)
			{
				CoverActors = Cast<AMM_CoverSystem>(Act);
			}
			bIsInCover = true;
			if (CoverActors != nullptr)
			{
				//Set player's rotation.
				FQuat PlayerRotation = CoverActors->ArrowComponent->GetComponentQuat();
				SetActorRotation(PlayerRotation, ETeleportType::None);
				//Set collision responses of the volumes to block the player character so the player cannot move out of bounds of the cover trigger actor.
				CoverActors->BlockingVol_Left->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
				CoverActors->BlockingVol_Right->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
				CoverActors->BlockingVolume_Front->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
			}
		}
		else if (bCanTakeCover == false && CoverActors != nullptr)
		{
			bIsInCover = false;
			//Set the collision responses of the volumes back to default (Ignore).
			CoverActors->BlockingVol_Left->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
			CoverActors->BlockingVol_Right->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
			CoverActors->BlockingVolume_Front->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
	}
}

void AMonsieurMonet_Character::ToggleInventory()
{
	//Checks players' hud state. closes if inventory is already open, else opens it up.
	AMonsieurMonetGameModeBase* MMGameMode = Cast<AMonsieurMonetGameModeBase>(GetWorld()->GetAuthGameMode());
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (MMGameMode != NULL)
	{
		if (MMGameMode->GetHudState() == MMGameMode->HS_InGame)
		{
			MMGameMode->ChangeHUDState(MMGameMode->HS_Inventory);
			PlayerController->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			MMGameMode->ChangeHUDState(MMGameMode->HS_InGame);
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMonsieurMonet_Character::Aim_Implementation()
{
	UWorld* world = GetWorld();

	float ThrowableRadius = 350.0f;


	if (bCrouching == false)
	{
		bIsAiming = true;
		GetCharacterMovement()->MaxWalkSpeed = 75.0f;
		NoiseLevel = 0.0f;
		bUseControllerRotationYaw = true;
		NoiseRadius = 0.0f;
		NoiseRadiusComp->SetSphereRadius(NoiseRadius);
	}
	else if (bCrouching == true)
	{
		bIsAiming_Crouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 70.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 50.0f;
		NoiseLevel = 0.0f;
		bUseControllerRotationYaw = true;
		NoiseRadius = 250.0f;
		NoiseRadiusComp->SetSphereRadius(NoiseRadius);
	}


	if (bIsAiming)
	{
		//FHitResult Hit;
		//TArray<FVector> OutHits;
		//FVector LastLocation;
		//FVector StartPos = LureStart->GetComponentLocation();
		//FVector LaunchVel = PlayerCamera->GetForwardVector * 2500.0F;
		//TArray<AActor*> ActorsIgnored;


		////tracing for the projectile location probablility
		//UGameplayStatics::Blueprint_PredictProjectilePath_ByTraceChannel(world, Hit, OutHits, LastLocation, StartPos, LaunchVel, true, 32.0F,
		//	ECollisionChannel::ECC_Visibility, false, ActorsIgnored,
		//	EDrawDebugTrace::None, 0.0F);
		////If the trace hit a location
		//if (Hit.bBlockingHit)
		//{
		//	FTransform item;
		//	item.SetLocation(Hit.ImpactPoint);
		//	//item.SetRotation(Hit.ImpactNormal);

		//	TargetEndDecal->SetWorldTransform(item);
		//	TargetEndDecal->SetVisibility(true);
		//}
	}
}

void AMonsieurMonet_Character::AimStop_Implementation()
{
	if (bCrouching == false)
	{
		/*Aim_ReverseTimeline();*/
		bIsAiming = false;
		bUseControllerRotationYaw = false;
		PlayerCamera->SetRelativeLocation(FVector(0.0f, AimCameraPos_R, AimCameraPos_R));
		GetCharacterMovement()->MaxWalkSpeed = 180.0f;
		CameraBoom->bUsePawnControlRotation = true;
		//bIsShooting = false;
	}
	else if (bCrouching == true)
	{/*
		Aim_ReverseTimeline();*/
		bIsAiming_Crouch = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 180.0f;
		PlayerCamera->SetRelativeLocation(FVector(0.0f, AimCameraPos_R, AimCameraPos_R));
		GetCharacterMovement()->MaxWalkSpeedCrouched = 110.0f;
		CameraBoom->bUsePawnControlRotation = true;
		//bIsShooting = false;
	}
}

void AMonsieurMonet_Character::Shoot_Implementation()
{
	if (bIsAiming == false && bIsAiming_Crouch == false)
	{
		if (bTriggered == true)
		{
			CanTakeDown = true;
			IsAttacking = true;
			PlayAnimMontage(TakedownMontage, 1.0f, NAME_None);
		}
	}

	if (bIsAiming == true && bIsAiming_Crouch == true)
	{
		//Code for throwing items.

	}
}

void AMonsieurMonet_Character::WeaponCooldown()
{
	bIsShooting = true;
}

void AMonsieurMonet_Character::PauseGame()
{
	AMonsieurMonetGameModeBase* MMGameMode = Cast<AMonsieurMonetGameModeBase>(GetWorld()->GetAuthGameMode());
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	class APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (MMGameMode != NULL)
	{
		if (MMGameMode->GetHudState() != MMGameMode->HS_PauseGame)
		{
			MMGameMode->ChangeHUDState(MMGameMode->HS_PauseGame);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());
		}
		else
		{
			MMGameMode->ChangeHUDState(MMGameMode->HS_InGame);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMonsieurMonet_Character::PlayerCrouch()
{
	if (bIsAiming == false)
	{
		if (GetCharacterMovement()->IsCrouching())
		{
			bCrouching = true;
			NoiseLevel = 0.0f;
			CrouchCameraAnimation.Reverse();
			UnCrouch();
			NotifyPawnNoise();
			NoiseRadius = 0.0f;
			NoiseRadiusComp->SetSphereRadius(NoiseRadius);
			CrouchWalkTL.Reverse();
		}
		else
		{
			if (SprintCameraAnimation.IsPlaying() == true)
			{
				SprintCameraAnimation.Reverse();
			}
			bCrouching = false;
			CrouchCameraAnimation.Play();
			Crouch();
			NotifyPawnNoise();
			NoiseRadius = 250.0f;
			NoiseRadiusComp->SetSphereRadius(NoiseRadius);
		}
	}
}

void AMonsieurMonet_Character::PlayerCrouch_Stop()
{
	/*bCrouching = false;
	GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchRate, true);
	if (bIsAiming == false && bIsAiming_Crouch == false)
		PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, CrouchCamera_R), false);*/
}

void AMonsieurMonet_Character::Cover()
{
	if (bCrouching == false)
	{
		bEnterCover = true;
		bCrouchCover = false;
	}
	else if (bCrouching == true)
	{
		bCrouchCover = true;
		bEnterCover = false;
	}
}

//this is for artifacts
bool AMonsieurMonet_Character::AddItemToInventory(AMM_LureItems_Base * Item)
{
	if (Item != nullptr)
	{
		const int32 AvailableSlots = Inventory.Find(nullptr);
		if (AvailableSlots != INDEX_NONE)
		{
			SlotEmpty = true;
			Item->OnPickedUp();
			Inventory[AvailableSlots] = Item;
			/*if (Inventory.Num() > 4)
			{
				Inventory.RemoveAt(0, 1, false);
			}*/
			return true;
		}
		/*else
		{
		SlotEmpty = false;
		return false;
		}*/
	}
	else
	{
		SlotEmpty = false;
		return false;
	}
	return false;
}

void AMonsieurMonet_Character::CheckForInteractables()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		//SearchRange->OnComponentBeginOverlap.AddDynamic(this, &AMonsieurMonet_Character::SearchRangeOnOverlap);
		//Create TArray for the HitResults of the Sphere Trace.
		TArray<FHitResult> OutHits;

		//Start and End Locations.
		FVector SweepStart = GetActorLocation();
		FVector SweepEnd = GetActorLocation() + RayLocation;

		//Creating the Sphere Collision Trace.
		FCollisionShape MyCollisionSphere = FCollisionShape::MakeSphere(150.0f);
		//Drawing the Debug Sphere.
		//DrawDebugSphere(GetWorld(), SweepStart, MyCollisionSphere.GetSphereRadius(), 50, FColor::Blue, true);

		//Check if the Interactable objects are detected in the Sweep/Trace.
		bool IsHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, MyCollisionSphere);
		if (IsHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(IsHit));

			for (int i = 0; i < OutHits.Num(); i++)
			{
				//Check for artifacts.
				//class AMM_Pickups* Interactables = Cast<AMM_Pickups>(OutHits[i].GetActor());
				class AMM_LureItems_Base* LureItems = Cast<AMM_LureItems_Base>(OutHits[i].GetActor());

				//Check for artifacts
				//if (OutHits[i].GetActor() == Interactables)
				//{
				//	if (Interactables != nullptr)
				//	{
				//		PlayAnimMontage(PickupMontage, 1.0f, NAME_None);
				//		CurrentInteractable = Interactables;
				//		bPickedUpP = true;
				//		PickupItemss = Interactables;
				//		/*World->GetTimerManager().SetTimer(PickupTimerHandle, this, &AMonsieurMonet_Character::PUPickedUp, 6.0f, false);*/
				//	}
				//	else
				//	{
				//		CurrentInteractable = nullptr;
				//		PickupItemss = nullptr;
				//		PickupText = FString("");
				//		bPickedUpP = false;
				//		return;
				//	}
				//}


				//Check for lure items.
				if (OutHits[i].GetActor() == LureItems)
				{
					if (LureItems != nullptr)
					{
						PlayAnimMontage(PickupMontage, 1.0f, NAME_None);
						NearestLureItem = LureItems;
						/*World->GetTimerManager().SetTimer(PickupTimerHandle, this, 4.8f, false);*/
						bPickedUpL = true;
						NearestLureItem->Interact(this);
						NearestLureItem->OnPickedUp();
					}
					else
					{
						NearestLureItem = nullptr;
						PickupText = FString("");
						bPickedUpL = false;
						return;
					}
				}
			}
		}
	}
}

void AMonsieurMonet_Character::UpdateGold(int32 Amount)
{
	Gold = Gold + Amount;
}

UTexture2D * AMonsieurMonet_Character::GetInventorySlotThumbnail(int32 Slot)
{
	/*if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->LureThumbnail;
	}
	else
	{
	}*/
	return nullptr;
}

FString AMonsieurMonet_Character::GivenNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->Name;
	}
	else
	{
		return FString("None");
	}
}

void AMonsieurMonet_Character::ThrowLures(int32 Slot)
{
	//if (Inventory[Slot] != NULL)
	//{
	//	return Inventory[Slot]->LThrow();
	//	/*Inventory[Slot] = NULL;*/
	//}
}




void AMonsieurMonet_Character::SearchRangeOnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	class AMM_Pickups* Interactables = Cast<AMM_Pickups>(OtherActor);
	if (OtherActor == Interactables && Interactables != nullptr)
	{
		CurrentInteractable = Interactables;
		PickupText = Interactables->InteractableHelpText;
	}
	else
	{
		PickupText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
}

void AMonsieurMonet_Character::SetCanTakeCover(bool CanTakeCover, AMM_CoverActor * CoverActor)
{
	if (!CanTakeCover && bIsInCover)
	{
		ToggleCover();
	}

	bCanTakeCover = CanTakeCover;
	Covers = CoverActor;
}

void AMonsieurMonet_Character::ToggleCover()
{
	if (bCanTakeCover)
	{
		bIsInCover = !bIsInCover;
		if (bIsInCover && Covers)
		{
			//this screws up the movement :(
			/*GetCharacterMovement()->bOrientRotationToMovement = false;*/
			/*CameraBoom->bUsePawnControlRotation = false;*/
			FRotator CoverRotation;
			Covers->RetrieveDirectionAndRotation(CoverMovementDirection, CoverRotation);
			SetActorRotation(CoverRotation);
			Covers->PlayerCoverEvent();
		}
	}
}

void AMonsieurMonet_Character::StartHealthregen()
{
	if (Health <= 0)
	{
		FMath::FInterpTo(0, 100, 3, 15);
	}
}

void AMonsieurMonet_Character::ModifyHealth(float Amount, UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser, float & Health_out, bool & AddingHealth_out)
{
	float AmountLocal = Amount;
	UDamageType* DamageTypeLocal = DamageType;
	AController* InstigatedByLocal = InstigatedBy;
	AActor* DamageCauserLocal = DamageCauser;
	Health_out = Health;
	bool AddingHealth = AmountLocal < 0;
	AddingHealth_out = AddingHealth;
	if (AddingHealth == true)
	{

	}
}

void AMonsieurMonet_Character::TakeDown(AActor * TDPawn)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("yay"));*/
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(TDPawn);
	SwiftGuard_Character->PlayAnimMontage(SwiftGuard_Character->TakendownMontage, 1.0f, NAME_None);
	SwiftGuard_Character->IsStunned = true;
}

void AMonsieurMonet_Character::TakeDownCB(AActor* CBoxes)
{
	AMM_CircuitBoxes_Camera* CircuitBoxes = Cast<AMM_CircuitBoxes_Camera>(CBoxes);
	CircuitBoxes->Disabled = true;
}

void AMonsieurMonet_Character::PauseAnimMontages()
{
	GetMesh()->bPauseAnims = true;
}

void AMonsieurMonet_Character::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//To take down guards and circuitboxes.
	ASwift_GuardCharacter* SwiftGuard_Character = Cast<ASwift_GuardCharacter>(OtherActor);
	AMM_CircuitBoxes_Camera* CircuitBoxes = Cast<AMM_CircuitBoxes_Camera>(OtherActor);
	if (OtherActor == SwiftGuard_Character && SwiftGuard_Character != nullptr)
	{
		if (IsAttacking == true)
		{
			TakeDown(SwiftGuard_Character);
		}
	}
	else if (OtherActor == CircuitBoxes && CircuitBoxes != nullptr)
	{
		if (IsAttacking == true)
		{
			TakeDownCB(CircuitBoxes);
		}
	}
}

void AMonsieurMonet_Character::OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IsAttacking = false;
}


void AMonsieurMonet_Character::PUPickedUp()
{
	//AMM_Pickups* PickupItems = Cast<AMM_Pickups>(CurrentInteractable);
	//PickupItems->OnPickedUp();
	//PickupText = CurrentInteractable->InteractableHelpText;
}

void AMonsieurMonet_Character::AddItemToInventoryByID(int ID)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	//FString ContextString;

	//AMonsieurMonetGameModeBase* StealthGameMode = Cast<AMonsieurMonetGameModeBase>(world->GetAuthGameMode());
	//UDataTable* LureTable = StealthGameMode->GetLureItemDB();

	//FLureItemInventory* ItemToAdd = LureTable->FindRow<FLureItemInventory>(ID, ContextString);

	//if (ItemToAdd)
	//{
	//	//switch(Name):
	//	//	case bombs:
	//	//		break;
	//	//add item to inventory
	//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT(Name));
	//	//LInventory.Add(*ItemToAdd);
	//	bombs++;

	//}
	switch (ID)
	{
	case 1: //Coins
		Coins++;
		break;

	case 2:	//Peels
		Peels++;
		break;

	case 3:	//HealthRegen
		HealthRegen++;
		break;

	case 4:	//Bombs
		Bombs++;
		break;

	default:
		//Do nothing
		break;
	}
}

void AMonsieurMonet_Character::LIPickedUp()
{
	AMM_LureItems_Base* LureItemBase = Cast<AMM_LureItems_Base>(NearestLureItem);
	PickupText = LureItemBase->LureHelpText;
	AddItemToInventoryByID(NearestLureItem->ItemID);
	/*AddItemToInventory(LureItemBase);*/
}

//Connect to Inventory UI
void AMonsieurMonet_Character::SelectCoin()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Coins are the current lure item"));
	if (Coins > 0)
	{
		CurrentLureItem = "Coins";
	}
}
void AMonsieurMonet_Character::SelectPeel()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Peels are the current lure item"));
	if (Peels > 0)
	{
		CurrentLureItem = "Peels";
	}
}
void AMonsieurMonet_Character::SelectHealthRegen()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("HealthRegen are the current lure item"));
	if (HealthRegen > 0)
	{
		CurrentLureItem = "HealthRegen";
	}
}
void AMonsieurMonet_Character::SelectBomb()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Bombs are the current lure item"));
	if (Bombs > 0)
	{
		CurrentLureItem = "Bombs";
	}
}